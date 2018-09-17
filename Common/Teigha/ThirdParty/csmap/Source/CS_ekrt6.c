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
**	err_cnt = CSekrt6Q (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	currently unused.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;				the size of the array pointed to be err_list,
**								may be zero.
**	int err_cnt;	returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greennwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CSekrt6Q (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Eckert VI specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* That's it for Eckert VI. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSekrt6S (cs_prm);
**
**	struct cs_Csprm_*cs_prm;	struture containing the coordinate system definition,
**								datum definition, and ellipsoid definition.  Results
**								are returned in the ekrt6 member of the prj_prms
**								union.
**
**********************************************************************/

void EXP_LVL9 CSekrt6S (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/*  180.0 / Pi */
	extern double cs_Degree;			/*  Pi / 180.0 */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Pi_o_2;			/*  PI / 2.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Two;				/*  2.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */

	struct cs_Ekrt6_ *ekrt6;

	double tmp;
	
	ekrt6 = &csprm->proj_prms.ekrt6;

	/* Transfer the necessary arguments to the cs_Ekrt6_ structure.
	   Notice, we don longitudes in radians.  Latitudes, however, are
	   done in degrees.  Rather strange projection. */

	ekrt6->org_lng = csprm->csdef.org_lng * cs_Degree;
	ekrt6->k = csprm->csdef.scale;
	ekrt6->e_rad = csprm->datum.e_rad;
	ekrt6->ka = ekrt6->e_rad * ekrt6->k;
	ekrt6->x_off = csprm->csdef.x_off;
	ekrt6->y_off = csprm->csdef.y_off;
	ekrt6->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
	ekrt6->one_cm = csprm->csdef.unit_scl * 0.01;
	if (ekrt6->e_rad == cs_One) ekrt6->one_cm = 2.0E-10;	/*lint !e777 */
	ekrt6->cnvrg_val = 1.0E-10;

	tmp = sqrt (cs_Two + cs_Pi);
	ekrt6->kF_theta = (cs_One + cs_Pi_o_2);
	ekrt6->kF_x = ekrt6->ka / tmp;
	ekrt6->kF_y = ekrt6->kF_x * cs_Two;
	ekrt6->kI_theta = tmp / (cs_Two * ekrt6->ka);
	ekrt6->kI_lat = cs_One + cs_Pi_o_2;
	ekrt6->kI_lng = tmp / ekrt6->ka;

	ekrt6->max_xx = cs_Pi * ekrt6->kF_x;
	ekrt6->max_yy = cs_Pi_o_2 * ekrt6->kF_y;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = ekrt6->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km180;
		csprm->max_ll [LNG] = cs_K180;
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

		csprm->min_xy [XX] = -ekrt6->max_xx;
		csprm->max_xy [XX] =  ekrt6->max_xx;
		csprm->min_xy [YY] = -ekrt6->max_yy;
		csprm->max_xy [YY] =  ekrt6->max_yy;

		CS_quadMM (csprm->min_xy,csprm->max_xy,ekrt6->x_off,
						       ekrt6->y_off,
						       ekrt6->quad);
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

	csprm->ll2cs    = (cs_CS2LL_CAST)CSekrt6F;
	csprm->cs2ll    = (cs_LL2CS_CAST)CSekrt6I;
	csprm->cs_scale = (cs_SCALE_CAST)CSekrt6K;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSekrt6K;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSekrt6H;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSekrt6C;
	csprm->llchk    = (cs_LLCHK_CAST)CSekrt6L;
	csprm->xychk    = (cs_XYCHK_CAST)CSekrt6X;

	return;
}

/**********************************************************************
**	rtn_val = CSekrt6F (ekrt6,xy,ll)
**
**	struct cs_Ekrt6_ *ekrt6;	structure which defines the parameters
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
**********************************************************************/

int EXP_LVL9 CSekrt6F ( Const struct cs_Ekrt6_ *ekrt6,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/*  1.0 / 57.23.... */
	extern double cs_Pi;				/*  Pi, i.e. 3.14159 */
	extern double cs_Two_pi;			/*  2 Pi */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_One;				/*  1.0 */

	int rtn_val;

	double lng;			/* Provided longitude in radians. */
	double del_lng;
	double lat;			/* Provided latitude in radians. */
	double sin_lat;

	double xx;
	double yy;

	double theta;		/* Parametric angle. */
	double del_theta;
	double sin_theta;
	double cos_theta;

	rtn_val = cs_CNVRT_NRML;

	/* Adjust longitude to be +- relative to the origin
	   longitude. */

	lng = cs_Degree * ll [LNG];
	del_lng = lng - ekrt6->org_lng;
	if      (del_lng >  cs_Pi) del_lng -= cs_Two_pi;
	else if (del_lng < -cs_Pi) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_NRML;
		lat = CS_adj1pi (lat);
	}
	sin_lat = sin (lat);

	theta = lat;
	for (;;)
	{
		sin_theta = sin (theta);
		cos_theta = cos (theta);

		del_theta  = -theta;
		del_theta -= sin_theta;
		del_theta += ekrt6->kF_theta * sin_lat;
		del_theta /= cs_One + cos_theta;
		if (fabs (del_theta) < ekrt6->cnvrg_val) break;
		theta += del_theta;
	}

	xx = ekrt6->kF_x * del_lng * (cs_One + cos_theta);
	yy = ekrt6->kF_y * theta;

	if (ekrt6->quad == 0)
	{
		xy[XX] = xx + ekrt6->x_off;
		xy[YY] = yy + ekrt6->y_off;
	}
	else
	{
		CS_quadF (xy,xx,yy,ekrt6->x_off,ekrt6->y_off,ekrt6->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSekrt6I (ekrt6,ll,xy);
**
**	struct cs_Ekrt6_ *ekrt6;	structure defining the natureof the coordinate
**								system as setup by CSekrt6S.
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
**	There are about 10 lines of common code with CSekrt4F here.
**	Should, perhaps, be merged into a single module.
**********************************************************************/

int EXP_LVL9 CSekrt6I (	Const struct cs_Ekrt6_ *ekrt6,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.23..... */
	extern double cs_3Pi_o_2;			/*  3 Pi over 2 */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_Mpi_o_2;			/* -Pi over 2 */
	extern double cs_One;				/*  1.0 */

	int rtn_val;

	double xx;
	double yy;
	double lat;
	double theta;
	double del_lng;
	double sin_theta;
	double cos_theta;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false origin. */

	if (ekrt6->quad == 0)
	{
		xx = xy [XX] - ekrt6->x_off;
		yy = xy [YY] - ekrt6->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,ekrt6->x_off,ekrt6->y_off,ekrt6->quad);
	}

	theta = ekrt6->kI_theta * yy;
	if (fabs (theta) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		theta = (theta >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
	}
	sin_theta = sin (theta);
	cos_theta = cos (theta);
	lat = asin ((theta + sin_theta) / ekrt6->kI_lat);

	del_lng = (ekrt6->kI_lng * xx) / (cs_One + cos_theta);
	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		del_lng = CS_adj2pi (del_lng);
		rtn_val = cs_CNVRT_RNG;
	}

	ll [LNG] = (del_lng + ekrt6->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSekrt6C (ekrt6,ll);
**
**	struct cs_Ekrt6_ *ekrt6;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Eckert Type VI
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

double EXP_LVL9 CSekrt6C (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2])
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
	   the mathemagics of the specific projection involved.

	   Should be zero on the central meridian and the equator. */

	my_ll [LAT] -= 0.00005;
	status = CSekrt6F (ekrt6,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSekrt6F (ekrt6,xy2,my_ll);
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
**kk = CSekrt6K (ekrt6,ll);
**
**	struct cs_Ekrt6_ *ekrt6;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Eckert,
**								number 6, projection.
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

double EXP_LVL9 CSekrt6K (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2])
{
	extern double cs_MoloK;				/* sine of one arc second */
	extern double cs_Degree;			/* 1/RADIAN */
	extern double cs_Mone;				/* -1.0, the value we return
										   when provided with a
										   bogus coordinate. */
	extern double cs_SclInf;			/* 9.9E+09, the value
										   returned for infinite
										   scale. */
	int status;

	double kk;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	double del_xx, del_yy;
	double dd_xy, dd_ll;

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
	ll1 [LNG] = ll [LNG] + (1.0 / 3600.0);	/* One second of arc,
											   in degrees. */
	ll1 [LAT] = ll [LAT];

	/* Establish two points, pretty close together. */

	status = CSekrt6F (ekrt6,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSekrt6F (ekrt6,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

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
	   on a sphere for an arc second.  cs_MoloK is the sine
	   of one arc second, or one arc second. See Synder, page 30. */

	dd_ll = ekrt6->ka * cs_MoloK * cos (ll0 [LAT] * cs_Degree);

	/* The ratio of the two distances is the grid scale factor.
	   Should be 1.0 on the 38th parallels, both north and south.
	   Will get huge at the poles, so we try to filter that out. */

	if (dd_ll > ekrt6->one_cm) kk = dd_xy / dd_ll;
	else			   kk = cs_SclInf;
	return (kk);
}

/**********************************************************************
**	hh = CSekrt6H (ekrt6,ll);
**
**	struct cs_Ekrt6_ *ekrt6;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Eckert, number 6,
**								Projection.
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

double EXP_LVL9 CSekrt6H (Const struct cs_Ekrt6_ *ekrt6,Const double ll [2])
{
	extern double cs_Sin1Sec;		/* sine of one arc second */
	extern double cs_Mone;			/* -1.0, the value returned when
									   provided with a bogus point. */
	extern double cs_HlfSecDeg;		/* on half second of arc, in
									   degrees */

	int status;

	double hh;

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

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
	ll1 [LAT] = ll [LAT] + cs_HlfSecDeg;

	/* Establish two points, pretty close together. */

	status = CSekrt6F (ekrt6,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSekrt6F (ekrt6,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Get the grid distance. */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the angular distance.  Keep in mind that delta
	   longitude is zero for this case, delta latitude is fixed
	   at one arc second, and the sine of a small angle like
	   one arc second is the same as the angle (in radians). */

	dd_ll = ekrt6->ka * cs_Sin1Sec;

	hh = dd_xy / dd_ll;
	return (hh);
}

/**********************************************************************
**	status = CSekrt6L (ekrt6,cnt,pnts);
**
**	struct cs_Ekrt6_ *ekrt6;	the coordinate system against which the check is
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

int EXP_LVL9 CSekrt6L (Const struct cs_Ekrt6_ *ekrt6,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Two_pi;		/* 2 Pi, 6.28..... */
	extern double cs_3Pi_o_2;		/* 3 Pi over 2 */
	extern double cs_Pi_o_2;		/* Pi over 2 */

	int ii;
	int rtn_val;

	double lat;
	double del_lng;

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
		del_lng = (pnts [ii][LNG] * cs_Degree) - ekrt6->org_lng;
		if      (del_lng >  cs_3Pi_o_2 && ekrt6->org_lng < 0.0) del_lng -= cs_Two_pi;
		else if (del_lng < -cs_3Pi_o_2 && ekrt6->org_lng > 0.0) del_lng += cs_Two_pi;
		if (fabs (del_lng) >= cs_3Pi_o_2)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSekrt6X (ekrt6,cnt,pnts);
**
**	struct cs_Ekrt6_ *ekrt6;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSekrt6L to check lat/long
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

int EXP_LVL9 CSekrt6X (Const struct cs_Ekrt6_ *ekrt6,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.
	   No special checks are required for lines and/or regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSekrt6I (ekrt6,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
