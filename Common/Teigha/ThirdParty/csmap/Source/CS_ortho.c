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
**	err_cnt = CSorthoQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSorthoQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */

	int err_cnt;
	
	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Orthographic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	/* That's it for the Orthographic projection. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSorthoS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	We handle two variations of this projection.  In the
**	first variation, the orientation of the positive Y axis
**	is defined by a latitude and longitude of a point on
**	the positive Y axis.  In the second form, the azimuth
**	of the Y axis is defined by an actual angle, east of
**	north, of the Y axis.  The specific variation to be
**	used is indicated by the form variable.  If an invalid
**	form is provided, a coordinate system where the Y axis
**	is true north is generated.
**	
**	Normal conventions and CS-MAP standards have NOT
**	been followed in coding this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CSorthoS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/*  1.0 / RADIAN  */
	extern double cs_Radian;			/*  57.27....  */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_Mpi_o_2;			/* -Pi over 2 */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Three;				/*  3.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds short
										   of the south pole, in
										   radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc in
										   radians. */

	struct cs_Ortho_ *ortho;

	ortho = &csprm->proj_prms.ortho;

	/* Transfer the necessary arguments to the "ortho" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	ortho->org_lng = csprm->csdef.org_lng * cs_Degree;
	ortho->org_lat = csprm->csdef.org_lat * cs_Degree;
	ortho->k = csprm->csdef.scale;
	ortho->x_off = csprm->csdef.x_off;
	ortho->y_off = csprm->csdef.y_off;
	ortho->e_rad = csprm->datum.e_rad;
	ortho->ka = csprm->datum.e_rad * ortho->k;
	ortho->cos_org_lat = cos (ortho->org_lat);
	ortho->sin_org_lat = sin (ortho->org_lat);
	ortho->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Force hard values for the polar and equatorial
	   aspects.  This should not be necessary.  However,
	   different run time libraries behave differently.
	   Since this is the set up function, I think we can
	   afford the following.  The numbers chosen for the
	   indicated aspect if the origin latitude is within
	   one minute of the approriate latitude location. */

	if (ortho->org_lat > cs_NPTest)
	{
		ortho->sin_org_lat = cs_One;
		ortho->cos_org_lat = cs_Zero;
		ortho->org_lat = cs_Pi_o_2;
	}
	else if (ortho->org_lat < cs_SPTest)
	{
		ortho->sin_org_lat = cs_Mone;
		ortho->cos_org_lat = cs_Zero;
		ortho->org_lat = cs_Mpi_o_2;
	}
	else if (fabs (ortho->org_lat) < cs_AnglTest)
	{
		ortho->sin_org_lat = cs_Zero;
		ortho->cos_org_lat = cs_One;
		ortho->org_lat = cs_Zero;
	}

	/* Set up a value in the units of the cartesian system
	   which represents one tenth of a millimeter.  If this is
	   a unit sphere test system, we simply provide out own value. */

	ortho->one_mm = 0.001 * ortho->k;
	if (ortho->e_rad <= cs_Three) ortho->one_mm = 2.0E-10;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = ortho->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  The limit is really a cirlce of
		   angular distance of Pi/2 radians from the origin.
		   This is difficult to cram into a rectangular box. */

		csprm->min_ll [LNG] = cs_Km90;
		csprm->max_ll [LNG] = cs_K90;
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
		   The setup is virtually complete, so we can use CSorthoF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		csprm->min_xy [XX] = -ortho->ka;
		csprm->max_xy [XX] =  ortho->ka;
		csprm->min_xy [YY] = -ortho->ka;
		csprm->max_xy [YY] =  ortho->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,ortho->x_off,
											   ortho->y_off,
											   ortho->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSorthoF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSorthoI;
	csprm->cs_scale = (cs_SCALE_CAST)CSorthoH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSorthoK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSorthoH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSorthoC;
	csprm->llchk    = (cs_LLCHK_CAST)CSorthoL;
	csprm->xychk    = (cs_XYCHK_CAST)CSorthoX;

	return;
}

