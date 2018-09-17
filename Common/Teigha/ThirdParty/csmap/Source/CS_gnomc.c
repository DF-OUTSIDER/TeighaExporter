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
**	err_cnt = CSgnomcQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	currently unused.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;				the size of the array pointed to by err_list,
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

int EXP_LVL9 CSgnomcQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;	/* -180.0 */
	extern double cs_MinLat;	/* - 90.0 */
	extern double cs_MaxLng;	/* +180.0 */
	extern double cs_MaxLat;	/* + 90.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Gnomonic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	/* That's it for Gnomonic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
** CSgnomcS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	Only the spherical form of this projection is defined.
**********************************************************************/

void EXP_LVL9 CSgnomcS (struct cs_Csprm_ *csprm)

{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/*  180.0 / pi */
	extern double cs_Degree;			/*  pi / 180.0 */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_K89;				/*  89.0 */
	extern double cs_AnglTest;			/*  0.001 seconds of arc
										    in radians. */
	extern double cs_AnglTest1;			/*  cosine of AnglTest */

	struct cs_Gnomc_ *gnomc;

	gnomc = &csprm->proj_prms.gnomc;

	/* Transfer the necessary arguments to the "gnomc" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	gnomc->org_lng = csprm->csdef.org_lng * cs_Degree;
	gnomc->org_lat = csprm->csdef.org_lat * cs_Degree;
	gnomc->k = csprm->csdef.scale;
	gnomc->x_off = csprm->csdef.x_off;
	gnomc->y_off = csprm->csdef.y_off;
	gnomc->e_rad = csprm->datum.e_rad;
	gnomc->ka = csprm->datum.e_rad * gnomc->k;
	gnomc->cos_org_lat = cos (gnomc->org_lat);
	gnomc->sin_org_lat = sin (gnomc->org_lat);
	gnomc->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Force hard values for the polar and equatorial aspects.
	   This should not be necessary.  However, different run
	   time libraries behave differently. */

	if (gnomc->sin_org_lat > cs_AnglTest1)
	{
		gnomc->sin_org_lat = cs_One;
		gnomc->cos_org_lat = cs_Zero;
	}
	else if (gnomc->sin_org_lat < -cs_AnglTest1)
	{
		gnomc->sin_org_lat = cs_Mone;
		gnomc->cos_org_lat = cs_Zero;
	}
	else if (fabs (gnomc->sin_org_lat) < cs_AnglTest)
	{
		gnomc->sin_org_lat = cs_Zero;
		gnomc->cos_org_lat = cs_One;
	}

	/* Set up a value in the units of the cartesian system
	   which represents one tenth of a millimeter.  We add
	   some special code for the test cases where the e_rad
	   values and units don't make sense. */

   	gnomc->one_mm = 0.0001 * gnomc->k;
	if (csprm->datum.e_rad <= 3.0)
	{
		gnomc->one_mm = 1.0E-10;
	}

	/* The following limit specifies how close we come to
	   the indeterminate case of 90 degrees from the origin.
	   We set it equal to about 5 minutes of arc.  The
	   value is the cosine of the maximum value, i.e.
	   89 degrees, 55 minutes. */

	gnomc->cos_limit = 0.0015;

	/* The rho limit is used to limit the exposure to the
	   inverse calculation. */

	gnomc->rho_limit = gnomc->ka * (cs_One - gnomc->cos_limit);

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = gnomc->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range. */

		csprm->min_ll [LNG] = cs_Km90;
		csprm->max_ll [LNG] = cs_K90;

		csprm->min_ll [LAT] = gnomc->org_lat * cs_Radian - cs_K89;
		if (csprm->min_ll [LAT] < cs_Km90)
		{
			csprm->min_ll [LAT] = cs_Km90;
		}
		csprm->max_ll [LAT] = gnomc->org_lat * cs_Radian + cs_K89;
		if (csprm->max_ll [LAT] > cs_K90)
		{
			csprm->max_ll [LAT] = cs_K90;
		}
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
		   The setup is virtually complete, so we can use CSgnomcF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		csprm->min_xy [XX] = -gnomc->ka;
		csprm->max_xy [XX] =  gnomc->ka;
		csprm->min_xy [YY] = -gnomc->ka;
		csprm->max_xy [YY] =  gnomc->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,gnomc->x_off,
						       gnomc->y_off,
						       gnomc->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSgnomcF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSgnomcI;
	csprm->cs_scale = (cs_SCALE_CAST)CSgnomcK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSgnomcK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSgnomcH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSgnomcC;
	csprm->llchk    = (cs_LLCHK_CAST)CSgnomcL;
	csprm->xychk    = (cs_XYCHK_CAST)CSgnomcX;

	return;
}

