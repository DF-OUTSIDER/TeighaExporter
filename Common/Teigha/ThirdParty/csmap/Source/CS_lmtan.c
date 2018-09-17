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
**	err_cnt = CSlmtanQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSlmtanQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLatFz;		/* - 89.99 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */
	extern double cs_MaxLatFz;		/* + 89.99 */
	extern double cs_ParmTest;		/* + 2.78E-05 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Lambert Tangential specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat <= cs_MinLatFz || cs_def->org_lat >= cs_MaxLatFz ||
		fabs (cs_def->org_lat) < cs_ParmTest)
	{
		/* Origin latitude is also the tangential point.  Can't
		   be either pole, or the equator (we wouldn't have a
		   cone). */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	
	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* That's it for Lambert Tangential. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	void CSlmtanS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSlmtanS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Pi;				/*  PI */
	extern double cs_Pi_o_4;			/*  PI / 4.0     */
	extern double cs_Radian;			/* 57.2..... */
	extern double cs_Degree;			/* 1.0 / 57.2...  */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_K15;				/* 15.0 */
	extern double cs_K45;				/* 45.0 */
	extern double cs_Km45;				/* -45.0 */
	extern double cs_MinLatFz;			/* -89.999777 */
	extern double cs_MaxLatFz;			/*  89.999777 */

	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */

	struct cs_Lmtan_ *lmtan;

	double tmp1;
	double tmp2;
	double tmp3;

	double test_ll [3];
	double test_xy [3];

	lmtan = &csprm->proj_prms.lmtan;

	/* Transfer the necessary arguments to the
	   "lmtan" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	lmtan->org_lng = csprm->csdef.org_lng * cs_Degree;
	lmtan->org_lat = csprm->csdef.org_lat * cs_Degree;
	lmtan->kerad = csprm->datum.e_rad * csprm->csdef.scale;
	lmtan->k = csprm->csdef.scale * csprm->csdef.scl_red;
	lmtan->x_off = csprm->csdef.x_off;
	lmtan->y_off = csprm->csdef.y_off;
	lmtan->ecent = csprm->datum.ecent;
	lmtan->e_sq = lmtan->ecent * lmtan->ecent;
	lmtan->e_ovr_2 = lmtan->ecent * cs_Half;
	lmtan->ka = csprm->datum.e_rad * lmtan->k;
	lmtan->sin_org_lat = sin (lmtan->org_lat);
	lmtan->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	lmtan->N0 = lmtan->ka / sqrt (cs_One - lmtan->e_sq *
								  lmtan->sin_org_lat *
								  lmtan->sin_org_lat);

	/* R0 is the radius of the arc which represents the origin
	   latitude.

	   Origin latitude of zero is invalid, and should be (is)
	   caught in CSlmtanQ. */

	lmtan->R0 = lmtan->N0 / tan (lmtan->org_lat);

	tmp1 = cs_Pi_o_4 + (lmtan->org_lat * cs_Half);
	tmp1 = tan (tmp1);
	tmp2 = lmtan->ecent * lmtan->sin_org_lat;
	tmp3 = log ((cs_One + tmp2) / (cs_One - tmp2));

	lmtan->L0 = log (tmp1) - (lmtan->e_ovr_2 * tmp3);
	lmtan->c = lmtan->R0 * exp (lmtan->sin_org_lat * lmtan->L0);
	lmtan->abs_c = fabs (lmtan->c);

	/* Compute the mathematical limits of this projection. */

	lmtan->one_mm = csprm->csdef.scale * 0.001;
	lmtan->max_Gamma = fabs (lmtan->sin_org_lat) * cs_Pi;
	lmtan->max_R = lmtan->c * pow (tan (cs_NPTest),lmtan->sin_org_lat);

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = lmtan->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate a useful range. */

		csprm->min_ll [LNG] = cs_Km45;
		csprm->max_ll [LNG] = cs_K45;
		csprm->min_ll [LAT] = lmtan->org_lat * cs_Radian - cs_K15;
		if (csprm->min_ll [LAT] < cs_MinLatFz) csprm->min_ll [LAT] = cs_MinLatFz;
		csprm->max_ll [LAT] = lmtan->org_lat * cs_Radian + cs_K15;
		if (csprm->min_ll [LAT] > cs_MaxLatFz) csprm->min_ll [LAT] = cs_MaxLatFz;
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
		   The setup is virtually complete, so we can use CStrmerF
		   to calculate some values. */

		test_ll [LNG] = CS_adj180 (csprm->cent_mer + csprm->min_ll [LNG]);
		test_ll [LAT] = csprm->min_ll [LAT];
		CSlmtanF (lmtan,test_xy,test_ll);				/*lint !e534 */
		csprm->min_xy [YY] = test_xy [YY] - lmtan->y_off;
		if (lmtan->sin_org_lat > 0.0)
		{
			/* North pole cone. */

			csprm->min_xy [XX] = test_xy [XX] - lmtan->x_off;
			csprm->max_xy [XX] = -csprm->min_xy [XX];
		}

		test_ll [LAT] = csprm->max_ll [LAT];
		CSlmtanF (lmtan,test_xy,test_ll);				/*lint !e534 */
		csprm->max_xy [YY] = test_xy [YY] - lmtan->y_off;
		if (lmtan->sin_org_lat < 0.0)
		{
			/* South pole cone. */

			csprm->min_xy [XX] = test_xy [XX] - lmtan->x_off;
			csprm->max_xy [XX] = -csprm->min_xy [XX];
		}

		CS_quadMM (csprm->min_xy,csprm->max_xy,lmtan->x_off,
											   lmtan->y_off,
											   lmtan->quad);
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
	   addresses and we are done.  We have no information
	   on the scale or convergence of this function. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSlmtanF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSlmtanI;
	csprm->cs_scale = (cs_SCALE_CAST)CSlmtanK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSlmtanK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSlmtanH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSlmtanC;
	csprm->llchk    = (cs_LLCHK_CAST)CSlmtanL;
	csprm->xychk    = (cs_XYCHK_CAST)CSlmtanX;

	return;
}