/**********************************************************************
**	rtn_val = CSorthoF (ortho,xy,ll);
**
**	struct cs_Ortho_ *ortho;	structure which defines the parameters
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
**	This function calculates the Orthographic projection
**	and returns values in the coordinate system described
**	by the cs_Ortho_ structure.  For this projection only the
**	spherical earth is supported.  We use the equatorial
**	radius of the ellipsoid provided as the radius of the
**	sphere.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 145-153.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSorthoF (Const struct cs_Ortho_ *ortho,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double cos_c;			/* cosine of the angular distance from the
							   origin to the point. */
	double del_lng;
	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;
	double cos_lat;

	double tmp;

	rtn_val = cs_CNVRT_NRML;

	/* For this projection, we only support the spherical form,
	   therefore there is only one set of equations.  These are
	   pretty simple. */

	lng = cs_Degree * ll [0];
	lat = cs_Degree * ll [1];

	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	del_lng = lng - ortho->org_lng;
	if      (del_lng > cs_Pi  && ortho->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && ortho->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* Is this point in range? */

	cos_c = (ortho->sin_org_lat * sin_lat) +
			(ortho->cos_org_lat * cos_lat * cos_del_lng);
	if (cos_c <= 0.0)
	{
		rtn_val = cs_CNVRT_RNG;
	}

	/* Certain computation efficiencies could be obtained by
	    handling the various aspects separately.  However, these
	    efficiences amount to two multiplies at most.  Therefore,
	    we would not save very much after we decide which aspect
	    to compute.

	    No divides, no atan2's, no square roots, no logs.  The
	    programmer's paradise. */

	xy [XX] = ortho->ka * cos_lat * sin_del_lng;
	tmp = (ortho->cos_org_lat * sin_lat) -
		  (ortho->sin_org_lat * cos_lat * cos_del_lng);
	xy [YY] = ortho->ka * tmp;

	if (ortho->quad == 0)
	{
		xy [XX] += ortho->x_off;
		xy [YY] += ortho->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],ortho->x_off,ortho->y_off,
												  ortho->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSorthoI (ortho,ll,xy);
**
**	struct cs_Ortho_ *ortho;	structure which defines the parameters
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
**	The ll and xy arguments of this function may point to the same
**	array with no adverse affects.
**
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 145-153.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**	For this projection, only the spherical form is supported.
**********************************************************************/

int EXP_LVL9 CSorthoI (Const struct cs_Ortho_ *ortho,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_AnglTest;			/* 0.001 arc seconds
										   in radians. */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds short
										   of the south pole, in
										   radians. */

	int rtn_val;

	double x;
	double y;
	double rho;

	double lat;
	double del_lng;
	double sin_c;
	double cos_c;

	double tmp;

	rtn_val = cs_CNVRT_NRML;

	if (ortho->quad == 0)
	{
		x = xy [XX] - ortho->x_off;
		y = xy [YY] - ortho->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,ortho->x_off,ortho->y_off,ortho->quad);
	}

	del_lng = cs_Zero;
	rho = sqrt (x * x + y * y);
	if (rho < ortho->one_mm)
	{
		/* This code eliminates many possible glitches in the
		   code below. */

		lat = ortho->org_lat;
	}
	else
	{
		/* The latitude calculation is common to all aspects.
		   c is the angular distance from the origin to the
		   point being converted.  Thus, by limiting rho
		   to ka, -pi/4 <= c >= pi/4, and cos_c is always
		   positive (possibly zero), and since we have already
		   handled the case of rho == 0, sin_c will never be
		   zero. */

		if (rho > ortho->ka)
		{
			rtn_val = cs_CNVRT_RNG;
			x *= ortho->ka / rho;
			y *= ortho->ka / rho;
			rho = ortho->ka;
		}
		sin_c = rho / ortho->ka;
		cos_c = sqrt (cs_One - (sin_c * sin_c));

		/* We handled rho == 0 above. */

		tmp = (y * sin_c * ortho->cos_org_lat) / rho;
		tmp = (cos_c * ortho->sin_org_lat) + tmp;
		lat = asin (tmp);

		/* The longitude calculation is much faster for the
		   two polar aspects.  There is no mathematical reason
		   for having three different cases.

		   Note, we force the values of sin_org_lat and
		   cos_org_lat to hard values in the setup function. */

		if (ortho->org_lat >= cs_NPTest)
		{
			/* Here for the north polar aspect:
			   cos_org_lat = 0, sin_org_lat = +1.
			   Since rho is not zero, we know that
			   either x or y is not zero. */

			del_lng = atan2 (x,-y);
		}
		else if (ortho->org_lat <= cs_SPTest)
		{
			/* Here for the south polar aspect:
			   cos_org_lat = 0, sin_org_lat = -1 */

			del_lng = atan2 (x,y);
		}
		else if (fabs (ortho->org_lat) <= cs_AnglTest)
		{
			/* Here for the equatorial aspect:
			   cos_org_lat = 1, sin_org_lat = 0.

			   Careful, there is a point at which both args
			   to atan2 here could be zero: x=0,y=ka.  If x is
			   zero, the point is on the central meridian.

			   Since this is an equatorial aspect, we need not
			   worry about polar wrap around. */

			if (fabs (x) > ortho->one_mm)
			{
				del_lng = atan2 (x * sin_c,rho * cos_c);
			}
		}
		else
		{
			/* Here for the oblique aspect. Again, if x == 0,
			   it is possible for both arguments to atan2 to
			   be zero. (org_lat=pi/4, c=pi/4). */

			if (fabs (x) > ortho->one_mm)
			{
				tmp = (rho * ortho->cos_org_lat * cos_c) -
					  (  y * ortho->sin_org_lat * sin_c);

				del_lng = atan2 (x * sin_c,tmp);
			}
		}
	}
	if (fabs (lat) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
		rtn_val = cs_CNVRT_INDF;
	}
		
	/* Convert the results to degrees. */

	ll [LNG] = (del_lng + ortho->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSorthoC (ortho,ll);
**
**	struct cs_Ortho_ *ortho;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Orthographic projection.
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

double EXP_LVL9 CSorthoC (Const struct cs_Ortho_ *ortho,Const double ll [2])
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
	status = CSorthoF (ortho,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSorthoF (ortho,xy2,my_ll);
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
**	kk = CSorthoK (ortho,ll);
**
**	struct cs_Ortho_ *ortho;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal Equal
**								Area projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	Normally, the k function returns the scale along a meridian.
**	In this case, as is the case with azimuthal projections
**	in general, k is calculated as the scale along a radial
**	line from the origin of the projection.  In this case,
**	this value is simply 1.0.
**
**	This projection is not defined, as far as we know at the
**	current time, for the ellipsoid.  We simple use the
**	equatorial radius of the supplied ellipsoid as the radius
**	of the sphere.
**********************************************************************/

double EXP_LVL9 CSorthoK (Const struct cs_Ortho_ *ortho,Const double ll [2])
{
	extern double cs_One;			/* 1.0 */

	return (cs_One);
}									/*lint !e715 */
/**********************************************************************
**	kk = CSorthoK (ortho,ll);
**
**	struct cs_Ortho_ *ortho;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal Equal
**								Area projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	Normally, the h function returns the scale along a parallel.
**	In this case, as is the case with azimuthal projections
**	in general, h is calculated as the scale along a line
**	orthogonal to a radial line from the origin of the projection.
**
**	This projection is not defined, as far as we know at the
**	current time, for the ellipsoid.  We simple use the
**	equatorial radius of the supplied ellipsoid as the radius
**	of the sphere.
**********************************************************************/

double EXP_LVL9 CSorthoH (Const struct cs_Ortho_ *ortho,Const double ll [2])
{
	extern double cs_Degree;			/*  1.0 / RADIAN */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_AnglTest;			/* 0.001 arc seconds in radians. */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds short
										   of the south pole, in
										   radians. */

	double lng;
	double lat;
	double del_lng;
	double h;

	double cos_lat;
	double sin_lat;
	double cos_del_lng;

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}
	sin_lat = sin (lat);

	/* We can save significant computation by examining the
	   aspect of the projection. Note, the setup function sets
	   sin_org_lat and cos_org_lat to hard values appropriate for
	   each aspect. */

	if (ortho->org_lat >= cs_NPTest)
	{
		/* North polar aspect. */

		h = sin_lat;
	}
	else if (ortho->org_lat <= cs_SPTest)
	{
		/* South polar aspect. */

		h = -sin_lat;
	}
	else
	{
		lng = ll [LNG] * cs_Degree;
		del_lng = CS_adj2pi (lng - ortho->org_lng);
		cos_del_lng = cos (del_lng);
		cos_lat = cos (lat);

		if (fabs (ortho->org_lat) <= cs_AnglTest)
		{
			/* Equatorial aspect. */

			h = cos_lat * cos_del_lng;
		}
		else
		{
			/* Oblique aspect. */

			h = (ortho->sin_org_lat * sin_lat) +
			    (ortho->cos_org_lat * cos_lat * cos_del_lng);
		}
	}
	return (h);
}

/**********************************************************************
**	status = CSorthoL (ortho,cnt,pnts);
**
**	struct cs_Ortho_ *ortho;	the coordinate system against which the check is
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

int EXP_LVL9 CSorthoL (Const struct cs_Ortho_ *ortho,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double test_xy [3];

	/* If all the points are inside the envelope, then all
	   lines and/or regions will be as well. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSorthoF (ortho,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSorthoX (ortho,cnt,pnts);
**
**	struct cs_Ortho_ *ortho;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSorthoL to check lat/long
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

int EXP_LVL9 CSorthoX (Const struct cs_Ortho_ *ortho,int cnt,Const double pnts [][3])
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
		status = CSorthoI (ortho,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

