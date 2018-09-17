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

/*
	The following tables are defined here in the setup module
	under the assumption that any executable which uses any of
	the other functions which reference these tables will also
	include this setup function.
*/

#define MAX_IP 17
#define IP_MAX 17			/* result of mergeing files. */

double cs_RobinPR [21] =
{
	 0.0000,	/* Dummy so indices match FORTRAN source document. */ 
	-0.0620,
	 0.0000,
	 0.0620,
	 0.1240,
	 0.1860, 
	 0.2480,
	 0.3100,
	 0.3720,
	 0.4340,
	 0.4958,
	 0.5571,
	 0.6176,
	 0.6769,
	 0.7346,
	 0.7903,
	 0.8435,
	 0.8936,
	 0.9394,
	 0.9761,
	 1.0000
};

double cs_RobinXLR [21] =
{
	 0.0000,	/* Dummy so indices match FORTRAN source document. */ 
	 0.9986,
	 1.0000,
	 0.9986,
	 0.9954,
	 0.9900,
	 0.9822,
	 0.9730,
	 0.9600,
	 0.9427,
	 0.9216,
	 0.8962,
	 0.8679,
	 0.8350,
	 0.7986,
	 0.7597,
	 0.7186,
	 0.6732,
	 0.6213,
	 0.5722,
	 0.5322
};

/**********************************************************************
**	err_cnt = CSrobinQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSrobinQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Robinson specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* That's it for Robinson. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSrobinS (cs_prm);
**
**	struct cs_Csprm_*cs_prm;	struture containing the coordinate system definition,
**								datum definition, and ellipsoid definition.  Results
**								are returned in the robin member of the prj_prms
**								union.
**********************************************************************/

void EXP_LVL9 CSrobinS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.23..... */
	extern double cs_Radian;			/* 57.23..... */
	extern double cs_One;				/* 1.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_3Pi_o_2;			/* 3 Pi / 2 */

	struct cs_Robin_ *robin;

	robin = &csprm->proj_prms.robin;

	/* Transfer the necessary arguments to the cs_Robin_ structure.
	   Notice, we do longitudes in radians.  Latitudes, however, are
	   done in degrees.  Rather strange projection. */

	robin->org_lng = csprm->csdef.org_lng * cs_Degree;
	robin->k = csprm->csdef.scale;
	robin->e_rad = csprm->datum.e_rad;	/* assume sphere radius =
										   equatorial radius if an
										   ellipsoid was given. */
	robin->ka = robin->e_rad * robin->k;
	robin->kax = robin->ka * 0.8487;
	robin->kay = robin->ka * 1.3523;
	robin->inv_kax = cs_One / robin->kax;
	robin->inv_kay = cs_One / robin->kay;
	robin->x_off = csprm->csdef.x_off;
	robin->y_off = csprm->csdef.y_off;
	robin->one_cm = csprm->csdef.unit_scl * 0.01;
	robin->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Check and see if this is a test case and adjust appropriately.
	   Throbin code is not necessary except for the Test cases from
	   Synder where the radius is 1.0 */

	if (robin->e_rad <= cs_One) robin->one_cm = 2.0E-09;

	/* Calculate the xx_max and yy_max values.  These are the extents
	   of the world, in cartesian form.  These values are used to
	   prevent the inverse from calculating with bogus numbers. */

	robin->yy_max = robin->kay;
	robin->xx_max = robin->kax * cs_3Pi_o_2;

	/* Setup the range checking.  Note, these values are conqidered
	   to be useful range values. */

	csprm->cent_mer = robin->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_min [LAT] == 0.0)
	{
		/* Here if the definition does not specify; we
		   calculate some reasonable values.  Note, the
		   longitude values are relative to the cent_mer
		   value stored above. */

		csprm->min_ll [LNG] = cs_Km180;
		csprm->max_ll [LNG] = cs_K180;
		csprm->min_ll [LAT] = cs_Km90;
		csprm->max_ll [LAT] = cs_K90;
	}
	else
	{
		/* Use whatever the user provides, without checking.
		   The user specifies values in absolute terms. */

		csprm->min_ll [LNG] = csprm->csdef.ll_min [LNG] - csprm->cent_mer;
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = csprm->csdef.ll_max [LNG] - csprm->cent_mer;
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the X's and Y's */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
		csprm->csdef.xy_max [YY] == 0.0)
	{
		/* Here if the definition does not specify; we have
		   already calculated some reasonable values. */

		csprm->min_xy [XX] = -robin->xx_max;
		csprm->min_xy [YY] = -robin->yy_max;
		csprm->max_xy [XX] =  robin->xx_max;
		csprm->max_xy [YY] =  robin->yy_max;

		CS_quadMM (csprm->min_xy,csprm->max_xy,robin->x_off,
											   robin->y_off,
											   robin->quad);
	}
	else
	{
		/* Use whatever the user provides, without checking. */

		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}

	/* That's all the calculations.  Stuff some function
	   addresses and we are done. */

	csprm->ll2cs    = (cs_CS2LL_CAST)CSrobinF;
	csprm->cs2ll    = (cs_LL2CS_CAST)CSrobinI;
	csprm->cs_scale = (cs_SCALE_CAST)CSrobinK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSrobinK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSrobinH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSrobinC;
	csprm->llchk    = (cs_LLCHK_CAST)CSrobinL;
	csprm->xychk    = (cs_XYCHK_CAST)CSrobinX;

	return;
}

