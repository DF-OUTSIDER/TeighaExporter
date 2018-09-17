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
/*lint -esym(534,CSbonneF)  ignoring return value */

/**********************************************************************
**	err_cnt = CSbonneQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSbonneQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Bonne specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		/* An org_lat of +-90 yeilds the Werner Projection. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	/* That's it for Bonne. */
	
	return (err_cnt + 1);
}
/**********************************************************************
**	(void) CSbonneS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate system
**								parameters.
**
**********************************************************************/

void EXP_LVL9 CSbonneS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/* 57.2... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_Huge;				/* 1.0E+38 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	struct cs_Bonne_ *bonne;

	double sin_org_lat;
	double cos_org_lat;

	double tmp1;

	double test_ll [3];
	double test_xy [2];

	bonne = &csprm->proj_prms.bonne;

	bonne->org_lng = csprm->csdef.org_lng * cs_Degree;
	bonne->org_lat = csprm->csdef.org_lat * cs_Degree;
	bonne->x_off = csprm->csdef.x_off;
	bonne->y_off = csprm->csdef.y_off;
	bonne->k = csprm->csdef.scale;
	bonne->ecent = csprm->datum.ecent;
	bonne->e_sq = bonne->ecent * bonne->ecent;
	bonne->e_rad = csprm->datum.e_rad;
	bonne->ka = bonne->e_rad * bonne->k;
	bonne->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* There are two limiting cases.  If the origin latitude
	   is either pole, we have the Werner projection, and
	   some special code is required to prevent divide by
	   zeros.

	   If the origin latitude is zero, the mathemagics for this
	   projection blow up, but the projection approaches the
	   Sinusoidal. */

	bonne->mode = cs_BONNE_NRML;
	if	 (fabs (bonne->org_lat) < cs_AnglTest)
	{
		bonne->mode = cs_BONNE_SINUS;
	}
	else if (fabs (bonne->org_lat) > cs_NPTest)
	{
		bonne->mode = cs_BONNE_WERNER;
	}
	bonne->south = (short)(bonne->org_lat < cs_Zero);

	if (bonne->ecent == 0.0)
	{
		/* Here for a spherical datum. */

		if (bonne->mode == cs_BONNE_NRML)
		{
			bonne->cot_lat1 = cs_One / tan (bonne->org_lat);
		}
		else
		{
			bonne->cot_lat1 = cs_Zero;
		}
		bonne->cot_lat1_lat1 = bonne->cot_lat1 + bonne->org_lat;
		bonne->Rcot_lat1 = bonne->ka * bonne->cot_lat1;
		bonne->small_xy = cs_AnglTest * bonne->ka;
	}
	else
	{
		sin_org_lat = sin (bonne->org_lat);
		bonne->sin_org_lat = sin_org_lat;
		cos_org_lat = cos (bonne->org_lat);

		/* Compute m for the origin latitude. */

		tmp1 = cs_One - bonne->e_sq * sin_org_lat * sin_org_lat;
		bonne->m1 = cos_org_lat / sqrt (tmp1);

		/* Generate the forward and inverse mu series
		   coefficients.  Note that we use the special
		   eccentricity term computed above for the
		   Inverse coefficients. */

		CSmmFsu (&bonne->mmcofF,bonne->ka,bonne->e_sq);
		CSmmIsu (&bonne->mmcofI,bonne->ka,bonne->e_sq);

		/* M1 is the distance, in scaled coordinate system units,
		   from the equator to the origin latitude. */

		bonne->M1 = CSmmFcal (&bonne->mmcofF,bonne->org_lat,
						     sin_org_lat,
						     cos_org_lat);

		/* mode is cs_BONNE_SINUS in the case of an origin
		   latitude at the equator, which would produce a
		   divide by zero here. */

		if (bonne->mode != cs_BONNE_SINUS)
		{
			bonne->kam1term = bonne->ka * bonne->m1 / sin_org_lat;
			bonne->kam1Mterm = bonne->kam1term + bonne->M1;
		}
		else
		{
			bonne->kam1term = cs_Huge;
			bonne->kam1Mterm = bonne->kam1term + bonne->M1;
		}

		/* Tmp1 is the polar radius, small_xy becomes the
		   cartesian quivalent of cs_AnglTest at the poles. */

		tmp1 = sqrt (cs_One - bonne->e_sq);
		bonne->small_xy = cs_AnglTest * bonne->ka * tmp1;
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = bonne->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km90;
		csprm->max_ll [LNG] = cs_K90;
		switch (bonne->mode) {

		default:
		case cs_BONNE_NRML:
		case cs_BONNE_WERNER:

			if (bonne->org_lat >= 0.0)
			{
				csprm->min_ll [LAT] = cs_Zero;
				csprm->max_ll [LAT] = cs_K90;
			}
			else
			{
				csprm->min_ll [LAT] = cs_Km90;
				csprm->max_ll [LAT] = cs_Zero;
			}
			break;

		case cs_BONNE_SINUS:

			csprm->min_ll [LAT] = cs_Km90;
			csprm->max_ll [LAT] = cs_K90;
			break;
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
		   The setup is virtually complete, so we can use CSbonneF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = csprm->max_ll [LAT];
		CSbonneF (bonne,test_xy,test_ll);
		csprm->max_xy [YY] = test_xy [YY] - bonne->y_off;

		test_ll [LAT] = csprm->min_ll [LAT];
		CSbonneF (bonne,test_xy,test_ll);
		csprm->min_xy [YY] = test_xy [YY] - bonne->y_off;

		test_ll [LNG] = csprm->cent_mer + cs_K90;
		test_ll [LAT] = cs_Zero;
		CSbonneF (bonne,test_xy,test_ll);
		csprm->max_xy [XX] = test_xy [XX] - bonne->x_off;
		csprm->min_xy [XX] = -csprm->max_xy [XX];

		CS_quadMM (csprm->min_xy,csprm->max_xy,bonne->x_off,
						       bonne->y_off,
						       bonne->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSbonneF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSbonneI;
	csprm->cs_scale = (cs_SCALE_CAST)CSbonneH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSbonneK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSbonneH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSbonneC;
	csprm->llchk    = (cs_LLCHK_CAST)CSbonneL;
	csprm->xychk    = (cs_XYCHK_CAST)CSbonneX;

	return;
}

/**********************************************************************
**	rtn_val = CSbonneF (bonne,xy,ll);
**
**	struct cs_Bonne_ *bonne;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]) values
**								to be converted, in degrees.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and the xy arrays may share the same memory
**	with no adverse affects.
**********************************************************************/

int EXP_LVL9 CSbonneF (Const struct cs_Bonne_ *bonne,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/* PI, 3.14159... */
	extern double cs_Mpi;				/* -PI, -3.14159... */
	extern double cs_Two_pi;			/* 2 PI */
	extern double cs_Pi_o_2;			/* PI over 2 */
	extern double cs_Mpi_o_2;			/* -PI over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole. */

	int rtn_val;

	double lng;
	double lat;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double rho;
	double mm;
	double E;
	double M;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	lng = ll [0] * cs_Degree;
	lat = ll [1] * cs_Degree;

	/* Force both input values to appropriate values. */

	if (fabs (lat) > cs_Pi_o_2)
	{
	   	rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}
	if (fabs (lat) > cs_NPTest)
	{
		if (rtn_val == cs_CNVRT_NRML) rtn_val = cs_CNVRT_INDF;
		lat = (lat >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
		cos_lat = cs_Zero;
	}
	else
	{
		cos_lat = cos (lat);
	}

	del_lng = lng - bonne->org_lng;
	if      (del_lng > cs_Pi  && bonne->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && bonne->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}

	/* Calculate the polar components: rho is the radius of the
	   arc for the given latitude; E is the polar angle; also
	   the convergence angle. */

	if (bonne->ecent == 0.0)
	{
		/* Here for a sphere. */

		switch (bonne->mode) {

		case cs_BONNE_NRML:
		default:
			/* In this case, lat1 is neither the pole or
			   the equator.  As lat1 increases, cot_lat1
			   decreases; thus assuring that rho is never
			   zero. */

			rho = bonne->ka * (bonne->cot_lat1_lat1 - lat);
			E = bonne->ka * del_lng * cos_lat / rho;
			xy [XX] = rho * sin (E);
			xy [YY] = bonne->Rcot_lat1 - rho * cos (E);
			break;

		case cs_BONNE_WERNER:

			/* Lat1 is one of the poles. We need different
			   formulas. */

			rho = bonne->ka * (bonne->org_lat - lat);
			if (rho < bonne->small_xy)
			{
				/* Lat is very close to the focus pole. */

				xy [XX] = cs_Zero;
				xy [YY] = cs_Zero;
			}
			else
			{
				/* Rho is not zero, the following is safe. */

				E = bonne->ka * del_lng * cos_lat / rho;
				xy [XX] = rho * sin (E);
				xy [YY] = bonne->Rcot_lat1 - rho * cos (E);
			}
			break;

		case cs_BONNE_SINUS:

			/* Lat1 is the equator, we have different formulas,
			   the same as the Sinusoidal. */

			xy [XX] = bonne->ka * del_lng * cos_lat;
			xy [YY] = bonne->ka * lat;
			break;
		}
	}
	else
	{
		/* Here for the ellipsoid. */

		sin_lat = sin (lat);
		M = CSmmFcal (&bonne->mmcofF,lat,sin_lat,cos_lat);
		tmp1 = cs_One - (bonne->e_sq * sin_lat * sin_lat);
		mm = cos_lat / sqrt (tmp1);

		switch (bonne->mode) {

		case cs_BONNE_NRML:
		case cs_BONNE_WERNER:
		default:

			/* Rho will be zero at the poles. */

			rho = bonne->kam1Mterm - M;
			if (fabs (rho) < bonne->small_xy)
			{
				/* E is essentially the polar angle. */

				E = cs_Zero;
			}
			else
			{
				E = bonne->ka * mm * del_lng / rho;
			}
			xy [XX] = rho * sin (E);
			xy [YY] = bonne->kam1term - (rho * cos (E));
			break;

		case cs_BONNE_SINUS:

			xy [XX] = bonne->ka * del_lng * mm;
			xy [YY] = M;
			break;
		}
	}

	/* Add the false easting and northing. */

	if (bonne->quad == 0)
	{
		xy [XX] += bonne->x_off;
		xy [YY] += bonne->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],bonne->x_off,bonne->y_off,
							  bonne->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSbonneI (bonne,ll,xy);
**
**	struct cs_Bonne_ *bonne;	structure which defines the parameters
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
**	1984, pages 139-140.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSbonneI (Const struct cs_Bonne_ *bonne,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Pi_o_2;			/*  PI / 2.0 */
	extern double cs_Mpi_o_2;			/* -PI / 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double xx;
	double yy;

	double M;
	double mm;
	double rho;
	double lat;
	double sin_lat;
	double cos_lat;
	double del_lng;
	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false easting and northing. */

	if (bonne->quad == 0)
	{
		xx = xy [XX] - bonne->x_off;
		yy = xy [YY] - bonne->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,bonne->x_off,bonne->y_off,bonne->quad);
	}

	del_lng = cs_Zero;
	if (bonne->ecent == 0.0)
	{
		/* Here for the shpere. */

		switch (bonne->mode) {

		case cs_BONNE_NRML:
		case cs_BONNE_WERNER:
		default:
			/* Here if lat1 is something other than the
			   equator, thus Rcot_lat1 is finite. */

			tmp1 = bonne->Rcot_lat1 - yy;
			rho = sqrt (xx * xx + tmp1 * tmp1);

			/* Adjust for a projection centered in the southern
			   hemisphere. */

			if (bonne->south) rho = -rho;

			lat = bonne->cot_lat1_lat1 - (rho / bonne->ka);
			if (fabs (lat) > cs_NPTest)
			{
				/* Special code for the poles. */

				rtn_val = cs_CNVRT_INDF;
				if (fabs (lat) > cs_Pi_o_2)
				{
					/* Opps!!! Y was north of the north
					   pole, or south of the south
					   pole. */

					rtn_val = cs_CNVRT_RNG;
					lat = (lat >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
					cos_lat = cs_Zero;
				}
			}
			else
			{
				/* Latitude in normal range. */

				if (bonne->south)
				{
					tmp1 = atan2 (-xx,-tmp1);
				}
				else
				{
					tmp1 = atan2 (xx,tmp1);
				}
				cos_lat = cos (lat);
				del_lng = rho * tmp1 / (bonne->ka * cos_lat);
			}
			break;

		case cs_BONNE_SINUS:

			/* Lat1 is the equator here. */

			lat = yy / bonne->ka;

			/* Need special code for the poles. */

			if (fabs (lat) > cs_NPTest)
			{
    				rtn_val = cs_CNVRT_INDF;
				if (fabs (lat) > cs_Pi_o_2)
				{
					/* Opps!!! Y was north of the
					   north pole, or south of the
					   south pole. */

					rtn_val = cs_CNVRT_RNG;
					lat = (lat >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
				}

				/* Lat is now at one of the poles, and
				   cos_lat will be zero. */
			}
			else
			{
				/* Latitude is in the normal range. */

				cos_lat = cos (lat);
				del_lng = xx / (bonne->ka * cos_lat);
				break;
			}
		}
	}
	else
	{
	    	/* Here for the ellipsoid. */

		switch (bonne->mode) {

		case cs_BONNE_NRML:
		case cs_BONNE_WERNER:
		default:

			tmp1 = bonne->kam1term - yy;		/* delta Y */
			rho = sqrt (xx * xx + tmp1 * tmp1);

			if (bonne->south) rho = -rho;

			M = bonne->kam1Mterm - rho;
			lat = CSmmIcal (&bonne->mmcofI,M);

			if (fabs (lat) > cs_NPTest)
			{
    				rtn_val = cs_CNVRT_INDF;
				if (fabs (lat) > cs_Pi_o_2)
				{
					/* Opps!! Y is north of the north
					   pole, or south of the south
					   pole. */

					rtn_val = cs_CNVRT_RNG;
					lat = (lat >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
				}
			}
			else
			{
				/* If rho were zero, then latitude would
				   be a pole, a situation handled above.
				   Thus, we know that at least one of
				   xx and tmp1 is non-zero, and it is safe
				   to call atan2. */

				if (bonne->south)
				{
					tmp1 = atan2 (-xx,-tmp1);
				}
				else
				{
					tmp1 = atan2 (xx,tmp1);
				}
				sin_lat = sin (lat);
				cos_lat = cos (lat);
				tmp2 = cs_One - bonne->e_sq * sin_lat * sin_lat;
				mm = cos_lat / sqrt (tmp2);
				del_lng = rho * tmp1 / (bonne->ka * mm);
			}
			break;

		case cs_BONNE_SINUS:

			lat = CSmmIcal (&bonne->mmcofI,yy);
			if (fabs (lat) > cs_NPTest)
			{
				/* This case should be very rare, we switch
				   optimization to size, rather than speed. */

    				rtn_val = cs_CNVRT_INDF;
				if (fabs (lat) > cs_Pi_o_2)
				{
					rtn_val = cs_CNVRT_RNG;
					lat = (lat >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
				}
			}
			else
			{
				/* Cos_lat can't be zero. */

				cos_lat = cos (lat);
				sin_lat = sin (lat);
				tmp1 = cs_One - (bonne->e_sq * sin_lat * sin_lat);
				del_lng = (xx * sqrt (tmp1)) / (bonne->ka * cos_lat);
			}
			break;
		}
	}

	/* Convert to degrees. */

	if (fabs (del_lng) > cs_Pi)
	{
		del_lng = CS_adj2pi (del_lng);
	    	rtn_val = cs_CNVRT_RNG;
	}
	ll [LNG] = (del_lng + bonne->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	/* That's it. */

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSbonneC (bonne,ll);
**
**	struct cs_Bonne_ *bonne;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Bonne projection.
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

double EXP_LVL9 CSbonneC (Const struct cs_Bonne_ *bonne,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;					/* some folks call this alpha */

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
	status = CSbonneF (bonne,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSbonneF (bonne,xy2,my_ll);
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
**	k = CSbonneK (bonne,ll);
**
**	struct cs_Bonne_ *bonne;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Bonne projection.
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

double EXP_LVL9 CSbonneK (Const struct cs_Bonne_ *bonne,Const double ll [2])
{
	extern double cs_One;			/* 1.0 */

	return (cs_One);
}									/*lint !e715 */

/**********************************************************************
**	k = CSbonneH (bonne,ll);
**
**	struct cs_Bonne_ *bonne;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Bonne projection.
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

double EXP_LVL9 CSbonneH (Const struct cs_Bonne_ *bonne,Const double ll [2])
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_MoloK;				/* Sine of one arc second */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */

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
	status = CSbonneF (bonne,xy1,ll1);
	if (status != cs_CNVRT_OK)
	{
		return (cs_SclInf);
	}

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + (0.5 / 3600.0);
	status = CSbonneF (bonne,xy2,ll2);
	if (status != cs_CNVRT_OK)
	{
		return (cs_SclInf);
	}

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the actual distance between the two points. */

	if (bonne->ecent == cs_Zero)			/*lint !e777 */
	{
		/* Here if we are on a sphere.  Compute the angular
		   distance.  Keep in mind that delta longitude is
		   zero for this case, delta latitude is fixed at one
		   arc second, and the sine of a small angle like
		   one arc second is the same as the angle (in radians).
		   cs_MoloK is the sine of one arc second. */

		ll_dd = bonne->ka * cs_MoloK;
	}
	else
	{
		/* Calculate the geodetic distance between the two
		   lat/long points on the ellipsoid.  Note, we provide
		   the geodetic calculator with the scaled radius of
		   the earth so that the distance it computes will be
		   in the same units as the X and Y coordinates. */

		CS_llazdd (bonne->ka,bonne->e_sq,ll1,ll2,&ll_dd);
	}

	/* LL_dd should never be zero, but just in case. */

	if ((ll_dd * cs_SclInf) <= xy_dd)
	{
		hh = cs_SclInf;
	}
	else
	{
		hh = xy_dd / ll_dd;
	}
	return (hh);
}

/**********************************************************************
**	status = CSbonneL (bonne,cnt,pnts);
**
**	struct cs_Bonne_ *bonne;	the coordinate system against which the
**								check is to be performed.
**	int cnt;	the number of points in the region to be checked.
**	double pnts [][2];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line,
**								or region is completely within the
**								domain of the coordinate system.
**								Otherwise, cs_CNVRT_DOMN is returned.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**
**	Essentially, all points in lat/long space (normalized) are
**	valid.  Any line, which dies not cross the back side of the
**	central meridian is valid. Any region which consists of
**	valid lines is valid.
**********************************************************************/

int EXP_LVL9 CSbonneL (Const struct cs_Bonne_ *bonne,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi;			/* 3.14159....  */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int status;
	int last_sgn = 0;				/* initialized to keep the gcc compiler happy */

	double lat;
	double lng;
	double tmp;
	double del_lng;
	double last_lng = 0.0;			/* initialized to keep the gcc compiler happy */

	/* Loop, checking all points.  If the point is not the first,
	   we see if the line formed by connecting to the previous
	   point crosses the back side of the origin longitude.  If
	   so, the line and.or region is invalid.  Note, the latitude
	   of any point, once determined to be valid, does not enter
	   into the determination of valid lines and/or regions. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		/* Deal with the lataitude. */

		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}

		/* Check the longitude value. */

		lng = pnts [ii][LAT] * cs_Degree;
		del_lng = lng - bonne->org_lng;
		if (fabs (del_lng) > cs_Pi)
		{
			status = cs_CNVRT_DOMN;
			break;
		}

		/* Check the line/region for validity. */

		if (ii != 0)
		{
			if ((del_lng < 0.0) ^ last_sgn)		/*lint !e514 !e530 */
			{
				/* Need to check further only if the
				   signs are different. */

				tmp = fabs (del_lng) + fabs (last_lng);		/*lint !e530 */
				if (tmp > cs_Pi)
				{
					status = cs_CNVRT_DOMN;
					break;
				}
			}
		}
		last_lng = del_lng;
		last_sgn = (last_lng < 0.0);
	}
	return (status);
}

/**********************************************************************
**	status = CSbonneX (bonne,cnt,pnts);
**
**	struct cs_Bonne_ *bonne;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points
**								which define a point, line, or region
**								to be checked.
**	int status;					returns the status of the check; see
**								remarks below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSbonneL to check lat/long
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

int EXP_LVL9 CSbonneX (Const struct cs_Bonne_ *bonne,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.27..... */
	extern double cs_Pi;			/* 3.14159 */

	int ii;
	int status;
	int rtn_val;
	int last_sgn = 0;				/* initialized to keep the gcc compiler happy */

	double tmp;
	double del_lng;
	double last_lng = 0.0;			/* initialized to keep the gcc compiler happy */
	double ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Checking a cartesian coordinate for validity is not that
	   much simpler than actually converting the point.  So, to
	   avoid duplicating a lot of code, that's what we'll do.

	   Any line which is defined by valid points is legitimate,
	   unless the line crosses the back side of the central
	   meridian. Further, any region which consists solely
	   of valid lines is also valid.

	   Thus, we loop, using the CSbonneI function for each point.
	   We use the resulting longitudes to determine if the line
	   segment crosses the back side of the central meridian. */

	for (ii = 1;ii < cnt;ii++)
	{
		/* Check the current point. */

		status = CSbonneI (bonne,ll,pnts [ii]);
		if (status != cs_CNVRT_OK)
		{
			/* A bogus point means everything is invalid,
			   no need to check any further. */

			rtn_val = cs_CNVRT_DOMN;
			break;
		}

		del_lng = ll [LNG] * cs_Degree - bonne->org_lng;
		if (ii != 0)
		{
			if ((del_lng < 0.0) ^ last_sgn)		/*lint !e514 !e530 */
			{
				/* Signs are different, worth checking
				   further. */

				tmp = fabs (del_lng) + fabs (last_lng);		/*lint !e530 */
				if (tmp > cs_Pi)
				{
					rtn_val = cs_CNVRT_DOMN;
					break;
				}
			}
		}
		last_lng = del_lng;
		last_sgn = (del_lng < 0.0);
	}
	return (rtn_val);
}

