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
**	err_cnt = CSedcncQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSedcncQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_MinLatFz;		/*  -89.999 */
	extern double cs_MaxLatFz;		/* + 89.999 */
	extern double cs_ParmTest;		/* 2.78E-05 */

	int err_cnt;
	int sign1, sign2;
	
	double tmp;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Equidistant Conic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	if (cs_def->prj_prm1 <= cs_MinLatFz || cs_def->prj_prm1 >= cs_MaxLatFz)
	{
		/* Standard parallels can't be the pole. */
		
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
	}
	if (cs_def->prj_prm2 <= cs_MinLatFz || cs_def->prj_prm2 >= cs_MaxLatFz)
	{
		/* Standard parallels can't be the pole. */
		
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
	}
	
	if (fabs (cs_def->prj_prm1 - cs_def->prj_prm2) <= cs_ParmTest)
	{
		/* Here if the standard parallels are the same.  In this
		   case, they cannot be zero. */

		if (fabs (cs_def->prj_prm1) < cs_ParmTest ||
		    fabs (cs_def->prj_prm2) < cs_ParmTest)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
		}
	}
	else
	{
		/* The parallels may be on opposite sides of the equator,
		   but not equidistant from the equator. */

		sign1 = (cs_def->prj_prm1 > 0.0) ? 0 : 1;
		sign2 = (cs_def->prj_prm2 > 0.0) ? 0 : 1;
		if (sign1 ^ sign2)
		{
			tmp = fabs (cs_def->prj_prm1) - fabs (cs_def->prj_prm2);
			if (fabs (tmp) < cs_ParmTest)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
			}
		}
	}

	/* That's it for Equidistant Conic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSedcncS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	Normal conventions and CS-MAP standards have NOT been followed
**	in codng this function so that the variable nomenclature could
**	adhere as closely as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CSedcncS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 57.27...  */
	extern double cs_Degree;			/* 1.0 / 57.27...  */
	extern double cs_Pi;				/* 3.14158... */
	extern double cs_Pi_o_2;			/* PI / 2.0 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_ParmTest;			/* 0.1 seconds of arc
										   in radians. */
	extern double cs_K45;				/*  45.0 */
	extern double cs_Km45;				/* -45.0 */
	extern double cs_MinLatFz;			/* -89.997777 */
	extern double cs_MaxLatFz;			/*  89.997777 */

	struct cs_Edcnc_ *edcnc;

	double M0, M1, M2;
	double m1, m2;
	double sin_lat0;
	double cos_lat0;
	double sin_lat1;
	double cos_lat1;
	double sin_lat2;
	double cos_lat2;
	double rhoMin, rhoMax;

	double tmp1;

	edcnc = &csprm->proj_prms.edcnc;

	/* Transfer the necessary arguments to the
	   "edcnc" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	edcnc->ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
	edcnc->ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
	edcnc->org_lng = csprm->csdef.org_lng * cs_Degree;
	edcnc->org_lat = csprm->csdef.org_lat * cs_Degree;
	edcnc->k = csprm->csdef.scale;
	edcnc->x_off = csprm->csdef.x_off;
	edcnc->y_off = csprm->csdef.y_off;
	edcnc->ka = csprm->datum.e_rad * edcnc->k;
	edcnc->ecent = csprm->datum.ecent;
	edcnc->e_sq = edcnc->ecent * edcnc->ecent;
	edcnc->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	sin_lat0 = sin (edcnc->org_lat);
	cos_lat0 = cos (edcnc->org_lat);
	sin_lat1 = sin (edcnc->ref_lat1);
	cos_lat1 = cos (edcnc->ref_lat1);
	sin_lat2 = sin (edcnc->ref_lat2);
	cos_lat2 = cos (edcnc->ref_lat2);

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */

	if (edcnc->ecent == 0.0)
	{
		/* If the reference latitudes are the same, the normal
		   a calculation of the cone constant is indeterminate.
		   Note, a single reference latitude at the equator
		   is bogus, and will produce a divide by zero. */

		if (fabs(edcnc->ref_lat1 - edcnc->ref_lat2) > cs_ParmTest)
		{
			edcnc->n = (cos_lat2 - cos_lat1) /
					   (edcnc->ref_lat1 - edcnc->ref_lat2);
		}
		else
		{
			edcnc->n = sin_lat1;
		}
		edcnc->G = edcnc->ref_lat1 + (cos_lat1 / edcnc->n);
		edcnc->rho0 = edcnc->ka * (edcnc->G - edcnc->org_lat);
	}
	else
	{
		/* Here for the ellipsoidal calculations.
		   Compute the M coefficients. */

		CSmmFsu (&edcnc->mmcofF,edcnc->ka,edcnc->e_sq);

		/* Compute M0, M1, and M2. */

		M0 = CSmmFcal (&edcnc->mmcofF,edcnc->org_lat,sin_lat0,cos_lat0);
		M1 = CSmmFcal (&edcnc->mmcofF,edcnc->ref_lat1,sin_lat1,cos_lat1);
		M2 = CSmmFcal (&edcnc->mmcofF,edcnc->ref_lat2,sin_lat2,cos_lat2);

		tmp1 = edcnc->e_sq * sin_lat1 * sin_lat1;
		m1 = cos_lat1 /  sqrt (cs_One - tmp1);

		tmp1 = edcnc->e_sq * sin_lat2 * sin_lat2;
		m2 = cos_lat2 /  sqrt (cs_One - tmp1);

		/* Now for n, the cone constant.  If the two standard
		   parallels are the same, we have a special case.  The
		   lataitudes are in radians here; 4.848E-05 is
		   equivalent to .1 seconds of arc. */

		if (fabs(edcnc->ref_lat1 - edcnc->ref_lat2) > cs_ParmTest)
		{
			/* The normal case, there are two reference
			   parallels.  Ref_lat1 and ref_lat2 are different,
			   therefore, M1- M2 can't be zero.  */

			edcnc->n = edcnc->ka * ((m2 - m1) / (M1 - M2));
		}
		else
		{
			/* Here if there is only one reference
			   parallel.  Note, that this is the only
			   place where edcnc->m2 and edcnc->t2
			   are used.  They can be skipped for this
			   case but the savings in compute time are
			   so minimal, its best that what works is
			   left alone. */

			edcnc->n = sin_lat1;
		}

		/* Now for G, whatever it is. If the definition passes
		   the Q test, n can't be zero, nor can ka be
		   zero. */

		edcnc->G = (m1 / edcnc->n) + (M1 / edcnc->ka);
		edcnc->kaG = edcnc->ka * edcnc->G;

		/* Now for rho0. */

		edcnc->rho0 = edcnc->kaG - M0;

		/* Compute the coefficients for the rectifying
		   latitude inverse. */

		CSmmIsu (&edcnc->mmcofI,edcnc->ka,edcnc->e_sq);
	}

	/* Some domain checking values. We calculate min_rho and max_rho
	   as positive values, regardless of the focus pole. */

	if (edcnc->ecent == 0.0)
	{
		edcnc->min_rho = edcnc->ka * (fabs (edcnc->G) - cs_Pi_o_2);
		edcnc->max_rho = edcnc->ka * (fabs (edcnc->G) + cs_Pi_o_2);
	}
	else
	{
		M0 = CSmmFcal (&edcnc->mmcofF,cs_Pi_o_2,cs_One,cs_Zero);
		edcnc->min_rho = fabs (edcnc->kaG) - M0;
		edcnc->max_rho = fabs (edcnc->kaG) + M0;
	}
	edcnc->max_theta = fabs (edcnc->n) * cs_Pi;

	/* Set up the useful range limit values.  If the definition
	   doesn't carry any, we will compute some. */

	csprm->cent_mer = edcnc->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* Calculate a rather liberal useful range.  Longitudes
		   are easy. */

		csprm->min_ll [LNG] = cs_Km45;
		csprm->max_ll [LNG] = cs_K45;

		/* Start with the min/max latitudes equal to the
		   standard parallels. Careful here, we don't require
		   standard parallel 1 to be north of parallel 2. */

		tmp1 = edcnc->ref_lat1 - edcnc->ref_lat2;
		if (tmp1 >= 0.0)
		{
			csprm->min_ll [LAT] = edcnc->ref_lat2 * cs_Radian;
			csprm->max_ll [LAT] = edcnc->ref_lat1 * cs_Radian;
		}
		else
		{
			csprm->min_ll [LAT] = edcnc->ref_lat1 * cs_Radian;
			csprm->max_ll [LAT] = edcnc->ref_lat2 * cs_Radian;
		}

		/* Widen the coverage as is appropriate. We'll widen by
		   10 degrees (five in each direction), unless the
		   difference between the two standard parallels is
		   more than five. */

		tmp1 = fabs (tmp1) * cs_Radian;
		if (tmp1 < cs_Five) tmp1 = cs_Five;
		csprm->min_ll [LAT] -= tmp1;
		csprm->max_ll [LAT] += tmp1;

		/* Make sure we haven't gone past either pole. */

		if (csprm->min_ll [LAT] < cs_MinLatFz)
		{
			csprm->min_ll [LAT] = cs_MinLatFz;
		}
		if (csprm->max_ll [LAT] > cs_MaxLatFz)
		{
			csprm->max_ll [LAT] = cs_MaxLatFz;
		}
	}
	else
	{
		/* Use whatever the defintion gives us, without
		   checking.  We do convert the longitudes from
		   absolute to relative. */

		csprm->min_ll [LNG] = CS_adj180 (csprm->csdef.ll_min [LNG] - csprm->cent_mer);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj180 (csprm->csdef.ll_max [LNG] - csprm->cent_mer);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the XY checking information. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		if (edcnc->n >= 0.0)
		{
			/* Focus pole is north pole. */
			rhoMax = edcnc->ka * (edcnc->G - csprm->min_ll [LAT] * cs_Degree);
			rhoMin = edcnc->ka * (edcnc->G - csprm->max_ll [LAT] * cs_Degree);

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * edcnc->n * sin (tmp1) + edcnc->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * edcnc->n * sin (tmp1) + edcnc->x_off;

			csprm->min_xy [YY] = edcnc->rho0 - rhoMax + edcnc->y_off;
			csprm->max_xy [YY] = edcnc->rho0 - rhoMin + edcnc->y_off;
		}
		else
		{
			/* Focus pole is south pole, curvature of the parallel
			   arcs is "up side down". */
			rhoMax = edcnc->ka * (edcnc->G - csprm->max_ll [LAT] * cs_Degree);
			rhoMin = edcnc->ka * (edcnc->G - csprm->min_ll [LAT] * cs_Degree);

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * edcnc->n * sin (tmp1) + edcnc->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * edcnc->n * sin (tmp1) + edcnc->x_off;

			csprm->min_xy [YY] = edcnc->rho0 - rhoMin + edcnc->y_off;
			csprm->max_xy [YY] = edcnc->rho0 - rhoMax + edcnc->y_off;
		}
	}
	else
	{
		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}

	/* That's all the calculations.  Stuff some function
	   addresses and we are done. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSedcncF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSedcncI;
	csprm->cs_scale = (cs_SCALE_CAST)CSedcncK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSedcncK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSedcncH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSedcncC;
	csprm->llchk    = (cs_LLCHK_CAST)CSedcncL;
	csprm->xychk    = (cs_XYCHK_CAST)CSedcncX;

	return;
}

/**********************************************************************
**	rtn_val = CSedcncF (edcnc,xy,ll);
**
**	struct cs_Edcnc_ *edcnc;	structure which defines the parameters
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
**	This function calculates the Equidistant Conic
**	projection and returns values in the coordinate system
**	described in the cs_Edcnc_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 111-115.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSedcncF (Const struct cs_Edcnc_ *edcnc,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/*  2 PI */
	extern double cs_Pi_o_2;			/*  PI / 2.0  */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double M;
	double rho;
	double theta;
	double del_lng;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and one for the
	   ellipsoid.  If the ecentricity of the datum in use is 0.0
	   exactly, we shall use the spherical formulae.  There is a
	   miminal amount of stuff which is common to both which we
	   perform first.

	   Convert the latitude and longitude to radians. */

	lng = cs_Degree * ll [LNG];
	lat = cs_Degree * ll [LAT];

	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}

	/* Theta is polar angle. */

	del_lng = lng - edcnc->org_lng;
	if      (del_lng > cs_Pi  && edcnc->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && edcnc->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}
	theta = del_lng * edcnc->n;

	/* Rho is the length of the polar vecto, but is negative
	   if the coordinate system is centered in the southern
	   hemisphere. */
	
	if (edcnc->ecent == 0.0)
	{
		/* Here for the sphere. */

		rho = edcnc->ka * (edcnc->G - lat);
	}
	else
	{
		/* Here for the ellisoid. */

		M = CSmmFcal (&edcnc->mmcofF,lat,sin(lat),cos(lat));
		rho = edcnc->kaG - M;
	}

	/* Now we can convert to the desired coordinate system. */

	xy [XX] = rho * sin (theta);
	xy [YY] = edcnc->rho0 - (rho * cos (theta));

	/* Now I know why this projection is also known as the SIMPLE
	   conic. */

	if (edcnc->quad == 0)
	{
		xy [XX] += edcnc->x_off;
		xy [YY] += edcnc->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],edcnc->x_off,edcnc->y_off,
							  edcnc->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSedcncI (edcnc,ll,xy);
