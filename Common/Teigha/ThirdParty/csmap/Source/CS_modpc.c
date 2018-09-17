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
**	err_cnt = CSmodpcQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSmodpcQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MinLatFz;		/* - 89.999 */
	extern double cs_MaxLatFz;		/* + 89.999 */
	extern double cs_ParmTest;		/* .1 seconds of arc in degrees */
	extern double cs_K15;			/* 15.0 */

	int err_cnt;

	double tmp;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->prj_prm2 <= cs_MinLng || cs_def->prj_prm2 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDLNG;
	}

	tmp = cs_def->prj_prm2 - cs_def->prj_prm1;
	if (fabs (tmp) < cs_ParmTest)
	{
		/* The two meridians cannot be the same. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LNGEQU;
	}
	if (tmp < 0.0)
	{
		/* Standard meridian must east of the central meridian. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDWEST;
	}
	else if (tmp > cs_K15)
	{
		/* Standard meridian must not be more than 15 degrees away
		   from the central meridian. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_MAX15;
	}

	/* Neither standard latitude should be a pole. */

	if (cs_def->prj_prm3 <= cs_MinLatFz || cs_def->prj_prm3 >= cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_NRTHLAT;
	}
	if (cs_def->prj_prm4 <= cs_MinLatFz || cs_def->prj_prm4 >= cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SOTHLAT;
	}

	tmp = cs_def->prj_prm3 - cs_def->prj_prm4;
	if (fabs (tmp) < cs_ParmTest)
	{
		/* The two parallels cannot be the same. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LATEQU;
	}
	if (tmp < 0.0 || tmp >cs_K15)
	{
		/* Prm3 must be the northernmost of the two standard
		   parallels, and within 15 degrees of the southern
		   parallel. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDSOU;
	}

	/* This projection was designed for a difference between the
	   two standard parallels of 2 degrees.  It is not known if
	   using largely different values causes a problem.  We'll
	   have to test. */

	/* That's it for Modified Polyconic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CSmodpcS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate system
**								parameters.
**********************************************************************/

void EXP_LVL9 CSmodpcS (struct cs_Csprm_ *csprm)

{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.29...  */
	extern double cs_Radian;			/* 57.29...  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_K45;				/* 45.0 */
	extern double cs_Km45;				/* -45.0 */
	extern double cs_Pi_o_4;			/* Pi over 4 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the North pole,
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the South pole,
										   in radians. */

	struct cs_Modpc_ *modpc;

	double M1, M2;
	double F1, F2;
	double xx1, xx2;
	double yy1, yy2;
	double T2;
	double del_lng;
	double del_M;
	double cot_lat1, cot_lat2;
	double tmp1;
	double tmp2;

	double test_ll [3];
	double test_xy [3];

	modpc = &csprm->proj_prms.modpc;

	modpc->lng0 = csprm->csdef.prj_prm1 * cs_Degree;
	modpc->lng1 = csprm->csdef.prj_prm2 * cs_Degree;
	modpc->lat2 = csprm->csdef.prj_prm3 * cs_Degree;
	modpc->lat1 = csprm->csdef.prj_prm4 * cs_Degree;
	modpc->x_off = csprm->csdef.x_off;
	modpc->y_off = csprm->csdef.y_off;
	modpc->k = csprm->csdef.scale;
	modpc->ecent = csprm->datum.ecent;
	modpc->e_sq = modpc->ecent * modpc->ecent;
	modpc->e_rad = csprm->datum.e_rad;
	modpc->ak = modpc->e_rad * modpc->k;
	modpc->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Force lat1 and lat2 to hard zeros if they are very close
	   to the equator. */

	if (fabs (modpc->lat1) < cs_AnglTest) modpc->lat1 = cs_Zero;
	if (fabs (modpc->lat2) < cs_AnglTest) modpc->lat2 = cs_Zero;

	/* We set limits on the latitude, for fast checking in the
	   forward function. */

	modpc->n_limit = modpc->lat2 + (modpc->lat2 - modpc->lat1);
	if (modpc->n_limit > cs_NPTest) modpc->n_limit = cs_NPTest;
	modpc->s_limit = modpc->lat1 + (modpc->lat1 - modpc->lat2);
	if (modpc->s_limit < cs_SPTest) modpc->s_limit = cs_SPTest;

	/* We have no information concerning a spherical implementation
	   of this projection.  However, we have determined that the
	   ellipsiodal implementation which is coded below appears to
	   work for the sphereical case, i.e. where the eccentricity
	   is zero.

	   Set up the M calculations. */

	CSmmFsu (&modpc->mmcofF,modpc->ak,modpc->e_sq);
	modpc->sin_lat1 = sin (modpc->lat1);
	modpc->sin_lat2 = sin (modpc->lat2);
	modpc->cos_lat2 = cos (modpc->lat2);

	/* Small_xy is set to be the equivalent of one millimeter
	   in the cartesian system unit.  We iterate the inverse
	   calculation until the difference is less than this
	   value. */

	modpc->small_xy = 0.001 * modpc->k;

	/* Now we compute M1 and M2 associated with the defining latiudes.
	   These are the meridonal distances from the equator to the
	   respective latitudes. */

	M1 = CSmmFcal (&modpc->mmcofF,modpc->lat1,modpc->sin_lat1,cos (modpc->lat1));

	M2 = CSmmFcal (&modpc->mmcofF,modpc->lat2,modpc->sin_lat2,modpc->cos_lat2);

	/* Now we work our way toward the P's and Q's. */

	del_lng = modpc->lng1 - modpc->lng0;
	F1 = del_lng * modpc->sin_lat1;
	F2 = del_lng * modpc->sin_lat2;
	if (modpc->lat1 != 0.0)
	{
		/* Lat1 is not the equator, so tan (lat1) is not zero. */

		cot_lat1 = cs_One / tan (modpc->lat1);
		tmp1 = cs_One - (modpc->e_sq * modpc->sin_lat1 *
					      modpc->sin_lat1);
		modpc->R1 = modpc->ak * cot_lat1 / sqrt (tmp1);
		xx1 = modpc->R1 * sin (F1);
		yy1 = modpc->R1 * (cs_One - cos (F1));
	}
	else
	{
		/* Lat1 is zero. */

		xx1 = modpc->ak * del_lng;
		yy1 = cs_Zero;
	}

	if (modpc->lat2 != 0.0)
	{
		cot_lat2 = cs_One / tan (modpc->lat2);
		tmp1 = cs_One - (modpc->e_sq * modpc->sin_lat2 *
					      modpc->sin_lat2);
		modpc->R2 = modpc->ak * cot_lat2 / sqrt (tmp1);
		xx2 = modpc->R2 * sin (F2);
		T2 = modpc->R2 * (cs_One - cos (F2));
	}
	else
	{
		xx2 = modpc->ak * del_lng;
		T2 = cs_Zero;
	}

	del_M = M2 - M1;
	tmp2 = xx2 - xx1;
	yy2 = sqrt ((del_M * del_M) - (tmp2 * tmp2)) + yy1;

	modpc->C2 = yy2 - T2;

	modpc->P_y = ((M2 * yy1) - (M1 * yy2)) / del_M;
	modpc->Q_y = (yy2 - yy1) / del_M;
	modpc->P_x = ((M2 * xx1) - (M1 * xx2)) / del_M;
	modpc->Q_x = (xx2 - xx1) / del_M;

	/* Now we need to determine some checking values.  The value
	   max_xx is the maximum magnitude of x (sans false orgin).
	   We use the equivalent to a delta longitude of 45 degrees. */

	tmp1 = (modpc->R1 > modpc->R2) ? modpc->R1 : modpc->R2;
	modpc->max_xx = cs_Pi_o_4 * tmp1;

	/* Similarly with max and min yy. We can use the CSmodpcF function
	   here, as it does not reference max_yy or min_yy.  Only
	   CSmodpcI does that. */

	test_ll [LNG] = modpc->lng0 * cs_Radian;
	test_ll [LAT] = modpc->s_limit * cs_Radian;
	CSmodpcF (modpc,test_xy,test_ll);				/*lint !e534 */
	modpc->min_yy = test_xy [YY];
	test_ll [LAT] = modpc->n_limit * cs_Radian;
	CSmodpcF (modpc,test_xy,test_ll);				/*lint !e534 */
	modpc->max_yy = test_xy [YY];

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = modpc->lng0 * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km45;
		csprm->max_ll [LNG] = cs_K45;
		csprm->min_ll [LAT] = modpc->s_limit * cs_Radian;
		csprm->max_ll [LAT] = modpc->n_limit * cs_Radian;
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
		   We've already computed these, so this is easy in this
		   case. */

		csprm->min_xy [XX] = -modpc->max_xx;
		csprm->max_xy [XX] =  modpc->max_xx;
		csprm->min_xy [YY] =  modpc->min_yy;
		csprm->max_xy [YY] =  modpc->max_yy;

		CS_quadMM (csprm->min_xy,csprm->max_xy,modpc->x_off,
											   modpc->y_off,
											   modpc->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSmodpcF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSmodpcI;
	csprm->cs_scale = (cs_SCALE_CAST)CSmodpcH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSmodpcK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSmodpcH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSmodpcC;
	csprm->llchk    = (cs_LLCHK_CAST)CSmodpcL;
	csprm->xychk    = (cs_XYCHK_CAST)CSmodpcX;

	return;
}

/**********************************************************************
**	(void) CSmodpcB (modpc,ll,xy,their_yc);
**
**	struct cs_Modpc_ *modpc;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				the longitude ([0]) and latitude ([1]) values
**								to be converted, in radians.
**	double xy [2];				converted values are returned here, x ([0])
**								and y ([1]).
**	double *their_yc;			the computed value of the yc variable is
**								returned here.  This is required by the
**								inverse function.
**
**	This function represents the bulk of the Modified
**	Polyconic Forward function in a slightly modified
**	form.  This is desireable as the inverse function
**	essentially is an interative loop of the forward
**	function.
**
**	The ll and the xy arrays may share the same memory
**	with no adverse affects.
**********************************************************************/

int EXP_LVL9 CSmodpcB (	Const struct cs_Modpc_ *modpc,
						Const double ll [2],
						double xy [2],
						double *their_yc)

{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/* 3.14159 */
	extern double cs_Mpi;				/* -3.14159 */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_4;			/* Pi over 4 */
	extern double cs_Mpi_o_4;			/* - Pi over 4 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	int rtn_val;

	double lng;
	double lat;
	double R, C, M;
	double D, B;
	double R_sq, D_sq;
	double xa, xb, xc;
	double ya, yb, yc;
	double cot_lat;
	double sin_lat;
	double cos_lat;
	double del_lng;
	double tmp1, tmp2;

	R = R_sq = 0.0;							/* to keep gcc happy */
	rtn_val = cs_CNVRT_NRML;

	lng = ll [LNG];
	lat = ll [LAT];

	/* Extremes of latitude cause major problems with this projection,
	   it was designed for latitudes within the standard parallels.
	   Once we get significantly outside of this range, strange
	   things start to happen.  The north and south limits should
	   always exclude the poles. */

	if (lat > modpc->n_limit)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = modpc->n_limit;
	}
	if (lat < modpc->s_limit)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = modpc->s_limit;
	}

	/* A del_lng greater than a couple of degrees is outside of the
	   useful range of this projection, but it doesn't cause a blow
	   up until del_lng gets rather large.  While I must admit that
	   I don't fully understand the formulas programmed below, I
	   have determined that a del_lng greater than or equal to
	   1 radian will cause problems.  Therefore, I limit del_lng
	   to something a bit less than 1 radian, cs_Pi_o_4 immediately
	   comes to mind.

	   In any case, we need to normalize del_lng in case the central
	   meridian is close to the -180 crack. */

	del_lng = lng - modpc->lng0;
	if      (del_lng > cs_Pi  && modpc->lng0 < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && modpc->lng0 > 0.0) del_lng += cs_Two_pi;

	/* Del_lng is now something adjusted for the crack, but not
	   fully normalized, and we can perform rational error
	   checking on it. */

	if (fabs (del_lng) >= cs_Pi_o_4)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = (del_lng >= 0.0) ? cs_Pi_o_4 : cs_Mpi_o_4;
	}

	/* Now we need the M for the given latitude.  This is the
	   meridonal distance from the equator to the latitude of
	   the point. */

	sin_lat = sin (lat);
	cos_lat = cos (lat);
	M = CSmmFcal (&modpc->mmcofF,lat,sin_lat,cos_lat);

	/* Now we compute xa, ya. */

	xa = modpc->P_x + (modpc->Q_x * M);
	ya = modpc->P_y + (modpc->Q_y * M);

	/* We compute C now.  We must be careful here to avoid a divide
	   by zero.  The checking we did above on lat and del_lng
	   should preclude the second square root from blowing up, but
	   we guard against that just the same. */

	if (fabs (lat) > cs_AnglTest)
	{
		cot_lat = cs_One / tan (lat);
		tmp1 = cs_One - (modpc->e_sq * sin_lat * sin_lat);
		R = modpc->ak * cot_lat / sqrt (tmp1);
		R_sq = R * R;

		tmp1 = R_sq - (xa * xa);
		if (tmp1 <= 0.0)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp1 = cs_Zero;
		}
		else
		{
			tmp1 = sqrt (tmp1);
		}
		C = ya - R;
		if (lat > 0.0)  C += tmp1;
		else		C -= tmp1;
	}
	else
	{
		C = cs_Zero;
	}

	/* Now for xb, yb, xc, and yc.  Again, we have to careful
	   about the equator.  In this case, we must be careful
	   about standard parallels which are the equator.  CSmodpcS
	   sets these guys to hard zeros if they are very close
	   to the equator. */

	if (modpc->lat2 != 0.0)
	{
		/* The northernmost parallel is not on the equator.
		   We use the normal calculations. Here we can see why,
		   at large lataitudes anyway, a del_lng greater than
		   1 radian can be a problem.  This is why I have
		   determined to limit del_lng to 1 radian. */

		tmp2 = del_lng * modpc->sin_lat2;
		xb = modpc->R2 * sin (tmp2);

		tmp1 = cs_One - cos (tmp2);
		yb = modpc->C2 + (modpc->R2 * tmp1);
	}
	else
	{
		/* Opps!!! The northernmost standard parallel is
		   on the equator. */

		xb = modpc->ak * del_lng;
		yb = modpc->C2;
	}

	if (modpc->lat1 != 0.0)
	{
		/* The southernmost standard parallel is not the equator. */

		tmp2 = del_lng * modpc->sin_lat1;
		xc = modpc->R1 * sin (tmp2);
		tmp1 = cs_One - cos (tmp2);
		yc = modpc->R1 * tmp1;
	}
	else
	{
		/* The southernmost standard parallel is the equator. */

		xc = modpc->ak * del_lng;
		yc = cs_Zero;
	}
	*their_yc = yc;

	/* We're getting close.  We can now finish it off, paying special
	   attention to the equator and two square root's which could cause
	   problems. */

	if (fabs (lat) > cs_AnglTest)
	{
		D = (xb - xc) / (yb - yc);
		D_sq = D * D;
		tmp1 = C + R - yc;								/*lint !e644 */
		B = xc + (D * tmp1);

		tmp1 = (R_sq * (cs_One + D_sq)) - (B * B);		/*lint !e644 */
		if (tmp1 <= 0.0)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp1 = cs_Zero;
		}
		else
		{
			tmp1 = D * sqrt (tmp1);
		}

		if (lat > 0.0)  tmp1 = B - tmp1;
		else		tmp1 = B + tmp1;
		xy [XX] = tmp1 / (cs_One + D_sq);

		/* If xx is larger than R, the following blows up.
		   This is why we have chosen a limit of 1.0 on
		   del_lng. */

		tmp1 = R_sq - (xy [XX] * xy [XX]);
		if (tmp1 <= 0.0)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp1 = cs_Zero;
		}
		else
		{
			tmp1 = sqrt (tmp1);
		}
		if (lat >= 0.0)  xy [YY] = C + R - tmp1;
		else		 xy [YY] = C + R + tmp1;
	}
	else
	{
		xy [XX] = modpc->ak * del_lng;
		xy [YY] = C;
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSmodpcF (modpc,xy,ll);
**
**	struct cs_Modpc_ *modpc;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned
**								here, x ([0]) and y ([1]).
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

int EXP_LVL9 CSmodpcF (Const struct cs_Modpc_ *modpc,double xy [2],Const double ll [2])
{
	extern double cs_Degree;		/* 1.0 / RADIAN  */

	int rtn_val;

	double dbl_dum;

	double my_ll [2];

	/* Convert the latitude and longitude to radians. */

	my_ll [LNG] = ll [LNG] * cs_Degree;
	my_ll [LAT] = ll [LAT] * cs_Degree;

	/* The CSmodpcB function contains the bulk of the forward
	   calculation.  This is desireable as the inverse function
	   also needs access to the basic forward calculation, thus
	   the suffix letter: B for Both. */

	rtn_val = CSmodpcB (modpc,my_ll,xy,&dbl_dum);

	/* Add the false easting and northing. */

	if (modpc->quad == 0)
	{
		xy [XX] += modpc->x_off;
		xy [YY] += modpc->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],modpc->x_off,modpc->y_off,modpc->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSmodpcI (modpc,ll,xy);
**
**	struct cs_Modcn_ *modpc;	structure which defines the parameters
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
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", pages 134-135.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSmodpcI (Const struct cs_Modpc_ *modpc,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int status;
	int rtn_val;
	int it_count;

	double x;
	double y;
	double yc;

	double ll_test [2];
	double xy_test [2];
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false easting and northing from the provided
	   coordinates. */

	if (modpc->quad == 0)
	{
		x = xy [XX] - modpc->x_off;
		y = xy [YY] - modpc->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,modpc->x_off,modpc->y_off,modpc->quad);
	}

	/* In the forward function, we limit del_lng to Pi/4 to
	   avoid many problems in the mathemagics.  Here, we need to
	   limit x to a value that will produce a del_lng of Pi/4. */

	if (fabs (x) > modpc->max_xx)
	{
		rtn_val = cs_CNVRT_RNG;
		x = (x >= 0.0) ? modpc->max_xx : -modpc->max_xx;
	}

	/* Similarly with the y, but y is relative to lat1, so there
	   is a min and a max value. */

	if (y > modpc->max_yy)
	{
		rtn_val = cs_CNVRT_RNG;
		y = modpc->max_yy;
	}
	if (y < modpc->min_yy)
	{
		rtn_val = cs_CNVRT_RNG;
		y = modpc->min_yy;
	}

	/* Compute the first lat/long test value.  Note, this blows
	   up if lat2 is either pole; but CSmodpcQ should take care
	   of that situation.  If it doesn't, this whole thing is in
	   big trouble. */

	ll_test [LNG] = (x / (modpc->ak * modpc->cos_lat2)) + modpc->lng0;
	ll_test [LAT] = modpc->lat2;

	/* Use these test values to compute the first set of
	   xy_test values.  We then use the new xy_test values
	   to compute new ll_test values.  When xy_test XX and YY
	   get within a millimeter of the desired values, ll_test
	   LNG and LAT are the desired values.  We set up a loop
	   counter so that we know if we are in an infinite loop,
	   i.e. the calculation is not converging. */

	it_count = -10;
	for (;;)
	{
		/* The CSmodpcB function is the bulk of the
		   forward calculation.  It has been expressly
		   setup to be used by both the forward and
		   inverse functions (i.e. B for Both). */

		status = CSmodpcB (modpc,ll_test,xy_test,&yc);
		if (status != cs_CNVRT_NRML) rtn_val = cs_CNVRT_RNG;

		/* See if the convergence is close enough. */

		if ((fabs (x - xy_test [XX]) <= modpc->small_xy &&
			 fabs (y - xy_test [YY]) <= modpc->small_xy))
		{
			/* We're close enough.  Break out of loop. */

			break;
		}

		/* See if we've iterated more than the expected number.
		   If so, we'll assume that the values given are way
		   out of range. */

		if (it_count++ > 0)
		{
			rtn_val = cs_CNVRT_RNG;
			break;
		}

		/* Not close enough yet.  Small_xy is set up for about
		   a millimeter.  This might be too small, but we'll
		   stick to it until we get some complaints.  Set up
		   for the next iteration, but don't divide by zero. */

		if (fabs (xy_test [XX]) > modpc->small_xy)
		{
			tmp1 = (ll_test [LNG] - modpc->lng0) * x / xy_test [XX];
			ll_test [LNG] = modpc->lng0 + tmp1;
		}

		if (fabs (xy_test [YY] - yc) > modpc->small_xy)
		{
			tmp1 = ((ll_test [LAT] - modpc->lat1) * (y - yc)) /
								(xy_test [YY] - yc);
			ll_test [LAT] = modpc->lat1 + tmp1;
		}

		/* Repeat the calculation with the new test values. */
	} 

	/* Convert the result to degrees.  This projection should never
	   be used near the poles, but just in case: */

	if (fabs (ll_test [LAT]) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
		rtn_val = cs_CNVRT_INDF;
	}
	ll [LNG] = ll_test [LNG] * cs_Radian;
	ll [LAT] = ll_test [LAT] * cs_Radian;

	/* That's it. */

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSmodpcC (modpc,ll);
**
**	struct cs_Modpc_ *modpc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Modified Polyconic
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

double EXP_LVL9 CSmodpcC (Const struct cs_Modpc_ *modpc,Const double ll [2])
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
	status = CSmodpcF (modpc,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSmodpcF (modpc,xy2,my_ll);
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
**	kk = CSmodpcK (modpc,ll);
**
**	struct cs_Modpc_ *modpc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Modified Polyconic
**								projection.
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

double EXP_LVL9 CSmodpcK (Const struct cs_Modpc_ *modpc,Const double ll [2])
{
	extern double cs_Mone;			/* -1.0 */
	extern double cs_SclInf;		/* 9.9E+04, value we return for
									   infinite scale factors. */
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
	   about 1 second (about 30 meters) apart from each
	   other, with the point in question in the middle.
	   Then convert each point to the equivalent grid
	   coordinates. */

	ll1 [LNG] = ll [LNG] - (0.5 / 3600.0);
	ll1 [LAT] = ll [LAT];
	status = CSmodpcF (modpc,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG] + (0.5 / 3600.0);
	ll2 [LAT] = ll [LAT];
	status = CSmodpcF (modpc,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	CS_llazdd (modpc->ak,modpc->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point.  I don't really know
	   what happens to this projection at the poles, so: */

	if (ll_dd > modpc->small_xy)
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
**	hh = CSmodpcH (modpc,ll);
**
**	struct cs_Modpc_ *modpc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Modified Polyconic
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double hh;					returns the actual scale factor, along a
**								meridian, of the projected coordinate system
**								at the specified point.
**
**	We have not as yet located any specific information on
**	analytical formulas for determining the H scale factor
**	for this projection.  Therefore, we simply sample the
**	projection at two points which are close to each other
**	and return the ration of geodetic and grid distance.
**********************************************************************/

double EXP_LVL9 CSmodpcH (Const struct cs_Modpc_ *modpc,Const double ll [2])
{
	extern double cs_Mone;			/* -1.0 */
	extern double cs_SclInf;		/* 9.9E+04, the value we return
									   for infinite scale factors. */
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
	ll1 [LAT] = ll [LAT] - (0.5 / 3600.0);
	status = CSmodpcF (modpc,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + (0.5 / 3600.0);
	status = CSmodpcF (modpc,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	(void)CS_llazdd (modpc->ak,modpc->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the meridian
	   scale factor at the indicated point.  I don't know what
	   happens to this projection at the poles, so: */

	if (fabs (ll_dd) > modpc->small_xy)
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
**	status = CSmodpcL (modpc,cnt,pnts);
**
**	struct cs_Modpc_ *modpc;	the coordinate system against which the check is
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

int EXP_LVL9 CSmodpcL (Const struct cs_Modpc_ *modpc,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_4;		/* PI over 4 */

	int ii;
	int status;

	double lat;
	double del_lng;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (lat > modpc->n_limit || lat < modpc->s_limit)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		del_lng = (pnts [ii][LNG] * cs_Degree) - modpc->lng0;
		if (fabs (del_lng) > cs_Pi_o_4)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSmodpcX (modpc,cnt,pnts);
**
**	struct cs_Mstro_ *modpc;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSmodpcL to check lat/long
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

int EXP_LVL9 CSmodpcX (Const struct cs_Modpc_ *modpc,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double xx;
	double yy;

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.  If
	   all points are valid, all lines will be valid, as well as
	   any region defined by the lines regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		if (modpc->quad == 0)
		{
			xx = pnts [ii][XX] - modpc->x_off;
			yy = pnts [ii][YY] - modpc->y_off;
		}
		else
		{
			CS_quadI (&xx,&yy,pnts [ii],modpc->x_off,
						    modpc->y_off,
						    modpc->quad);
		}
		if (fabs (xx) > modpc->max_xx)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
		if (yy > modpc->max_yy || yy < modpc->min_yy)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
