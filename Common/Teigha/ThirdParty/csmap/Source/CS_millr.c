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
**	err_cnt = CSmillrQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSmillrQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Miller specific stuff. */
	
	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* That's it for Miller. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSmillrS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSmillrS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/*   57.29...  */
	extern double cs_Degree;			/*    1.0 / RADIAN  */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */

	struct cs_Millr_ *millr;

	millr = &csprm->proj_prms.millr;

	/* Transfer the necessary arguments to the
	   "millr" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	millr->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
	millr->k = csprm->csdef.scale;
	millr->x_off = csprm->csdef.x_off;
	millr->y_off = csprm->csdef.y_off;
	millr->ka = csprm->datum.e_rad * millr->k;
	millr->ka_16 = millr->ka * (1.0 / 1.6);
	millr->ka_8 = 0.8 / millr->ka;

	millr->max_xx = millr->ka * cs_3Pi_o_2;
	millr->max_yy = millr->ka_16 * 3.685460063;
	millr->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = millr->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km180 + cs_Km90;
		csprm->max_ll [LNG] = cs_K180 + cs_K90;
		csprm->min_ll [LAT] = cs_Km90;
		csprm->max_ll [LAT] = cs_K90;
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
		   The setup is virtually complete, so we can use CSgnricF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		csprm->min_xy [XX] = -millr->max_xx;
		csprm->min_xy [YY] =  millr->max_xx;
		csprm->max_xy [XX] = -millr->max_yy;
		csprm->max_xy [YY] =  millr->max_yy;

		CS_quadMM (csprm->min_xy,csprm->max_xy,millr->x_off,
											   millr->y_off,
											   millr->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSmillrF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSmillrI;
	csprm->cs_scale = (cs_SCALE_CAST)CSmillrK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSmillrK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSmillrH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSmillrC;
	csprm->llchk    = (cs_LLCHK_CAST)CSmillrL;
	csprm->xychk    = (cs_XYCHK_CAST)CSmillrX;

	return;
}

/**********************************************************************
**	rtn_val = CSmillrF (millr,xy,ll);
**
**	struct cs_Millr_ *millr;	structure which defines the parameters
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
**	This function calculates the Miller Cylindrical
**	projection and returns values in the coordinate system
**	described in the cs_Millr_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 86-89.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSmillrF (Const struct cs_Millr_ *millr,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/* Pi, i.e. 3.14159...  */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Two_pi;			/* 2 Pi */

	int rtn_val;

	double lng;			/* The given longitude, after conversion
						   to radians. */
	double lat;			/* The given latitude after conversion
						   to radians. */
	double sin_lat;
	double del_lng;
	double tmp;

	rtn_val = cs_CNVRT_NRML;

	/* Convert the latitude and longitude to radians. */

	lat = cs_Degree * ll [LAT];
	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}

	lng = cs_Degree * ll [LNG];
	del_lng = lng - millr->cent_lng;
	if      (del_lng >  cs_Pi) del_lng -= cs_Two_pi;
	else if (del_lng < -cs_Pi) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	/* Compute the X and Y.  There is no ellipsoidal version of
	   this projection.  Note, sin_lat is actually sine of
	   .8 time lat, so sin_lat should never be one. */

	xy [XX] = millr->ka * del_lng;

	sin_lat = sin (lat * 0.8);
	tmp = (cs_One + sin_lat) / (cs_One - sin_lat);

	/* One might think that log (tmp) will blow up at the
	   poles,  It doen't, because sin_lat is actually sine
	   of 0.8 * latitude.  Thus, this sine never reaches
	   1.0 or -1.0. */

	xy [YY] = millr->ka_16 * log (tmp);

	if (millr->quad == 0)
	{
		xy [XX] += millr->x_off;
		xy [YY] += millr->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],millr->x_off,millr->y_off,
												  millr->quad);
	}
	/* That's it. */

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSmillrI (millr,ll,xy);
**
**	struct cs_Millr_ *millr;	structure which defines the parameters
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
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 86-89.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSmillrI (Const struct cs_Millr_ *millr,double ll [2],Const double xy [2])
{
	extern double cs_Radian;		/* 57.29577..... */

	int rtn_val;

	double x;
	double y;
	double lat;
	double del_lng;
	double tmp;

	rtn_val = cs_CNVRT_NRML;

	/* There is no ellipsoidal version of this projection.
	   Therefore, this is relatively simple. */

	if (millr->quad == 0)
	{
		x = xy [XX] - millr->x_off;
		y = xy [YY] - millr->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,millr->x_off,millr->y_off,millr->quad);
	}

	/* Adjust for bogus coordinates. */

	if (fabs (x) > millr->max_xx)
	{
		rtn_val = cs_CNVRT_RNG;
		x = (x >= 0.0) ? millr->max_xx : -millr->max_xx;
	}

	if (fabs (y) > millr->max_yy)
	{
		rtn_val = cs_CNVRT_RNG;
		y = (y >= 0.0) ? millr->max_yy : -millr->max_yy;
	}

	/* We normalized x above, so we shouldn't have to normalize
	   longitude here. */

	del_lng = x / millr->ka;

	/* Now for the latitude.  We adjust for a very large y above,
	   so we shouldn't need to do it again. */

	tmp = exp (millr->ka_8 * y);
	lat = (2.5 * atan (tmp)) - (PI * 5.0 / 8.0);

	ll [LNG] = (del_lng + millr->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	alpha = CSmillrK (millr,ll);
**
**	struct cs_Millr_ *millr;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Miller Cylindrical
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the convergence angle of
**								the coordinate system is to be computed.
**	double alpha;				returns the convergence angle of the projected
**								coordinate system at the given point.
**
**	The function returns the convergence angle of the projection at
**	the point specified.
**********************************************************************/

double EXP_LVL9 CSmillrC (Const struct cs_Millr_ *millr,Const double ll [2])

{
	extern double cs_Zero;			/* 0.0 */

	/* And to think I got paid for this. */

	return (cs_Zero);
}							/*lint !e715 */

/**********************************************************************
**	k = CSmillrK (millr,ll);
**
**	struct cs_Millr_ *millr;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Miller Cylindrical
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor along a parallel
**								of the projected coordinate system at the given
**								point.
**
**	The function returns the grid scale factor, along a parallel, of
**	the projection at the point specified.
**
**	The cs_Millr_ structure must be setup properly, CSmillrS can do
**	this for you.
**********************************************************************/

double EXP_LVL9 CSmillrK (Const struct cs_Millr_ *millr,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */
	double lat;
	double k;

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) < cs_NPTest)
	{
		k = cs_One / cos (lat);
	}
	else
	{
		k = cs_SclInf;
	}
	return (k);
}						/*lint !e715 */