/**********************************************************************
**	rtn_val = CSgnomcF (gnomc,xy,ll);
**
**	struct cs_Gnomc_ *gnomc;	structure which defines the parameters
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
**	This function calculates the Gnomonic projection and returns
**	values in the coordinate system described by the cs_Gnomc_
**	structure.  For this projection, only the spherical earth is
**	supported.  We use the equatorial radius of the ellipsoid
**	provided as the radius of the sphere.
**
**	All the formulas used here were extracted from the USGS
**	publication "Map Projections - A Working Manual",
**	 U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 164-168.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSgnomcF (Const struct cs_Gnomc_ *gnomc,double xy [2],Const double ll [2])

{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/* 57.2.....  */
	extern double cs_Pi;				/* 3.14159 */
	extern double cs_Mpi;				/* -3.14159 */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double del_lng;
	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;
	double cos_lat;
	double k_prime;

	double cos_c;
	double ll_az;

	double tmp;

	double ll0 [2];

	rtn_val = cs_CNVRT_NRML;

	/* For this projection, we only support the spherical form,
	   therefore there is only one set of equations.  These are
	   pretty simple. */

	lat = cs_Degree * ll [LAT];
	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	lng = cs_Degree * ll [LNG];
	del_lng = lng - gnomc->org_lng;
	if      (del_lng > cs_Pi  && gnomc->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && gnomc->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* Read cos_c as the cosine of the angular distance from the
	   origin of the projection to the point in question. */

	cos_c = (gnomc->sin_org_lat * sin_lat) +
			(gnomc->cos_org_lat * cos_lat * cos_del_lng);

	/* If cos_c is close to or less than zero, we cannot compute
	   the X and Y.  The equations are indeterminate.  We
	   choose a point on the outer perimeter which is the logical
	   location of the point.  Cos_limit is set in the setup
	   function, about one minute of arc from the outer
	   perimeter. */

	if (cos_c < gnomc->cos_limit)
	{
		rtn_val = cs_CNVRT_RNG;

		/* Compute the azimuth of the point from the origin. */

		ll0 [LNG] = gnomc->org_lng * cs_Radian;
		ll0 [LAT] = gnomc->org_lat * cs_Radian;
		ll_az = CS_azsphr (ll0,ll) * cs_Degree;

		/* Compute the X and Y values based on the limit,
		   in cartesian form, and the azimuth. */

		xy [XX] = gnomc->rho_limit * sin (ll_az);
		xy [YY] = gnomc->rho_limit * cos (ll_az);
	}
	else
	{
		/* The primary grid scale factor at the point in
		   question.  We know that cos_c will not be zero. */

		k_prime = cs_One / cos_c;

		xy [XX] = gnomc->ka * k_prime * cos_lat * sin_del_lng;
		tmp = (gnomc->cos_org_lat * sin_lat) -
			  (gnomc->sin_org_lat * cos_lat * cos_del_lng);
		xy [YY] = gnomc->ka * k_prime * tmp;
	}

	if (gnomc->quad == 0)
	{
		xy [XX] += gnomc->x_off;
		xy [YY] += gnomc->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],gnomc->x_off,gnomc->y_off,gnomc->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSgnomcI (gnomc,ll,xy);
**
**	struct cs_Gnomc_ *gnomc;	structure which defines the parameters
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
**	John P. Synder, pages 164-168.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**	For this projection, only the spherical form is supported.
**********************************************************************/

int EXP_LVL9 CSgnomcI (Const struct cs_Gnomc_ *gnomc,double ll [2],Const double xy [2])

{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the south pole,
										   in radians. */

	int rtn_val;

	double x;
	double y;
	double rho;

	double c;
	double lat;
	double del_lng;
	double sin_c;
	double cos_c;

	double tmp;

	rtn_val = cs_CNVRT_NRML;

	if (gnomc->quad == 0)
	{
		x = xy [XX] - gnomc->x_off;
		y = xy [YY] - gnomc->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,gnomc->x_off,gnomc->y_off,gnomc->quad);
	}

	rho = sqrt (x * x + y * y);

	/* We divide by rho, so we need to be careful about
	   rho being zero. */

	if (rho < gnomc->one_mm)
	{
		del_lng = cs_Zero;
		lat = gnomc->org_lat;
	}
	else
	{
		/* If rho is greater than or equal to the maximum
		   calculated in the setup function, we have a
		   problem.  The coordinate is not really on the map.
		   We recompute our local x and y to represent a
		   point on the circumference of the map. */

		if (rho > gnomc->rho_limit)
		{
			rtn_val = cs_CNVRT_RNG;
			x *= gnomc->rho_limit / rho;
			y *= gnomc->rho_limit / rho;
			rho = gnomc->rho_limit;
		}

		/* The latitude calculation is common to all aspects.
		   c is the angular distance from the origin to the
		   point being converted.  Therefore, -pi/4 <= c >= pi/4,
		   and cos_c is always positive. */

		c = atan (rho / gnomc->ka);
		cos_c = cos (c);
		sin_c = sin (c);
		tmp = (y * sin_c * gnomc->cos_org_lat) / rho;
		tmp = (cos_c * gnomc->sin_org_lat) + tmp;
		lat = asin (tmp);

		/* The longitude calculation is much faster for the
		   two polar aspects.  There is no mathematical reason
		   for having three different cases.

		   We've already dealt with the case of rho == 0.  Thus,
		   we know that at least one of X and Y is not zero,
		   and the atan2's based on X and Y alone are OK. */

		if (gnomc->org_lat >= cs_NPTest)
		{
			/* Here for the north polar aspect:
			   cos_org_lat = 0, sin_org_lat = +1 */

			del_lng = atan2 (x,-y);
		}
		else if (gnomc->org_lat <= cs_SPTest)
		{
			/* Here for the south polar aspect:
			   cos_org_lat = 0, sin_org_lat = -1 */

			del_lng = atan2 (x,y);
		}
		else if (fabs (gnomc->org_lat) <= cs_AnglTest)
		{
			/* Here for the equatorial aspect:
			   cos_org_lat = 1, sin_org_lat = 0.
			   Both arguments can be zero if x == 0.
			   But if x == 0, lng = gnomc->org_lng. */

         // SAFE -- Changed < to > in the following test. Believed typo.
			if (fabs (x) > gnomc->one_mm)
			{
				del_lng = atan2 (x * sin_c,rho * cos_c);
			}
			else
			{
			    	del_lng = cs_Zero;
			}
		}
		else
		{
			/* Here for the oblique aspect. Again, both
			   arguments can be zero, but again, only if
			   x == 0. */

			if (fabs (x) > gnomc->one_mm)
			{
				tmp = (rho * gnomc->cos_org_lat * cos_c) -
				      (  y * gnomc->sin_org_lat * sin_c);
				del_lng = atan2 (x * sin_c,tmp);
			}
			else
			{
			    	del_lng = cs_Zero;
			}
		}
	}
	if (fabs (lat) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
	    	rtn_val = cs_CNVRT_INDF;
	}
		
	/* Convert the results to degrees. */

	ll [LNG] = (del_lng + gnomc->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSgnomcC (gnomc,ll);
**
**	struct cs_Gnomc_ *gnomc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Gnomonic projection.
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

double EXP_LVL9 CSgnomcC (Const struct cs_Gnomc_ *gnomc,Const double ll [2])
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
	status = CSgnomcF (gnomc,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSgnomcF (gnomc,xy2,my_ll);
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
**	kk = CSgnomcK (gnomc,ll);
**
**	struct cs_Gnomc_ *gnomc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Gnomonic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	Normally, the K function returns the scale along a parallel.
**	In this case, as is the case with many azimuthal projections,
**	is calculated as the scale along a radial line from the origin
**	of the projection.
**********************************************************************/

double EXP_LVL9 CSgnomcK (Const struct cs_Gnomc_ *gnomc,Const double ll [2])

{
	extern double cs_Degree;			/*  1.0/RADIAN */
	extern double cs_Pi_o_2;			/*  Pi over 2  */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_SclInf;			/* 9.99E+04, the value we
										   return for infinite
										   scale. */

	double kk;
	double lng;
	double lat;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double cos_del_lng;

	double cos_c;			/* Cosine of the angular distance
							   from the projection origin to
							   the point provided. */

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}
	lng = ll [LNG] * cs_Degree;
	cos_lat = cos (lat);
	sin_lat = sin (lat);

	del_lng = CS_adj2pi (lng - gnomc->org_lng);
	cos_del_lng = cos (del_lng);

	cos_c = (gnomc->sin_org_lat * sin_lat) +
		(gnomc->cos_org_lat * cos_lat * cos_del_lng);

	if (cos_c < gnomc->cos_limit)
	{
		kk = cs_SclInf;
	}
	else
	{
		kk = cs_One / cos_c;
	}
	return (kk);
}

/**********************************************************************
**	hh = CSgnomcH (gnomc,ll);
**
**	struct cs_Gnomc_ *gnomc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Gnomonic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double hh;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	Normally, the h function returns the scale along a meridian.
**	In this case, as is the case with many azimuthal projections,
**	h is calculated as the scale along a line orthogonal to a
**	radial line from the origin of the projection.
**********************************************************************/

double EXP_LVL9 CSgnomcH (Const struct cs_Gnomc_ *gnomc,Const double ll [2])
{
	extern double cs_Degree;			/*  1.0 / RADIAN */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */

	double hh;
	double lng;
	double lat;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double cos_del_lng;

	double cos_c;

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}
	cos_lat = cos (lat);
	sin_lat = sin (lat);

	lng = ll [LNG] * cs_Degree;
	del_lng = CS_adj2pi (lng - gnomc->org_lng);
	cos_del_lng = cos (del_lng);

	cos_c = (gnomc->sin_org_lat * sin_lat) +
		(gnomc->cos_org_lat * cos_lat * cos_del_lng);

	if (cos_c < gnomc->cos_limit)
	{
		hh = cs_SclInf;
	}
	else
	{
		hh = cs_One / (cos_c * cos_c);
	}
	return (hh);
}

/**********************************************************************
**	status = CSgnomcL (gnomc,cnt,pnts);
**
**	struct cs_Gnomc_ *gnomc;	the coordinate system against which the check is
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

int EXP_LVL9 CSgnomcL (Const struct cs_Gnomc_ *gnomc,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  A point is generally OK if it is
	   within 90 degrees (angular distance) of the origin. */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSgnomcF (gnomc,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSgnomcX (gnomc,cnt,pnts);
**
**	struct cs_Gnomc_ *gnomc;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSgnomcL to check lat/long
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

int EXP_LVL9 CSgnomcX (Const struct cs_Gnomc_ *gnomc,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.
	   Generally, a point is OK if it's cartesian distance
	   from the origin is less than gnomc->rho_limit. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSgnomcI (gnomc,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
