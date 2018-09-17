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

/**********************************************************************
**	err_cnt = CSsinusQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSsinusQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Sinusoidal specific stuff.  Note, we require a
	   central meridian (origin longitude) even if there are
	   zones. */

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

	/* That's it for Sinusoidal. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSsinusS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CSsinusS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;				/*  57.2957...  */
	extern double cs_Degree;				/*   1.0 / RADIAN  */
	extern double cs_3Pi_o_2;				/* 3 Pi over 2 */
	extern double cs_Pi_o_2;				/* Pi over 2 */
	extern double cs_Zero;					/*   0.0 */
	extern double cs_Half;					/*   0.5 */
	extern double cs_One;					/*   1.0 */
	extern double cs_Mone;					/*   1.0 */
	extern double cs_K180;					/* 180.0 */
	extern double cs_K90;					/*  90.0 */
	extern double cs_Km90;					/* -90.0 */

	int ii;

	struct cs_Sinus_ *sinus;
	struct cs_Zone_ *zp;

	double qxk;
	double del_lng;
	double west_lng;
	double east_lng;

	sinus = &csprm->proj_prms.sinus;

	/* Transfer the necessary arguments to the
	   "sinus" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	sinus->cent_lng = csprm->csdef.org_lng * cs_Degree;
	sinus->k = csprm->csdef.scale;
	sinus->x_off = csprm->csdef.x_off;
	sinus->y_off = csprm->csdef.y_off;
	sinus->ecent = csprm->datum.ecent;
	sinus->e_sq = sinus->ecent * sinus->ecent;
	sinus->e_rad = csprm->datum.e_rad;
	sinus->ka = sinus->k * sinus->e_rad;
	sinus->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	sinus->one_mm = 0.001 * sinus->k;
	sinus->max_xx = sinus->ka * cs_3Pi_o_2;
	sinus->max_yy = sinus->ka * cs_Pi_o_2;		/* For the sphere,
												   replaced below
												   for the
												   ellipsoid. */

	/* No special set up required for the sphere. */

	if (sinus->ecent != 0.0)
	{
		/* Here only for the ellipsoid. */

		CSmmFsu (&sinus->mmcofF,sinus->ka,sinus->e_sq);
		CSmmIsu (&sinus->mmcofI,sinus->ka,sinus->e_sq);
		sinus->max_yy = CSmmFcal (&sinus->mmcofF,cs_Pi_o_2,cs_One,cs_Zero);
	}

	/* Set up the function pointers. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSsinusF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSsinusI;
	csprm->cs_scale = (cs_SCALE_CAST)CSsinusH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSsinusK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSsinusH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSsinusC;
	csprm->llchk    = (cs_LLCHK_CAST)CSsinusL;
	csprm->xychk    = (cs_XYCHK_CAST)CSsinusX;

	/* Setup the zones. */

	sinus->zone_cnt = (short)CS_zones (&csprm->csdef,sinus->zones);

	/* Set up the easting values.  Note, if the latitude is
	   zero, there is no difference between normal eastings
	   and zoned eastings.  The zone affects the easting
	   value only when the latitude is not zero.  Also,
	   the Y values are always independent of the zone.

	   Since the latitude is zero for all of these calculations,
	   we can easily do them here rather efficiently, and the
	   calculation is the same, regardless of sphere or
	   ellipsoid.

	   Note, we also accumulate the minimum and maximum longitude
	   values for use in setting up user limits below. */

	west_lng = cs_3Pi_o_2;
	east_lng = -cs_3Pi_o_2;

	/* We use qxk to adjust our zone definition numbers for
	   non-standard quadrants. */

	qxk = ((sinus->quad & cs_QUAD_INVX) == 0) ? cs_One : cs_Mone;
	for (ii = 0;ii < sinus->zone_cnt;ii++)
	{
		zp = &sinus->zones [ii];

		del_lng = (zp->west_lng - sinus->cent_lng);
		zp->west_xx = (sinus->ka * del_lng * qxk) + sinus->x_off;

		del_lng = (zp->cent_lng - sinus->cent_lng);
		zp->x_off = (sinus->ka * del_lng * qxk) + sinus->x_off;

		del_lng = (zp->east_lng - sinus->cent_lng);
		zp->east_xx = (sinus->ka * del_lng * qxk) + sinus->x_off;

		if (zp->west_lng < west_lng) west_lng = zp->west_lng;
		if (zp->east_lng > east_lng) east_lng = zp->east_lng;
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	if (sinus->zone_cnt > 0)
	{
		csprm->cent_mer = cs_Half * (east_lng + west_lng) * cs_Radian;
	}
	else
	{
		csprm->cent_mer = sinus->cent_lng * cs_Radian;
		east_lng = cs_K180 * cs_Degree;
		west_lng = -east_lng;
	}
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */
		csprm->min_ll [LNG] = west_lng;
		csprm->max_ll [LNG] = east_lng;
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

		csprm->min_xy [XX] = -sinus->max_xx;
		csprm->max_xy [XX] =  sinus->max_xx;

		csprm->min_xy [YY] = -sinus->max_yy;
		csprm->max_xy [YY] =  sinus->max_yy;

		CS_quadMM (csprm->min_xy,csprm->max_xy,sinus->x_off,
											   sinus->y_off,
											   sinus->quad);
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
	return;
}

