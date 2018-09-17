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
**	err_cnt = CShmlsnQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code,	currently unused.
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

int EXP_LVL9 CShmlsnQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Homolosine specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* We could now check the zone specifications, but CS_zones is
	   pretty robust.  It produces rational results no matter
	   what gets thrown at it.  If a check is to be developed
	   in the future, we proably should just add the error
	   list parameters to CS_zone and modify the other calls to
	   it to disable the feature. */

	/* That's it for Homolosine. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CShmlsnS (cs_prm);
**
**	struct cs_Csprm_*cs_prm;	struture containing the coordinate system definition,
**								datum definition, and ellipsoid definition.  Results
**								are returned in the hmlsn member of the prj_prms
**								union.
**
**********************************************************************/

void EXP_LVL9 CShmlsnS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 180.0 / Pi */
	extern double cs_Degree;			/* Pi / 180.0 */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_One;				/*    1.0 */
	extern double cs_Mone;				/*   -1.0 */
	extern double cs_Two;				/*    2.0 */
	extern double cs_Eight;				/*    8.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	int ii;

	struct cs_Hmlsn_ *hmlsn;
	struct cs_Zone_ *zp;

	double del_lng;
	double qxk;

	hmlsn = &csprm->proj_prms.hmlsn;

	/* Transfer the necessary arguments to the cs_Hmlsn_
	   structure. */

	hmlsn->org_lng = csprm->csdef.org_lng * cs_Degree;
	hmlsn->k = csprm->csdef.scale;
	hmlsn->e_rad = csprm->datum.e_rad;
	hmlsn->ka = hmlsn->e_rad * hmlsn->k;
	hmlsn->x_off = csprm->csdef.x_off;
	hmlsn->y_off = csprm->csdef.y_off;
	hmlsn->one_cm = csprm->csdef.unit_scl * 0.01;
	if (hmlsn->e_rad == cs_One) hmlsn->one_cm = 2.0E-10;	/*lint !e777 */
	hmlsn->cnvrg_val = cs_AnglTest;
	hmlsn->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* The following values are used in the Mollweide
	   calculations.  Since this projection is always
	   used for the sphere, there are no special values
	   for the sinusoidal. */

	hmlsn->kF_x = (sqrt (cs_Eight) / cs_Pi) * hmlsn->ka;
	hmlsn->kF_y = sqrt (cs_Two) * hmlsn->ka;

	hmlsn->kI_theta = cs_One / hmlsn->kF_y;
	hmlsn->kI_lng = cs_One / hmlsn->kF_x;

	/* Specify the latitude at which we switch from
	   Sinusoidal to Mollweide.  The value is documented
	   at 44 degrees, 44 minutes, and 12 seconds.  I suspect it
	   is somewhat different.  It should be the latitude at
	   which the K scale factor (i.e. scale along a parallel)
	   is the same for both projections.  In the case of the
	   sinusoidal, K is always one.  Thus, the switch latitude
	   is the latitude at which the Mollweide has a K scale
	   factor of 1. */

	hmlsn->sw_lat = 40.73666218973456 * cs_Degree;
	hmlsn->sw_yy  = hmlsn->sw_lat * hmlsn->ka;
	hmlsn->yy_crct = 0.052803527368531 * hmlsn->ka;

	/* Max X is a Sinusoidal calculation, max Y is a Mollweide
	   calculation. */

	hmlsn->max_xx = cs_3Pi_o_2 * hmlsn->ka;
	hmlsn->max_yy = hmlsn->kF_y * hmlsn->ka;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = hmlsn->org_lng * cs_Radian;
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

		csprm->min_xy [XX] = -hmlsn->max_xx;
		csprm->max_xy [XX] =  hmlsn->max_xx;
		csprm->min_xy [YY] = -hmlsn->max_yy;
		csprm->max_xy [YY] =  hmlsn->max_yy;

		CS_quadMM (csprm->min_xy,csprm->max_xy,hmlsn->x_off,
										       hmlsn->y_off,
										       hmlsn->quad);
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

	csprm->ll2cs    = (cs_CS2LL_CAST)CShmlsnF;
	csprm->cs2ll    = (cs_LL2CS_CAST)CShmlsnI;
	csprm->cs_scale = (cs_SCALE_CAST)CShmlsnK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CShmlsnK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CShmlsnH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CShmlsnC;
	csprm->llchk    = (cs_LLCHK_CAST)CShmlsnL;
	csprm->xychk    = (cs_XYCHK_CAST)CShmlsnX;

	/* See if there are any zones. */

	hmlsn->zone_cnt = (short)CS_zones (&csprm->csdef,hmlsn->zones);

	/* If there were some zones, we need to populate the cartesian
	   portions thereof.  We use qxk to adjust our zone definition
	   numbers for non-standard quadrants. */

	qxk = ((hmlsn->quad & cs_QUAD_INVX) == 0) ? cs_One : cs_Mone;
	for (ii = 0;ii < hmlsn->zone_cnt;ii++)
	{
		zp = &hmlsn->zones [ii];

		/* Latitude is zero for all of these calculations. */

		del_lng = zp->west_lng - hmlsn->org_lng;
		zp->west_xx = (del_lng * hmlsn->kF_x * qxk) + hmlsn->x_off;

		del_lng = zp->cent_lng - hmlsn->org_lng;
		zp->x_off = (del_lng * hmlsn->kF_x * qxk) + hmlsn->x_off;

		del_lng = zp->east_lng - hmlsn->org_lng;
		zp->east_xx = (del_lng * hmlsn->kF_x * qxk) + hmlsn->x_off;
	}

	return;
}