**
**	struct cs_Edcnc_ *edcnc;	structure which defines the parameters
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
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 111-115.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSedcncI (Const struct cs_Edcnc_ *edcnc,double ll [2],Const double xy [2])
{
	extern double cs_Radian;		/* 57.29577..... */

	int rtn_val;

	double x;
	double y;
	double M;
	double lat;
	double del_lng;
	double rho;
	double theta;
	double adj_y;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.

	   The longitude calculation is the same for the
	   sphere and the ellipsoid.  The value of rho, rho0,
	   and theta is the same for both calcuations. */

	if (edcnc->quad == 0)
	{
		x = xy [XX] - edcnc->x_off;
		y = xy [YY] - edcnc->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,edcnc->x_off,edcnc->y_off,edcnc->quad);
	}

	/* Calculate the polar vector length and check it. Below
	   we assume rho is not zero. */

	adj_y = edcnc->rho0 - y;
	rho = sqrt (x * x + adj_y * adj_y);
	if (rho < edcnc->min_rho)
	{
		rtn_val = cs_CNVRT_RNG;
		rho = edcnc->min_rho;
	}
	else if (rho > edcnc->max_rho)
	{
		rtn_val = cs_CNVRT_RNG;
		rho = edcnc->max_rho;
	}

	/* Calculate and check theta, the polar angle. */

	if (edcnc->n > 0.0)
	{
		/* North pole is the focus, rho must be positive. */

		theta = atan2 (x,adj_y);
	}
	else
	{
		/* South pole is the focus, rho must be negative. */

		theta = atan2 (-x,-adj_y);
		rho = -rho;
	}
	if (fabs (theta) > edcnc->max_theta)
	{
		/* The coordinate is in the pie slice behind the central
		   meridian. */

		rtn_val = cs_CNVRT_RNG;
		theta = (theta > 0.0) ? edcnc->max_theta : -edcnc->max_theta;
	}

	/* Longitude, n is never zero. */

	del_lng = theta / edcnc->n;

	/* Now we can turn to the latitude.  The sphere
	   and the ellipsoid require different calculations. */
   
	if (edcnc->ecent == 0.0)
	{
		/* Here for the sphere. */

		lat = (edcnc->G - (rho / edcnc->ka));
	}
	else
	{
		/* Here for the ellisoid.  Since the M value which we
		   will convert to geographic latitude needs some
		   special attention, we use the CSmuIcal function
		   instead of the CSmmIcal function as one might
		   expect. */

		M = edcnc->kaG - rho;
		lat = CSmmIcal (&edcnc->mmcofI,M);
	}

	ll [LNG] = (del_lng + edcnc->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSedcncC (edcnc,ll);
**
**	struct cs_Edcnc_ *edcnc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Equidistant Conic
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

double EXP_LVL9 CSedcncC (Const struct cs_Edcnc_ *edcnc,Const double ll [2])
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
	status = CSedcncF (edcnc,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSedcncF (edcnc,xy2,my_ll);
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
**	k = CSedcncK (edcnc,ll);
**
**	struct cs_Edcnc_ *edcnc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Equidistant Conic
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor along a meridian
**								of the projected coordinate system at the given
**								point.
**
**	The function returns the grid scale factor, along a parallel, of
**	the projection at the point specified.
**
**	Normal conventions and CS-MAP standards have NOT been followed
**	in codng this function so that the variable nomenclature could
**	adhere as closely as possible to that used in the reference.
**
**	The cs_Edcnc_ structure must be setup properly, CSedcncS can do
**	this for you.
**********************************************************************/

double EXP_LVL9 CSedcncK (Const struct cs_Edcnc_ *edcnc,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole, in readians. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale factors. */

	double lat;
	double k;

	double sin_lat;
	double cos_lat;
	double M;
	double m;
	double tmp;

	lat = ll [LAT] * cs_Degree;

	/* At either pole, the scale is infinite, and the mathemagics
	   below have a big problem. */

	if (fabs (lat) >= cs_NPTest)
	{
		return (cs_SclInf);
	}

	/* We should be safe from floating point exceptions now. */

	sin_lat = sin (lat);
	cos_lat = cos (lat);

	if (edcnc->ecent == 0.0)
	{
		/* Here for the sphere. */

		k = (edcnc->G - lat) * edcnc->n / cos_lat;
	}
	else
	{
		/* Here for the ellipsoid. */

		M = CSmmFcal (&edcnc->mmcofF,lat,sin_lat,cos_lat);
		tmp = edcnc->e_sq * sin_lat * sin_lat;

		/* If cos_lat is never zero, m should never be zero. */

		m = cos_lat / sqrt (cs_One - tmp);
		k = (edcnc->G - M / edcnc->ka) * (edcnc->n / m);
	}
	return (k);
}

/**********************************************************************
**	k = CSedcnch (edcnc,ll);
**
**	struct cs_Edcnc_ *edcnc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert projection.
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
**	Normal conventions and CS-MAP standards have NOT been followed
**	in codng this function so that the variable nomenclature could
**	adhere as closely as possible to that used in the reference.
**
**	The cs_Edcnc_ structure must be setup properly, CSedcncS can do
**	this for you.
**********************************************************************/

double EXP_LVL9 CSedcncH (Const struct cs_Edcnc_ *edcnc,Const double ll [2])
{
	extern double cs_One;			/* 1.0 */

	return (cs_One);
}						/*lint !e715 */

/**********************************************************************
**	status = CSedcncL (edcnc,cnt,pnts);
**
**	struct cs_Edcnc_ *edcnc;	the coordinate system against which the check is
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

int EXP_LVL9 CSedcncL (Const struct cs_Edcnc_ *edcnc,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi;			/*  PI */
	extern double cs_Pi_o_2;		/* PI/2 */

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
		if (fabs (tmp) > cs_Pi_o_2)
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
	del_lng1 = CS_adj2pi (tmp - edcnc->org_lng);
	sgn_lng1 = (del_lng1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Does the line cross the back azimuth of the
		   central meridian??? */

		tmp = pnts [ii][LNG] * cs_Degree;
		del_lng2 = CS_adj2pi (tmp - edcnc->org_lng);
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
**	status = CSedcncX (edcnc,cnt,pnts);
**
**	struct cs_Edcnc_ *edcnc;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSedcncL to check lat/long
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

static int CSedcncXP (	Const struct cs_Edcnc_ *edcnc,Const double xy [2],double *theta)
{
	double xx,yy;
	double adj_yy;
	double rho;

	/* Convert to polar coordinates. */

	if (edcnc->quad == 0)
	{
		xx = xy [XX] - edcnc->x_off;
		yy = xy [YY] - edcnc->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,edcnc->x_off,edcnc->y_off,
						  edcnc->quad);
	}

	adj_yy = edcnc->rho0 - yy;
	rho = sqrt (xx * xx + adj_yy * adj_yy);
	if (rho < edcnc->min_rho || rho > edcnc->max_rho)
	{
		return (cs_CNVRT_DOMN);
	}
	if (edcnc->n > 0.0)
	{
		*theta = atan2 (xx,adj_yy);
	}
	else
	{
		*theta = atan2 (-xx,-adj_yy);
	}
	if (fabs (*theta) > edcnc->max_theta)
	{
		return (cs_CNVRT_DOMN);
	}
	return (cs_CNVRT_OK);
}

int EXP_LVL9 CSedcncX (Const struct cs_Edcnc_ *edcnc,int cnt,Const double pnts [][3])
{
	extern double cs_Pi;			/* 3.14159 */

	int ii;
	int status;
	int sgn_theta1;
	int sgn_theta2;

	double tmp;
	double theta1, theta2;

	status = CSedcncXP (edcnc,pnts [0],&theta1);
	if (status != cs_CNVRT_OK) return (status);
	sgn_theta1 = (theta1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Check the current point. */

		status = CSedcncXP (edcnc,pnts [ii],&theta2);
		if (status != cs_CNVRT_OK) break;

		sgn_theta2 = (theta2 < 0.0);
		if (sgn_theta1 != sgn_theta2)
		{
			/* Segment crosses the central meridian
			   or the back azimuth thereof. Which is
			   it? */

			tmp = fabs (theta1) + fabs (theta2);
			if (tmp > cs_Pi)
			{
				/* Opps!!! Crosses the back
				   azimuth. */

				status = cs_CNVRT_DOMN;
				break;
			}
		}

		/* Set up for the next segment. */

		theta1 = theta2;
		sgn_theta1 = sgn_theta2;
	}
	return (status);
}