/**********************************************************************
**	k = CSmillrH (millr,ll);
**
**	struct cs_Millr_ *millr;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Miller Cylindrical
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor along a meridian
**								of the projected coordinate system at the given
**								point.
**
**	The function returns the grid scale factor, along a meridian, of
**	the projection at the point specified.
**
**	The cs_Millr_ structure must be setup properly, CSmillrS can do
**	this for you.
**********************************************************************/

double EXP_LVL9 CSmillrH (Const struct cs_Millr_ *millr,Const double ll [2])
{
	extern double cs_Degree;			/*  1.0 / RADIAN  */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */

	double lat;
	double k;

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}

	/* Having eliminated bogus latitudes, the following doesn't
	   cause a divide by zero due to the .8 */

	k = cs_One / cos (lat * 0.8);

	return (k);
}						/*lint !e715 */

/**********************************************************************
**	status = CSmillrL (millr,cnt,pnts);
**
**	struct cs_Millr_ *millr;	the coordinate system against which the check is
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

int EXP_LVL9 CSmillrL (Const struct cs_Millr_ *millr,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSmillrF (millr,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSmillrX (millr,cnt,pnts);
**
**	struct cs_Millr_ *millr;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSmillrL to check lat/long
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

int EXP_LVL9 CSmillrX (Const struct cs_Millr_ *millr,int cnt,Const double pnts [][3])
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
		status = CSmillrI (millr,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