/**********************************************************************
**	rtn_val = CSrobinF (robin,xy,ll)
**
**	struct cs_Robin_ *robin;	structure which defines the parameters
**								in effect for the transformation.
**	double xy [2];				the results are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, to be converted.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**********************************************************************/

int EXP_LVL9 CSrobinF ( Const struct cs_Robin_ *robin,double xy [2],Const double ll [2])
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Fifth;				/* 1.0 / 5.0 */
	extern double cs_Half;				/* 1.0 / 2.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Degree;			/* 1.0 / 57.23.... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_K180;				/* 180.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_MaxLatFz;			/*  89.9999 */

	int rtn_val;

	/* The following integers are used as indices into the coefficient
	   tables.  ip is a zero based index used in the calculations of
	   the remaining indices.  ip1, ip2, and ip3 are 1 based indices
	   to remain cosistent with the source material which was written
	   for FORTRAN. */
	
	int ip;
	int ip1;
	int ip2;
	int ip3;

	double del_lng;		/* Delta Longitude, in radians. */
	double lat;			/* Provided latitude, in degrees, after
						   being limited to an appropriate
						   range. */

	double p2;			/* Offset into the current interval. */
	double p2_sq;
	double yy;

	double term1;
	double term2;
	double term3;

	rtn_val = cs_CNVRT_NRML;

	/* Verify that latitude is in range.  If not, we force it to
	   the specific range we accept.  While the calculations below
	   will operate with a wide variety of numbers, really absurd
	   numbers will cause problems. */

	lat = ll [LAT];
	if (fabs (lat) > cs_MaxLatFz)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_K90)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = fmod (lat,cs_K180);
			if (lat < cs_Km90) lat += cs_K180;
			if (lat > cs_K90)  lat -= cs_K180;
		}
	}

	/* Adjust longitude to be +- relative to the origin longitude.
	   Notice, longitudes units are radians, latitude units are
	   degrees.  Strange, but true. */

	del_lng = ll [LNG] * cs_Degree - robin->org_lng;
	if      (del_lng >  cs_3Pi_o_2 && robin->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < -cs_3Pi_o_2 && robin->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}

	/* Compute the indices into the coefficient tables. */

	p2 = fabs (lat) * cs_Fifth;
	ip = (int)p2;
	if (ip > MAX_IP) ip = MAX_IP;

	p2 -= (double)ip;
	p2_sq = p2 * p2;

	ip1 = ip + 1;
	ip2 = ip1 + 1;
	ip3 = ip2 + 1;

	term1 = cs_RobinXLR [ip2];
	term2 = cs_Half * p2    * (cs_RobinXLR [ip3] - cs_RobinXLR [ip1]);
	term3 = cs_Half * p2_sq * (cs_RobinXLR [ip3] - cs_RobinXLR [ip2] * cs_Two + cs_RobinXLR [ip1]);
	xy [XX] = robin->kax * del_lng * (term1 + term2 + term3);

	term1 = cs_RobinPR [ip2];
	term2 = cs_Half * p2    * (cs_RobinPR [ip3] - cs_RobinPR [ip1]);
	term3 = cs_Half * p2_sq * (cs_RobinPR [ip3] - cs_RobinPR [ip2] * cs_Two + cs_RobinPR [ip1]);
	yy = robin->kay * (term1 + term2 + term3);

	if      (lat > 0.0) xy [YY] = yy;
	else if (lat < 0.0) xy [YY] = -yy;
	else		    xy [YY] = cs_Zero;

	if (robin->quad == 0)
	{
		xy [XX] += robin->x_off;
		xy [YY] += robin->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],robin->x_off,robin->y_off,robin->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSrobinI (robin,ll,xy);