/**********************************************************************
**	rtn_val = CSsinusF (sinus,xy,ll);
**
**	struct cs_Sinus_ *sinus;	structure which defines the parameters
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
**	This function calculates the Sinusoidal
**	projection and returns values in the coordinate system
**	described in the cs_Sinus_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 243-248.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSsinusF (Const struct cs_Sinus_ *sinus,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole, in radians. */

	int rtn_val;

	Const struct cs_Zone_ *zp;

	double lng;			/* The given longitude, after conversion
						   to radians. */
	double lat;			/* The given latitude after conversion
						   to radians. */

	double cent_lng;		/* Local central meridian after adjust-
							   ment for appropriate zones. */
	double x_off;			/* Local false easting after adjustment
							   for the appropriate zone. */
	double del_lng;
	double sin_lat;
	double cos_lat;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	/* Convert the latitude and longitude to radians. */

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

	/* If there are any interrupted zones for this coordinate
	   system, select the proper zone for this longitude and
	   latitude.  Once selected, we use the zone to establish
	   the origin longitude and the appropriate false easting
	   for the zone. */

	if (sinus->zone_cnt > 0)
	{
		zp = CS_znlocF (sinus->zones,sinus->zone_cnt,lng,lat);
		if (zp != NULL)
		{
			cent_lng = zp->cent_lng;
			x_off    = zp->x_off;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
			cent_lng = sinus->cent_lng;
			x_off    = sinus->x_off;
		}
	}
	else
	{
		zp = NULL;
		cent_lng = sinus->cent_lng;
		x_off    = sinus->x_off;
	}

	/* Perform the conversion. */

	del_lng = lng - cent_lng;
	if      (del_lng >  cs_3Pi_o_2 && cent_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < -cs_3Pi_o_2 && cent_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	if (sinus->ecent == 0.0)
	{
		/* Here for the sphere. */

		xy [XX] = sinus->ka * del_lng * cos (lat);
		xy [YY] = sinus->ka * lat;
	}
	else
	{
		/* Here for the ellipsoid. Tmp1 should never be zero. */

		sin_lat = sin (lat);
		cos_lat = cos (lat);
		tmp1 = cs_One - (sinus->e_sq * sin_lat * sin_lat);
		xy [XX] = sinus->ka * del_lng * cos_lat / sqrt (tmp1);
		xy [YY] = CSmmFcal (&sinus->mmcofF,lat,sin_lat,cos_lat);
	}

	/* Apply the false easting and northing.  Notice that the X
	   is now relative to the zone origin, thus we must use
	   the zone False Easting. */

	if (sinus->quad == 0)
	{
		xy [XX] += x_off;
		xy [YY] += sinus->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],x_off,sinus->y_off,sinus->quad);
	}

	/* That's it. */

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSsinusI (sinus,ll,xy);
**
**	struct cs_Sinus_ *sinus;	structure which defines the parameters
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
**	1984, pages 243-248.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSsinusI (Const struct cs_Sinus_ *sinus,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Mpi_o_2;			/* -Pi over 2 */
	extern double cs_3Pi_o_2;			/* 3 Pi over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole in
										   radians. */

	int rtn_val;

	Const struct cs_Zone_ *zp;

	double xx;
	double yy;
	double zn_xx;
	double zn_yy;
	double lat;
	double lng;
	double del_lng;

	double cent_lng;		/* Local central longitude
							   after adjustment for the
							   appropriate zone. */
	double x_off;			/* The false easting appropriate
							   for the zone. */
	double sin_lat;
	double cos_lat;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	/* Before we can do much with the inverse, we need to determine
	   which zone the coordinate is in, if zones are active. */

	if (sinus->zone_cnt <= 0)
	{
		/* No zones are active, this is easy. */

		cent_lng = sinus->cent_lng;
		x_off    = sinus->x_off;
		zp = NULL;
	}
	else
	{
		/* We have a complication introduced by variable
		   quadrants.  In order to locate the zone, we
		   must undo the quadrant processing in selected
		   pieces. */

		zn_xx = xy [XX];
		zn_yy = xy [YY];
		if ((sinus->quad & cs_QUAD_SWAP) != 0)
		{
			zn_xx = xy [YY];
			zn_yy = xy [XX];
		}
		zn_yy -= sinus->y_off;

		/* We can now use zn_xx and zn_yy to locate the proper
		   zone. */

		zp = CS_znlocI (sinus->zones,sinus->zone_cnt,zn_xx,zn_yy);
		if (zp != NULL)
		{
			cent_lng = zp->cent_lng;
			x_off    = zp->x_off;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
			cent_lng = sinus->cent_lng;
			x_off    = sinus->x_off;
		}
	}

	/* Remove the false origin. */

	if (sinus->quad == 0)
	{
		xx = xy [XX] - x_off;
		yy = xy [YY] - sinus->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,x_off,sinus->y_off,sinus->quad);
	}

	/* Rather sane stuff from here on out. */

	if (fabs (yy) > sinus->max_yy)
	{
		rtn_val = cs_CNVRT_RNG;
		yy = (yy >= 0.0) ? sinus->max_yy : -sinus->max_yy;
	}

	if (fabs (xx) > sinus->max_xx)
	{
		rtn_val = cs_CNVRT_RNG;
		xx = (xx >= 0.0) ? sinus->max_xx : -sinus->max_xx;
	}

	/* We have now compensated for the zone. */

	del_lng = cs_Zero;
	if (sinus->ecent == 0.0)
	{
		/* Here for the shpere. Cos (lat) will be zero at
		   the poles. In this case, any longitude will do. */

		lat = yy / sinus->ka;
		if (fabs (lat) < cs_NPTest)
		{
			cos_lat = cos (lat);
			del_lng = xx / (sinus->ka * cos_lat);
		}
		else if (rtn_val == cs_CNVRT_NRML)
		{
			lat = (lat > 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
			rtn_val = cs_CNVRT_INDF;
		}
	}
	else
	{
		/* Here for the ellipsoid.  Again, the longitude is
		   indeterminate at the poles, and any longitude will
		   do. */

		lat = CSmmIcal (&sinus->mmcofI,yy);
		if (fabs (lat) < cs_NPTest)
		{
			cos_lat = cos (lat);
			sin_lat = sin (lat);
			tmp1 = cs_One - (sinus->e_sq * sin_lat * sin_lat);
			del_lng = (xx * sqrt (tmp1)) / (sinus->ka * cos_lat);
		}
		else if (rtn_val == cs_CNVRT_NRML)
		{
			lat = (lat > 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
			rtn_val = cs_CNVRT_INDF;
		}
	}

	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	lng = del_lng + cent_lng;

	/* If the X and Y coordinates are in the void space between
	   the zones, the resulting longitude will end up in a zone
	   other than the one we started with.  In this case, we have
	   yet another range error, and we adjust the longitude to
	   the appropriate edge of the zone we started out in. */

	if (zp != NULL)
	{
		if (lng < zp->west_lng)
		{
			rtn_val = cs_CNVRT_RNG;
			lng = zp->west_lng;
		}
		if (lng > zp->east_lng)
		{
			rtn_val = cs_CNVRT_RNG;
			lng = zp->east_lng;
		}
	}

	/* Convert to degrees. */

	ll [LNG] = lng * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSsinusC (sinus,ll);
**
**	struct cs_Sinus_ *sinus;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Sinusoidal projection.
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

double EXP_LVL9 CSsinusC (Const struct cs_Sinus_ *sinus,Const double ll [2])
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
	   the mathemagics of the specific projection involved.

	   Should be zero on the central meridian and the equator. */

	my_ll [LAT] -= 0.00005;
	status = CSsinusF (sinus,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSsinusF (sinus,xy2,my_ll);
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
**	k = CSsinusK (sinus,ll);
**
**	struct cs_Sinus_ *sinus;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Sinusoidal
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
**	For this projection, k = 1.0 no matter where you are.
**********************************************************************/

double EXP_LVL9 CSsinusK (Const struct cs_Sinus_ *sinus,Const double ll [2])
{
	extern double cs_One;			/* 1.0 */

	return (cs_One);
}									/*lint !e715 */

/**********************************************************************
**	k = CSsinusH (sinus,ll);
**
**	struct cs_Sinus_ *sinus;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Sinusoidal
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
**	We have located specific formulas for the sphere, but not for
**	the ellipsoid.  In the ellipsoid case, we use an empirical
**	determination.
**********************************************************************/

double EXP_LVL9 CSsinusH (Const struct cs_Sinus_ *sinus,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	int status;

	Const struct cs_Zone_ *zp;

	double hh;
	double lng;
	double lat;

	double del_lng;
	double sin_lat;
	double cent_lng;

	double ll_dd;
	double xy_dd;
	double del_xx, del_yy;
	double xy1 [2];
	double xy2 [2];
	double ll1 [2];
	double ll2 [2];

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	/* A latitude above either pole is an error.  At either pole,
	   the scale is 1.0.  We deal with these now to prevent
	   divides by zero below. */

	if (fabs (lat) > cs_NPTest)
	{
		if (fabs (lat) > cs_Pi_o_2)
		{
			return (cs_Mone);
		}
		else
		{
			return (cs_One);
		}
	}

	/* Determine which zone we're in, assuming there are some
	   zones active. */

	if (sinus->zone_cnt > 0)
	{
		zp = CS_znlocF (sinus->zones,sinus->zone_cnt,lng,lat);
		if (zp != NULL)
		{
			cent_lng = zp->cent_lng;
		}
		else
		{
			/* Opps!!!  No zone, even though they are
			   active.  The lat/long are bogus. */

			return (cs_Mone);
		}
	}
	else
	{
		zp = NULL;
		cent_lng = sinus->cent_lng;
	}

	/* Different algorithms for sphere or ellipsoid. */

	if (sinus->ecent == 0.0)
	{
		del_lng = CS_adj2pi (lng - cent_lng);
		sin_lat = sin (lat);
		hh = sqrt (cs_One + (del_lng * del_lng) * (sin_lat * sin_lat));
	}
	else
	{
		/* We haven'y located a formula for the ellipsoid yet.

		   Establish two points along the meridian which are
		   about 1 second (about 30 meters) apart from each
		   other, with the point in question in the middle.
		   Then convert each point to the equivalent grid
		   coordinates. */

		ll1 [LNG] = ll [LNG];
		ll1 [LAT] = ll [LAT] - (0.5 / 3600.0);
		status = CSsinusF (sinus,xy1,ll1);
		if (status != cs_CNVRT_NRML)
		{
			return (cs_Mone);
		}

		ll2 [LNG] = ll [LNG];
		ll2 [LAT] = ll [LAT] + (0.5 / 3600.0);
		status = CSsinusF (sinus,xy2,ll2);
		if (status != cs_CNVRT_NRML)
		{
			return (cs_Mone);
		}

		/* Calculate the geodetic distance between the two
		   lat/long points.  Note, we provide the geodetic
		   calculator with the scaled radius of the earth
		   so that the distance it computes will be in the
		   same units as the X and Y coordinates. */

		CS_llazdd (sinus->ka,sinus->e_sq,ll1,ll2,&ll_dd);

		/* Calculate the grid distance between the two points. */

		del_xx = xy1 [XX] - xy2 [XX];
		del_yy = xy1 [YY] - xy2 [YY];
		xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

		/* Return the ratio of the two distances as the
		   meridian scale factor at the indicated point.
		   ll_dd should never be zero here, but just in
		   case: */

		if (ll_dd > sinus->one_mm)
		{
			hh = xy_dd / ll_dd;
		}
		else
		{
			hh = cs_SclInf;
		}
	}
	return (hh);
}

/**********************************************************************
**	status = CSsinusL (sinus,cnt,pnts);
**
**	struct cs_Sinus_ *sinus;	the coordinate system against which the check is
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

int EXP_LVL9 CSsinusL (Const struct cs_Sinus_ *sinus,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int status;
	int last_ns;

	Const struct cs_Zone_ *zp;

	double lat;
	double lng;
	double last_cm;

	/* We check all the points.  In so doing, we also check to
	   see if the line formed by any successive two points
	   crosses a zone boundary.  If so, we have a problem with
	   the line, and thus any region formed by the line. To avoid
	   pointer comparisons, we use the information in the returned
	   zone structures to determine if they are the same. */

	last_ns = 0;					/* initialization to keep gcc happy */
	last_cm = 0.0;					/* initialization to keep gcc happy */
	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lng = pnts [ii][LNG] * cs_Degree;
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		if (sinus->zone_cnt > 0)
		{
			zp = CS_znlocF (sinus->zones,sinus->zone_cnt,lng,lat);
			if (zp != NULL)
			{
				if (ii != 0)
				{
					if (zp->cent_lng != last_cm ||		/*lint !e777 !e530 */
						zp->ns_flag  != last_ns)		/*lint !e530 */
					{
						/* Different zone. */

						status = cs_CNVRT_DOMN;
						break;
					}
				}
				last_cm = zp->cent_lng;
				last_ns = zp->ns_flag;
			}
			else
			{
				/* There are zones in the definition, but
				   the lat/long provided wasn't in one. */

				status = cs_CNVRT_DOMN;
				break;
			}
		}
		else
		{
			/* There are no zones.  If there is some check
			   on the longitude, add it here.  Currently,
			   we have no specific test, and we do a no-op
			   to keep the compilers happy. */

			lng = lat;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSsinusX (sinus,cnt,pnts);
**
**	struct cs_Sinus_ *sinus;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSsinusL to check lat/long
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

int EXP_LVL9 CSsinusX (Const struct cs_Sinus_ *sinus,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;
	int last_ns;

	Const struct cs_Zone_ *zp;

	double zn_xx, zn_yy;
	double last_cm;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;
	last_ns = 0;				/* initialization to keep gcc happy */
	last_cm = 0.0;				/* initialization to keep gcc happy */

	/* We use CSsinusI to verify that X and Y pairs are valid,
	   and then CSznlocI to verify that all coordinates in the
	   array are in the same zone. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSsinusI (sinus,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
		if (sinus->zone_cnt > 0)
		{
			/* We have a complication introduced by variable
			   quadrants.  In order to locate the zone, we
			   must undo the quadrant processing in selected
			   pieces. */

			zn_xx = pnts [ii][XX];
			zn_yy = pnts [ii][YY];
			if ((sinus->quad & cs_QUAD_SWAP) != 0)
			{
				zn_xx = pnts [ii][YY];
				zn_yy = pnts [ii][XX];
			}
			zn_yy -= sinus->y_off;

			/* We can now use zn_xx and zn_yy to locate the proper
			   zone. */

			zp = CS_znlocI (sinus->zones,sinus->zone_cnt,zn_xx,
								     zn_yy);
			if (zp == NULL)
			{
				rtn_val = cs_CNVRT_DOMN;
				break;
			}
			if (ii > 0)
			{
				if (zp->cent_lng != last_cm ||			/*lint !e777 !e530 */
				    zp->ns_flag  != last_ns)			/*lint !e530 */
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