/**********************************************************************
**	rtn_val = CShmlsnF (hmlsn,xy,ll)
**
**	struct cs_Hmlsn_ *hmlsn;	structure which defines the parameters
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

int EXP_LVL9 CShmlsnF ( Const struct cs_Hmlsn_ *hmlsn,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.23.... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_3Pi_o_2;			/* 3 Pi */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	Const struct cs_Zone_ *zp;

	double lng;		/* Provided longitude in radians. */
	double del_lng;
	double lat;		/* Provided latitude in radians. */
	double sin_lat;

	double xx;
	double yy;

	double cent_lng;	/* Central longitude, adjusted for
						   for the zone. */
	double x_off;		/* False easting, adjusted for
						   for the zone. */

	double theta;		/* Mollweide parametric angle. */
	double del_theta;
	double sin_theta;
	double cos_theta;

	rtn_val = cs_CNVRT_NRML;

	/* Convert to radians for the zone search. */

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
	lng = ll [LNG] * cs_Degree;

	/* Locate the proper zone if there are any. */

	if (hmlsn->zone_cnt > 0)
	{
		zp = CS_znlocF (hmlsn->zones,hmlsn->zone_cnt,lng,lat);
		if (zp != NULL)
		{
			cent_lng = zp->cent_lng;
			x_off = zp->x_off;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
			cent_lng = hmlsn->org_lng;
			x_off = hmlsn->x_off;
		}
	}
	else
	{
		cent_lng = hmlsn->org_lng;
		x_off = hmlsn->x_off;
	}

	/* We've adjusted for the zone, do it. */

	del_lng = lng - cent_lng;
	if      (del_lng >  cs_3Pi_o_2 && cent_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < -cs_3Pi_o_2 && cent_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	/* There are two algoirthms, Sinusoidal and Mollweide.
	   Select the proper one based on the latitude. */

	if (fabs (lat) <= hmlsn->sw_lat)
	{
		/* Here for the Sinusoidal calculations.  We
		   always use the sphere for this one. */

		xx = hmlsn->ka * del_lng * cos (lat);
		yy = hmlsn->ka * lat;
	}
	else
	{
		/* Here for the Mollweide calculation. */

		theta = lat;
		sin_lat = sin (lat);
		for (;;)
		{
			sin_theta = sin (theta);
			cos_theta = cos (theta);

			del_theta  = -theta;
			del_theta -= sin_theta;
			del_theta += cs_Pi * sin_lat;
			del_theta /= cs_One + cos_theta;
			theta += del_theta;
			if (fabs (del_theta) < hmlsn->cnvrg_val) break;
		}
		sin_theta = sin (theta * cs_Half);
		cos_theta = cos (theta * cs_Half);

		xx = hmlsn->kF_x * del_lng * cos_theta;
		yy = hmlsn->kF_y * sin_theta;
		if (lat > 0.0) yy -= hmlsn->yy_crct;
		else           yy += hmlsn->yy_crct;
	}

	if (hmlsn->quad == 0)
	{
		xy [XX] = xx + x_off;
		xy [YY] = yy + hmlsn->y_off;
	}
	else
	{
		CS_quadF (xy,xx,yy,x_off,hmlsn->y_off,hmlsn->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CShmlsnI (hmlsn,ll,xy);
**
**	struct cs_Hmlsn_ *hmlsn;	structure defining the natureof the coordinate
**								system as setup by CShmlsnS.
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
**********************************************************************/

int EXP_LVL9 CShmlsnI (	Const struct cs_Hmlsn_ *hmlsn,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.23..... */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole, in radians. */

	int rtn_val;

	Const struct cs_Zone_ *zp;

	double xx;
	double yy;
	double zn_xx;
	double zn_yy;
	double lat;
	double theta;

	double cent_lng;	/* Central longitude, adjusted for
						   the appropriate zone, if any. */
	double x_off;		/* False easting, adjusted for the
						   appropriate zone, if any. */
	double del_lng;
	double sin_theta;
	double cos_theta;
	double tmp;

	rtn_val = cs_CNVRT_NRML;

	/* Locate the appropriate zone. */

	if (hmlsn->zone_cnt <= 0)
	{
		/* No zones, this is easy. */

		cent_lng = hmlsn->org_lng;
		x_off    = hmlsn->x_off;
	}
	else
	{
		/* We have a complication introduced by variable
		   quadrants.  In order to locate the zone, we
		   must undo the quadrant processing in selected
		   pieces. */

		zn_xx = xy [XX];
		zn_yy = xy [YY];
		if ((hmlsn->quad & cs_QUAD_SWAP) != 0)
		{
			zn_xx = xy [YY];
			zn_yy = xy [XX];
		}
		zn_yy -= hmlsn->y_off;

		/* We can now use zn_xx and zn_yy to locate the proper
		   zone. */

		zp = CS_znlocI (hmlsn->zones,hmlsn->zone_cnt,zn_xx,zn_yy);
		if (zp != NULL)
		{
			cent_lng = zp->cent_lng;
			x_off    = zp->x_off;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
			cent_lng = hmlsn->org_lng;
			x_off    = hmlsn->x_off;
		}
	}

	/* Remove false origin. */

	if (hmlsn->quad == 0)
	{
		xx = xy [XX] - x_off;
		yy = xy [YY] - hmlsn->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,x_off,hmlsn->y_off,hmlsn->quad);
	}

	/* Have now adjusted for the zone.  There are two algorithms:
	   Sinusoidal and Mollweide.  In both cases below, we check
	   the resultant delta longitude against cs-3Pi_o_2.  This is
	   appropriate as if zones were present, the zone check above
	   will have set the appropriate return code.  If zones are
	   not present, cs_3Pi_o_2 is the appropriate value. */

	if (fabs (yy) <= hmlsn->sw_yy)
	{
		/* Here for the Sinusoidal.  Note, by selecting
		   the sinusoidal, we have essentially insured
		   that the yy value is within the appropriate
		   range. */

		lat = yy / hmlsn->ka;
		del_lng = xx / (hmlsn->ka * cos (lat));
	}
	else
	{
		/* Here for the Mollweide.  Compute theta, the
		   parametric angle. Be careful, theta resembles
		   latitude, and cosine theta will be zero at the
		   poles. */

		if (yy > 0.0) yy += hmlsn->yy_crct;
		else          yy -= hmlsn->yy_crct;
		sin_theta = yy * hmlsn->kI_theta;
		if (fabs (sin_theta) > cs_One)
		{
			rtn_val = cs_CNVRT_RNG;
			sin_theta = (sin_theta >= 0.0) ? cs_One : cs_Mone;
		}
		theta = asin (sin_theta);
		cos_theta = cos (theta);

		tmp = theta + theta;
		tmp += sin (tmp);
		lat = asin (tmp / cs_Pi);

		if (fabs (lat) < cs_NPTest)
		{
			del_lng = (xx * hmlsn->kI_lng) / cos_theta;
		}
		else
		{
			if (rtn_val == cs_CNVRT_NRML) rtn_val = cs_CNVRT_INDF;
			del_lng = cs_Zero;
		}
	}

	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		del_lng = CS_adj2pi (del_lng);
		rtn_val = cs_CNVRT_RNG;
	}

	ll [LNG] = (del_lng + cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CShmlsnC (hmlsn,ll);
**
**	struct cs_Hmlsn_ *hmlsn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Godde Homolosine
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

double EXP_LVL9 CShmlsnC (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2])

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

	   Should be zero on the central meridian and the equator. */

	my_ll [LAT] -= 0.00005;
	status = CShmlsnF (hmlsn,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CShmlsnF (hmlsn,xy2,my_ll);
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
**	kk = CShmlsnK (hmlsn,ll);
**
**	struct cs_Hmlsn_ *hmlsn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Goode Homolosine
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

double EXP_LVL9 CShmlsnK (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2])
{
	extern double cs_Degree;			/*  1/ 57.2957... */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0, the value returned
										   when provided a bogus
										   point. */
	extern double cs_Sin1Sec;			/* sine of one arc second */
	extern double cs_SclInf;			/* 9.9E+04, the value
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

	/* In the sinusoidal region, this is simple. */

	if (fabs (ll [LAT] * cs_Degree) <= hmlsn->sw_lat)
	{
		return (cs_One);
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
	ll1 [LNG] = ll [LNG] + (1.0 / 3600.0);	 /* One second of arc,
											    in degrees. */
	ll1 [LAT] = ll [LAT];

	/* Establish two points, pretty close together. */

	status = CShmlsnF (hmlsn,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CShmlsnF (hmlsn,xy2,ll1);
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
	   know that the sine of very small angles, like one arc
	   second, is the same as the angle.  Working these facts into
	   spherical trigonometry yields the following simple formula
	   for the geographic distance on a sphere for an arc second.
	   cs_Sin1Sec is the sine of one arc second, or one arc second.
	   See Synder, page 30. */

	dd_ll = hmlsn->ka * cs_Sin1Sec * cos (ll0 [LAT] * cs_Degree);

	/* The ratio of the two distances is the grid scale factor.
	   Should be 1.0 on the 38th parallels, both north and south.
	   Will get huge at the poles, so we try to filter that out. */

	if (dd_ll > hmlsn->one_cm) kk = dd_xy / dd_ll;
	else			   kk = cs_SclInf;
	return (kk);
}

/**********************************************************************
**	hh = CShmlsnH (hmlsn,ll);
**
**	struct cs_Hmlsn_ *hmlsn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Goode Homolosine
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

double EXP_LVL9 CShmlsnH (Const struct cs_Hmlsn_ *hmlsn,Const double ll [2])
{
	extern double cs_Sin1Sec;		/* sine of one arc second */
	extern double cs_Mone;			/* -1.0, the value returned
									   when provided with a bogus
									   point. */
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
	ll0 [LAT] = ll [LAT];
	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] + (1.0 / 3600.0); /* One second of arc. */

	/* Establish two points, pretty close together. */

	status = CShmlsnF (hmlsn,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CShmlsnF (hmlsn,xy2,ll1);
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

	dd_ll = hmlsn->ka * cs_Sin1Sec;

	hh = dd_xy / dd_ll;
	return (hh);
}

/**********************************************************************
**	status = CShmlsnL (hmlsn,cnt,pnts);
**
**	struct cs_Hmlsn_ *hmlsn;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][3];
**													the list of coordinates to be checked.
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

int EXP_LVL9 CShmlsnL (Const struct cs_Hmlsn_ *hmlsn,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int rtn_val;

	double lat;

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
	}
	return (rtn_val);
}								/*lint !e715 */

/**********************************************************************
**	status = CShmlsnX (hmlsn,cnt,pnts);
**
**	struct cs_Hmlsn_ *hmlsn;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CShmlsnL to check lat/long
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

int EXP_LVL9 CShmlsnX (Const struct cs_Hmlsn_ *hmlsn,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;
	int last_ns	= 0;					/* initialization to keep gcc happy */

	Const struct cs_Zone_ *zp;

	double zn_xx, zn_yy;
	double last_cm = 0.0;				/* initialization to keep gcc happy */

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.
	   No special checks are required for lines and/or regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CShmlsnI (hmlsn,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
		if (hmlsn->zone_cnt > 0)
		{
			/* We have a complication introduced by variable
			   quadrants.  In order to locate the zone, we
			   must undo the quadrant processing in selected
			   pieces. */

			zn_xx = pnts [ii][XX];
			zn_yy = pnts [ii][YY];
			if ((hmlsn->quad & cs_QUAD_SWAP) != 0)
			{
				zn_xx = pnts [ii][YY];
				zn_yy = pnts [ii][XX];
			}
			zn_yy -= hmlsn->y_off;

			/* We can now use zn_xx and zn_yy to locate the proper
			   zone. */

			zp = CS_znlocI (hmlsn->zones,hmlsn->zone_cnt,zn_xx,
								     zn_yy);
			if (zp == NULL)
			{
				rtn_val = cs_CNVRT_DOMN;
				break;
			}
			if (ii > 0)
			{
				if (zp->cent_lng != last_cm ||			/*lint !e777 !e530 */
				    zp->ns_flag  != last_ns)			/*lint !e777 !e530 */
				{
					rtn_val = cs_CNVRT_DOMN;
					break;
				}
			}
			last_cm = zp->cent_lng;
			last_ns = zp->ns_flag;
		}
	}
	return (rtn_val);
}