**
**	struct cs_Robin_ *robin;	structure defining the natureof the coordinate
**								system as setup by CSrobinS.
**	double ll [2];				lat/long results returned here; longitude is
**								element zero, latitude in element 1.  Values
**								are degrees east of greenwich and north of
**								equator.
**	double xy [2];				the cartesian X & Y to be converted.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and xy arguments may point to the same array.
**
**	There are about 10 line of common code with CSrobinF here.
**	Should, perhaps, be merged into a single module.
**********************************************************************/

#if defined (_MSC_VER) && _MSC_VER >= 900
/* There's a bug in that there optimizier. */
#pragma optimize ("g",off)
#endif

int EXP_LVL9 CSrobinI (	Const struct cs_Robin_ *robin,double ll [2],Const double xy [2])
{
	extern double cs_Fifth;				/*  1.0 / 5.0 */
	extern double cs_Half;				/*  0.5 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Two;				/*  2.0 */
	extern double cs_Five;				/*  5.0 */
	extern double cs_Radian;			/*  57.23..... */
	extern double cs_3Pi_o_2;			/*  3 Pi over 2 (6.28...) */
	extern double cs_MaxLatFz;			/*  89.9999 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_K180;				/*  180.0 */

	int ip;
	int nn;
	int ip1;
	int ip2;
	int ip3;
	int rtn_val;

	double xx;
	double yy;
	double yy1;
	double del_lng;

	double cc;
	double tt;
	double pd;
	double p2;
	double p2_sq;
	double term1;
	double term2;
	double term3;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false origin. */

	if (robin->quad == 0)
	{
		xx = xy [XX] - robin->x_off;
		yy = xy [YY] - robin->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,robin->x_off,robin->y_off,robin->quad);
	}

	/* Force fit the X and Y's to be within range. The calculations
	   which follow are very tolerant, but absurd values may cause
	   problems. */

	if (fabs (xx) > robin->xx_max)
	{
		rtn_val = cs_CNVRT_RNG;
		xx = (xx >= 0.0) ? robin->xx_max : -robin->xx_max;
	}
	if (fabs (yy) > robin->yy_max)
	{
		rtn_val = cs_CNVRT_RNG;
		yy = (yy >= 0.0) ? robin->yy_max : -robin->yy_max;
	}

	/* Compute pd, a real rough guess at the latitude,
	   in degrees. */

	yy1 = fabs (yy) * robin->inv_kay;
	pd = yy1 * cs_K90;

	/* Use this rough guess to make a rough guess at the
	   interval in the table we will be using. */

	p2 = fabs (pd) * cs_Fifth;
	ip = (int)p2;
	if (ip > IP_MAX) ip = IP_MAX;
	if (ip <= 0)
	{
		ip = 1;
	}

	/* The algorithm below searches for an appropriate interval
	   in the table. This algorithm works down, not up. Also,
	   the zero interval is actually non-existant; there only
	   to provide second level differencing for the 0 to 5 degree
	   interval.

	   Now, we enter a loop which refines our guess as to the
	   correct interval in the tables, and the offset (p2)
	   into that interval.  We might be on the hairy edge of
	   the zone. */

	for (;;)
	{
		ip1 = ip + 1;
		ip2 = ip1 + 1;
		ip3 = ip2 + 1;

		/* Due to the nature of the data in the fixed coefficient
		   tables, term2 will never be zero. */

		term1 = cs_RobinPR [ip2];
		term2 = cs_RobinPR [ip3] - cs_RobinPR [ip1];	 /* U */
		term3 = cs_RobinPR [ip3] - cs_Two * cs_RobinPR [ip2] + cs_RobinPR [ip1];    /* V */
		tt = cs_Two * (fabs (yy1) - term1) / term2;
		cc = term3 / term2;
		p2 = tt * (cs_One - (cc * tt * (cs_One - (cs_Two * cc * tt))));

		if (p2 > 0.0 || ip <= 1) break;
		ip -= 1;
	};

	/* ip is now a good approximation of the correct table interval,
	   and p2 is a good approximation of the offset into that interval.
	   pd now is set to our first guess at the actual latitude, in
	   degrees.  Note, p2 can be negative at this point if we are in
	   the first interval (i.e. ip = 0). */

	pd = (p2 + (double)(ip)) * cs_Five;
	if (yy < 0.0) pd = -pd;

	/* Now we iterate through the forward equation and adjust our
	   latitude to the desired precision.  As a safety precaution,
	   we use nn to break out of an unconverging loop after 25
	   iterations. Typically, four or five iterations is all that
	   is necessary.  Since Robinson is used only for full world
	   maps, the one_cm limit used here may be too small.  For
	   higher performance, raise this value (in CSrobinS). */

	nn = 0;
	for (;;)
	{
		/* Must do the following in case we crossed an interval
		   boundary; rare but possible. */

		p2 = fabs (pd) * cs_Fifth;
		ip = (int)p2;
		if (ip > IP_MAX) ip = IP_MAX;
		p2 = p2 - (double)ip;		/* offset into interval */
		p2_sq = p2 * p2;

		/* Compute the Y value indicated by our approximate latitude. */

		ip1 = ip + 1;
		ip2 = ip1 + 1;
		ip3 = ip2 + 1;

		term1 = cs_RobinPR [ip2];
		term2 = cs_Half * p2 * (cs_RobinPR [ip3] - cs_RobinPR [ip1]);
		term3 = cs_Half * p2_sq * (cs_RobinPR [ip3] - cs_Two * cs_RobinPR [ip2] + cs_RobinPR [ip1]);
		yy1 = robin->kay * (term1 + term2 + term3);
		if (yy < 0.0) yy1 = -yy1;

		/* If yy1 is within one centimeter of yy, we say that pd
		   is the desired result and break the loop. */

		if (fabs (yy1 - yy) <= robin->one_cm || nn > 35) break;

		/* We have an approximate yy.  Adjust our approximate latitude relative
		   to the difference in the true Y and our approximate Y. */

		pd -= cs_K90 * (yy1 - yy) * robin->inv_kay;
		nn += 1;
	}
	if (fabs (pd) > cs_MaxLatFz)
	{
		/* Essentially a pole. */

		if (rtn_val == cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_INDF;
		}
		if (fabs (pd) > cs_K90)
		{
			rtn_val = cs_CNVRT_RNG;
			pd = fmod (pd,cs_K180);
			if (pd < cs_Km90) pd += cs_K180;
			if (pd > cs_K90)  pd -= cs_K180;
		}
	}
	ll [LAT] = pd;

	/* pd is now the correct latitude in degrees.  Using the
	   same interval and offset values computed for the last
	   iteration of the latitude loop, and xx, compute the
	   true longitude.  I don't believe its possible for term1,
	   term2, and term3 to sum to zero; but I haven't proved it. */

	term1 = cs_RobinXLR [ip2];
	term2 = cs_Half * p2 *    (cs_RobinXLR [ip3] - cs_RobinXLR [ip1]);
	term3 = cs_Half * p2_sq * (cs_RobinXLR [ip3] - cs_Two * cs_RobinXLR [ip2] + cs_RobinXLR [ip1]);
	del_lng = xx * robin->inv_kax / (term1 + term2 + term3);

	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}

	ll [LNG] = (robin->org_lng + del_lng) * cs_Radian;
	return (rtn_val);
}