/**********************************************************************
**	rtn_val = CSlmtanF (lmtan,xy,ll);
**
**	struct cs_Lmtan_ *lmtan;	structure which defines the parameters
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
**	This function calculates the Lambert Tangential
**	projection and returns values in the coordinate system
**	described in the cs_Lmtan_ structure.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**	This projection is a tangential cone, with a scale reduction.
**	Thus, we probably should be able to get the same results
**	use the Lambert Conformal Conic with a single parallel, if
**	the scale reduction of this projection is one.  We use
**	the code here so as to reproduce, exactly, the algorithm
**	used by the source, the National Geogrpahic Institute of
**	France.
**********************************************************************/

int EXP_LVL9 CSlmtanF (Const struct cs_Lmtan_ *lmtan,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/* 2 PI */
	extern double cs_Pi_o_2;			/* PI / 2.0  */
	extern double cs_Pi_o_4;			/* PI / 4.0  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   in radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds short
										   of the south pole, in
										   in radians. */

	int rtn_val;

	double lng;			/* The given longitude, after conversion
						   to radians. */
	double lat;			/* The given latitude after conversion
						   to radians. */
	double del_lng;
	double L0;
	double Gamma;
	double R;
	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	lng = cs_Degree * ll [LNG];
	lat = cs_Degree * ll [LAT];

	/* Compute the polar angle, Gamma, also known as the
	   convergence angle. */

	del_lng = lng - lmtan->org_lng;
	if      (del_lng > cs_Pi  && lmtan->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && lmtan->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}
	Gamma = lmtan->sin_org_lat * del_lng;

	/* Now, we compute the radius of the polar arc. R will be
	   negative (lmtan->c is negative) if the south pole is
	   the focus pole of the coordinate system. */

	if (fabs (lat) > cs_NPTest)
	{
		if ((lat > 0.0) == (lmtan->sin_org_lat > 0.0))		/*lint !e731 !e777 */
		{
			rtn_val = cs_CNVRT_INDF;
		}
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}
	if (lat > cs_NPTest)
	{
		R = (lmtan->sin_org_lat > 0.0) ? cs_Zero : lmtan->max_R;
	}
	else if (lat < cs_SPTest)
	{
		R = (lmtan->sin_org_lat < 0.0) ? cs_Zero : lmtan->max_R;
	}
	else
	{
		tmp1 = lmtan->ecent * sin (lat);
		tmp1 = (cs_One + tmp1) / (cs_One - tmp1);
		tmp1 = lmtan->e_ovr_2 * log (tmp1);
		tmp2 = cs_Pi_o_4 + (lat * cs_Half);

		/* Tmp2 can be zero (and thus tan(tmp2) can be zero)
		   at the south pole, but we've already dealt with
		   that. */

		tmp2 = log (tan (tmp2));
		L0 = tmp2 - tmp1;
		R = lmtan->c * exp (-lmtan->sin_org_lat * L0);
	}

	/* Convert to cartesian coordinates. Note, ys has the false
	   northing and the radius of the origin latitude built into
	   it by the setup function. */

	xy [XX] = R * sin (Gamma);
	xy [YY] = lmtan->R0 - (R * cos (Gamma));

	if (lmtan->quad == 0)
	{
		xy [XX] += lmtan->x_off;
		xy [YY] += lmtan->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],lmtan->x_off,lmtan->y_off,
												  lmtan->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSlmtanI (lmtan,ll,xy);
**
**	struct cs_Lmtan_ *lmtan;	structure which defines the parameters
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
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSlmtanI (Const struct cs_Lmtan_ *lmtan,double ll [2],Const double xy [2])
{
	extern double cs_Pi_o_2;			/* PI / 2.0  */
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */

	int i_cnt;
	int rtn_val;

	double x;
	double y;
	double R;
	double L;
	double exp_L;
	double Gamma;
	double del_lng;
	double new_lat;
	double last_lat;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	if (lmtan->quad == 0)
	{
		x = xy [XX] - lmtan->x_off;
		y = xy [YY] - lmtan->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,lmtan->x_off,lmtan->y_off,lmtan->quad);
	}
	y -= lmtan->R0;

	/* Set R equal to the length of the line from the
	   origin to the point being converted.  Gamma is
	   the angle portion of the polar coordinate system. */

	R = sqrt (x * x + y * y);
	if (R > lmtan->max_R)
	{
		/* R is too large, the coordinates provided must be
		   really bogus. */

		rtn_val = cs_CNVRT_RNG;
		R = lmtan->max_R;
	}

	/* If R is not zero, then at least one of x and y is not zero, and
	   the atan2 function should be happy. */

	if (R > lmtan->one_mm)
	{
		Gamma = atan2 (x,-y);
	}
	else
	{
		/* If R is zero, we are at the focus pole, and any
		   gamma, like zero, will do nicely since longitude is
		   (mathematically) undefined at this point. */

		Gamma = cs_Zero;
		rtn_val = cs_CNVRT_INDF;
	}
	if (fabs (Gamma) > lmtan->max_Gamma)
	{
		/* Coordinate is in the pie slice opposite the
		   central meridian. */

		rtn_val = cs_CNVRT_RNG;
		Gamma = (Gamma > 0.0) ? lmtan->max_Gamma : -lmtan->max_Gamma;
	}

	/* Compute the longitude. sin_org_lat should never be zero as a
	   zero origin latitude is not permitted by CSlmtanQ. If the
	   coordinate system has a south pole focus, sin_org_lat
	   will be negative, thereby effecting the necessary reversal
	   on the sense of gamma. */

	del_lng = Gamma / lmtan->sin_org_lat;

	/* The latitude is a bit more difficult.  We first compute
	   the isometric latitude.  The use of abs_c here precludes
	   taking the log of a negative number.  We've already dealt
	   with the case of R being zero; c and sin_org_lat are
	   never zero. */

	L = -(log (R / lmtan->abs_c) / lmtan->sin_org_lat);
	exp_L = exp (L);
	new_lat = cs_Two * atan (exp_L) - cs_Pi_o_2;

	/* We must now iterate to convert the isometric latitude to
	   the geographic latitude that we desire.  We could use
	   our CSchiIcal function to calculate the latitude without
	   iteration, but we choose to iterate to duplicate the
	   algorithm published by the French NGI which is the primary
	   user of this projection.  If the iteration fails to
	   converge, we assume that we are out of range.  Since we
	   limited latitude to reasonable values above, I don't think
	   this can happen. */

	i_cnt = -12;
	do
	{
		last_lat = new_lat;
		tmp1 = lmtan->ecent * sin (last_lat);
		tmp1 = (cs_One + tmp1) / (cs_One - tmp1);
		tmp1 = pow (tmp1,lmtan->e_ovr_2);
		tmp1 *= exp_L;
		new_lat = cs_Two * atan (tmp1) - cs_Pi_o_2;
		if (i_cnt++ > 0)
		{
		    	rtn_val = cs_CNVRT_RNG;
		    	break;
		}
	} while (fabs (new_lat - last_lat) > 1.0E-11);

	ll [LNG] = (del_lng + lmtan->org_lng) * cs_Radian;
	ll [LAT] = new_lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSlmtanC (lmtan,ll);
**
**	struct cs_Lmtan_ *lmtan;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert Tangential
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
**	latitude (i.e. delta X).  See Snyder/Bugayevskiy, page 16.
**********************************************************************/

double EXP_LVL9 CSlmtanC (Const struct cs_Lmtan_ *lmtan,Const double ll [2])
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
	   the mathemagics of the specific projection involved. */
		   
	my_ll [LAT] -= 0.00005;
	status = CSlmtanF (lmtan,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSlmtanF (lmtan,xy2,my_ll);
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
**	kk = CSlmtanK (lmtan,ll);
**
**	struct cs_Lmtan_ *lmtan;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert Tangential
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
**	and return the ration of geodetic and grid distance.
**********************************************************************/

double EXP_LVL9 CSlmtanK (Const struct cs_Lmtan_ *lmtan,Const double ll [2])
{
	extern double cs_SclInf;		/* 9.9E+04, the value we return
									   for an infinite scale
									   factor. */
	extern double cs_HlfSecDeg;		/* One half arc second in
									   degrees. */

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

	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] - cs_HlfSecDeg;
	CSlmtanF (lmtan,xy1,ll1);					/*lint !e534 */

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + cs_HlfSecDeg;
	CSlmtanF (lmtan,xy2,ll2);					/*lint !e534 */

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	CS_llazdd (lmtan->kerad,lmtan->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point.  I don't think
	   ll_dd can be zero since we know that ll1 id not the
	   same as ll0.  However, just to be safe: */

	if (fabs (ll_dd) > lmtan->one_mm)
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
**	hh = CSlmtanH (lmtan,ll);
**
**	struct cs_Lmtan_ *lmtan;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert Tangential
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

double EXP_LVL9 CSlmtanH (Const struct cs_Lmtan_ *lmtan,Const double ll [2])
{
	extern double cs_HlfSecDeg;		/* One half second of arc in
									   degrees. */
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
	status = CSlmtanF (lmtan,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + cs_HlfSecDeg;
	status = CSlmtanF (lmtan,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	CS_llazdd (lmtan->kerad,lmtan->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the meridian
	   scale factor at the indicated point. ll_dd should never
	   be zero, but just in case. */

	if (ll_dd > lmtan->one_mm)
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
**	status = CSlmtanL (lmtan,cnt,pnts);
**
**	struct cs_Lmtan_ *lmtan;	the coordinate system against which the check is
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

int EXP_LVL9 CSlmtanL (Const struct cs_Lmtan_ *lmtan,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi;			/*  PI */
	extern double cs_NPTest;		/* .001 seconds of arc short of
									   the north pole, in radians. */

	int ii;
	int status;
	int sgn_lng1, sgn_lng2;

	double tmp;
	double del_lng1, del_lng2;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		tmp = pnts [ii][LAT] * cs_Degree;
		if (fabs (tmp) > cs_NPTest)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	if (cnt <= 1 || status != cs_CNVRT_OK)
	{
		return (status);
	}

	/* Now, need to see if any segment cross the back side of the
	   central meridian. */

	tmp = pnts [0][LNG] * cs_Degree;
	del_lng1 = CS_adj2pi (tmp - lmtan->org_lng);
	sgn_lng1 = (del_lng1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Does the line cross the back azimuth of the
		   central meridian??? */

		tmp = pnts [ii][LNG] * cs_Degree;
		del_lng2 = CS_adj2pi (tmp - lmtan->org_lng);
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
**	status = CSlmtanX (lmtan,cnt,pnts);
**
**	struct cs_Lmtan_ *lmtan;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSlmtanL to check lat/long
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

static int CSlmtanXP (	Const struct cs_Lmtan_ *lmtan,Const double xy [2],double *gamma)
{
	double my_xx, my_yy;
	double R;

	/* Convert to polar coordinates. */

	if (lmtan->quad == 0)
	{
		my_xx = xy [XX] - lmtan->x_off;
		my_yy = xy [YY] - lmtan->y_off;
	}
	else
	{
		CS_quadI (&my_xx,&my_yy,xy,lmtan->x_off,lmtan->y_off,lmtan->quad);
	}
	my_yy -= lmtan->R0;
	R = sqrt (my_xx * my_xx + my_yy * my_yy);
	if (R < lmtan->one_mm || R > lmtan->max_R)
	{
		return (cs_CNVRT_DOMN);
	}
	*gamma = atan2 (my_xx,-my_yy);
	if (fabs (*gamma) > lmtan->max_Gamma)
	{
		return (cs_CNVRT_DOMN);
	}
	return (cs_CNVRT_OK);
}

int EXP_LVL9 CSlmtanX (Const struct cs_Lmtan_ *lmtan,int cnt,Const double pnts [][3])
{
	extern double cs_Pi;			/* 3.14159 */

	int ii;
	int status;
	int sgn_gamma1;
	int sgn_gamma2;

	double tmp;
	double gamma1, gamma2;

	status = CSlmtanXP (lmtan,pnts [0],&gamma1);
	if (status != cs_CNVRT_OK) return (status);
	sgn_gamma1 = (gamma1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Check the current point. */

		status = CSlmtanXP (lmtan,pnts [ii],&gamma2);
		if (status != cs_CNVRT_OK) break;

		sgn_gamma2 = (gamma2 < 0.0);
		if (sgn_gamma1 != sgn_gamma2)
		{
			/* Segment crosses the central meridian
			   or the back azimuth thereof. Which is
			   it? */

			tmp = fabs (gamma1) + fabs (gamma2);
			if (tmp > cs_Pi)
			{
				/* Opps!!! Crosses the back azimuth. */

				status = cs_CNVRT_DOMN;
				break;
			}
		}

		/* Set up for the next segment. */

		gamma1 = gamma2;
		sgn_gamma1 = sgn_gamma2;
	}
	return (status);
}