#if defined (_MSC_VER) && _MSC_VER >= 900
/* There's a bug in that there optimizier. */
#pragma optimize ("g",on)
#endif

/**********************************************************************
**	gamma = CSrobinC (robin,ll);
**
**	struct cs_Robin_ *robin;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Robinson projection.
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
**	We have not as yet found or deduced an analytical equation for the
**	convergence angle for this projection.  We calculate it empicially.
**	The convergence angle is defined as the arctangent of the partial
**	derivative of Y with respect to latitude (read delta Y when the
**	latitude changes a skosh) divied by the partial derivative of
**	X with repsect to latitude (i.e. delta X).
**********************************************************************/

double EXP_LVL9 CSrobinC (Const struct cs_Robin_ *robin,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;

	double xy1 [2];
	double xy2 [2];
	double my_ll [3];

	my_ll [LNG] = ll [LNG];
	my_ll [LAT] = ll [LAT];

	/* Compute the cartesian coordinates of the end points of a
	   linear line segment whose end points are on the same meridian,
	   but separated by a small amount of latitude.  The degree of
	   latitude separation is rather arbitrary.  Technically, the
	   smaller the better, but if its too small, we end up with
	   problems in the trig functions.  0.00005 gives us a line of
	   about 10 meters on the surface of the earth.

	   Should be zero on the central meridian and everywhere on
	   the equator. */

	my_ll [LAT] -= 0.00005;
	status = CSrobinF (robin,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSrobinF (robin,xy2,my_ll);
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
**	kk = CSrobinK (robin,ll);
**
**	struct cs_Robin_ *robin;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Robinson projection.
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

double EXP_LVL9 CSrobinK (Const struct cs_Robin_ *robin,Const double ll [2])
{
	extern double cs_Sin1Sec;			/* sine of one arc second */
	extern double cs_Sec2Deg;			/* 1.0 / 36000.0 */
	extern double cs_Degree;			/* 1/RADIAN */
	extern double cs_MaxLatFz;			/* One second of arc less
										   than 90 in degrees. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */

	double kk;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	if (fabs (ll [LAT]) > cs_MaxLatFz)
	{
		return (cs_SclInf);
	}

	/* We compute the grid scale factor empirically. We are not
	   aware of any analytical formulas for doing same.

	   Algorithm here is very simple:

	   1) convert the lat/long to X and Y.
	   2) add a skosh to the longitude and compute a second X and Y.
	   3) calculate the distance between the two XY's.
	   4) compute the angular distance between the two lat/longs.
	   5) convert the angular distance to linear using the radius.
	   6) compare the two distances. */

	ll0 [LNG] = ll [LNG];
	ll0 [LAT] = ll [LAT];
	ll1 [LNG] = ll0 [LNG] + cs_Sec2Deg;		/* One second of arc,
											   in degrees. */
	ll1 [LAT] = ll0 [LAT];

	/* Establish two points, pretty close together. */

	CSrobinF (robin,xy1,ll0);				/*lint !e534 */
	CSrobinF (robin,xy2,ll1);				/*lint !e534 */

	/* Get the grid distance. */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the angular distance.  We know that delta longitude
	   is one arc second, and delta latitude is zero.  We also
	   know that the sine of very small angles, like one 
	   arc second, is the same as the angle.  Working
	   these facts into spherical trigonometry yields the
	   following simple formula for the geographic distance
	   on a sphere for an arc second.  cs_Sin1Sec is the sine
	   of one arc second, or one arc second. See Synder, page 30. */

	dd_ll = robin->ka * cs_Sin1Sec * cos (ll0 [1] * cs_Degree);

	/* The ratio of the two distances is the grid scale factor.
	   Should be 1.0 on the 38th parallels, both north and south.
	   Will get huge at the poles, so we try to filter that out. */

	if (dd_ll > robin->one_cm) kk = dd_xy / dd_ll;
	else			   kk = cs_SclInf;

	return (kk);
}

/**********************************************************************
**	hh = CSrobinH (robin,ll);
**
**	struct cs_Robin_ *robin;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Robinson Projection.
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double hh;					returns the grid scale along a meridian of the
**								coordinate system at the specified point.
**
**	This projection is not defined, as far as we know at the
**	current time, for the ellipsoid.  We simple use the
**	equatorial radius of the supplied ellipsoid as the radius
**	of the sphere.
**
**	We do not know of any analytical formulas for the meridian
**	scale, so this value is computed empirically.
**********************************************************************/

double EXP_LVL9 CSrobinH (Const struct cs_Robin_ *robin,Const double ll [2])
{
	extern double cs_Sin1Sec;		/* sine of one arc second */
	extern double cs_HlfSecDeg;		/* 0.5 / 3600.0 */
	extern double cs_MaxLatFz;		/* 1 second of arc less than 90
									   in degrees. */
	extern double cs_SclInf;		/* 9.9E+04, the value we return
									   for infinite scale. */

	double hh;

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	if (fabs (ll [LAT]) > cs_MaxLatFz)
	{
		return (cs_SclInf);
	}

	/* We compute the grid scale factor empirically.  Algorithm here
	   is rather simple since we only support the sphere:

	   1) convert the lat/long to X and Y.
	   2) add a skosh to the longitude and compute a second X and Y.
	   3) calculate the distance between the two XY's.
	   4) compute the angular distance between the two lat/longs.
	   5) convert the angular distance to linear using the radius.
	   6) compare the two distances. */

	ll0 [LNG] = ll [LNG];
	ll0 [LAT] = ll [LAT] - cs_HlfSecDeg;
	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] + cs_HlfSecDeg;	/* One second of arc. */

	/* Establish two points, pretty close together. */

	CSrobinF (robin,xy1,ll0);				/*lint !e534 */
	CSrobinF (robin,xy2,ll1);				/*lint !e534 */

	/* Get the grid distance. */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the angular distance.  Keep in mind that delta
	   longitude is zero for this case, delta latitude is fixed
	   at one arc second, and the sine of a small angle like
	   one arc second is the same as the angle (in radians). */

	dd_ll = robin->ka * cs_Sin1Sec;

	hh = dd_xy / dd_ll;
	return (hh);
}

/**********************************************************************
**	status = CSrobinL (robin,cnt,pnts);
**
**	struct cs_Robin_ *robin;	the coordinate system against which the check is
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

int EXP_LVL9 CSrobinL (Const struct cs_Robin_ *robin,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi;			/* 3.14159....  */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int status;

	double lat;
	double lng;

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
		lng = pnts [ii][LNG] * cs_Degree - robin->org_lng;
		if (fabs (lng) > cs_Pi)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);

	/* No special test are required for lines and or regions. */
}

/**********************************************************************
**	status = CSrobinX (robin,cnt,pnts);
**
**	struct cs_Robin_ *robin;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSrobinL to check lat/long
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

int EXP_LVL9 CSrobinX (Const struct cs_Robin_ *robin,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	rtn_val = cs_CNVRT_OK;

	/* Due to the eliptical nature of the east/west boundary,
	   we use the inverse function to test each point.  If all
	   the points are in range, then the region, if any, is OK. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSrobinI (robin,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
