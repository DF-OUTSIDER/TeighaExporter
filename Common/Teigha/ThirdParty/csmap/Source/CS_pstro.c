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
**	err_cnt = CSpstroQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSpstroQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_Zero;			/*  0.0 */
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_K90;			/*  90.0 */
	extern double cs_Km90;			/* -90.0 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Sterographic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (!CS_cmpDbls (cs_def->org_lat,cs_K90) && !CS_cmpDbls (cs_def->org_lat,cs_Km90))
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLRLAT;
	}

	if (prj_code == cs_PRJCOD_PSTRO)
	{
		/* Scale reduction. */
		if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
		}
	}
	else if (prj_code == cs_PRJCOD_PSTROSL)
	{
		if (CS_cmpDbls (cs_def->org_lat,cs_K90))
		{
			if (cs_def->prj_prm1 >= cs_K90 || cs_def->prj_prm1 <= cs_Zero)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDLAT;
			}
		}
		else
		{
			if (cs_def->prj_prm1 <= cs_Km90 || cs_def->prj_prm1 >= cs_Zero)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDLAT;
			}
		}
	}

	/* That's it for Polar Sterographic. */

	return (err_cnt + 1);
}

/**********************************************************************
**	CSpstroS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSpstroS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/*   57.29....  */
	extern double cs_Pi_o_2;			/*  PI / 2.0     */
	extern double cs_Pi_o_4;			/*  PI / 4.0     */
	extern double cs_Mpi_o_2;			/* -PI / 2.0     */
	extern double cs_Zero;				/*    0.0 */
	extern double cs_Half;				/*    0.5 */
	extern double cs_One;				/*    1.0 */
	extern double cs_Mone;				/*   -1.0 */
	extern double cs_Two;				/*    2.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */

	struct cs_Pstro_ *pstro;

	double latc;
	double sinlatc,	coslatc, esinlatc;
	double onePlusE, oneMinusE;
	double mc, tc, eTerm;
	double scaleFactor;
	double tmp1, tmp2;

	pstro = &csprm->proj_prms.pstro;

	/* Transfer the necessary arguments to the "pstro" structure. */

	pstro->org_lng = csprm->csdef.org_lng * cs_Degree;
	pstro->org_lat = csprm->csdef.org_lat * cs_Degree;
	pstro->x_off = csprm->csdef.x_off;
	pstro->y_off = csprm->csdef.y_off;
	pstro->ecent = csprm->datum.ecent;
	pstro->e_rad = csprm->datum.e_rad;

	// There are two variations of this projection.  In the traditional form,
	// the scale factor is specified explicitly.  In the other, a "Standard
	// Circle" is supplied, and the scale factor is computed from this.
	if (csprm->prj_code == cs_PRJCOD_PSTRO)
	{
		scaleFactor = csprm->csdef.scl_red;
	}
	else if (csprm->prj_code == cs_PRJCOD_PSTROSL)
	{
		latc = csprm->csdef.prj_prm1 * cs_Degree;
		if (pstro->org_lat < 0)
		{
			// South pole variant. The formulas below compute the scale from the
			// north pole perspective. The latc variable is not used after that.
			// So we reverse the sign here.
			latc = -latc;
		}

		if (pstro->ecent == 0.0)
		{
			// Here for a sphere.
			sinlatc = sin (latc);
			scaleFactor = cs_Half * (cs_One + sinlatc);
		}
		else
		{
			// Here for an ellipsoid.
			sinlatc = sin (latc);
			coslatc = cos (latc);
			esinlatc = pstro->ecent * sinlatc;
			mc = coslatc / sqrt (cs_One - esinlatc * esinlatc);
			tmp1 = (cs_One - esinlatc) / (cs_One + esinlatc);
			tmp1 = pow (tmp1,pstro->ecent * cs_Half);
			tc = tan (cs_Pi_o_4 - latc * cs_Half) / tmp1;

			// The 1+e and 1-e terms.
			onePlusE = cs_One + pstro->ecent;
			oneMinusE = cs_One - pstro->ecent;
			eTerm = sqrt (pow (onePlusE,onePlusE) * pow (oneMinusE,oneMinusE));

			// The scale factor at the pole.
			scaleFactor = cs_Half * (mc / tc) * eTerm;
		}
	}
	else
	{
		scaleFactor = cs_One;
	}

	// The rest is common to both variations.
	pstro->k = csprm->csdef.scale * scaleFactor;
	pstro->ka = csprm->datum.e_rad * pstro->k;
	pstro->two_ka = pstro->ka * cs_Two;
	pstro->cos_org_lat = cos (pstro->org_lat);
	pstro->sin_org_lat = sin (pstro->org_lat);
	pstro->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Determine which pole. */

	if (fabs (pstro->org_lat) > cs_NPTest)
	{
		pstro->cos_org_lat = cs_Zero;
		if (pstro->org_lat < 0.0)
		{
			/* South polar aspect. */

			pstro->aspect = cs_STERO_SOUTH;
			pstro->sin_org_lat = cs_Mone;
			pstro->org_lat = cs_Mpi_o_2;
		}
		else
		{
			/* North polar aspect. */

			pstro->aspect = cs_STERO_NORTH;
			pstro->sin_org_lat = cs_One;
			pstro->org_lat = cs_Pi_o_2;
		}
	}

	/* Set up a value in the units of the cartesian system
	   which represents one millimeter. */

	pstro->one_mm = 0.001 * csprm->csdef.scale;

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */

	if (pstro->ecent == 0.0)
	{
		/* Not much we can do here. */

		pstro->two_k0 = cs_Two * csprm->csdef.scl_red;
	}
	else
	{
		/* Here for the ellipsoidal calculations. */

		pstro->e_o_2 = csprm->datum.ecent * cs_Half;
		pstro->e_sq = pstro->ecent * pstro->ecent;

		tmp1 = cs_One + pstro->ecent;
		tmp1 = pow (tmp1,tmp1);
		tmp2 = cs_One - pstro->ecent;
		tmp2 = pow (tmp2,tmp2);
		pstro->e_term = sqrt (tmp1 * tmp2);

		CSchiIsu (&pstro->chicofI,pstro->e_sq);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = pstro->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to
		   establish the useful range.  We'll establish some
		   pretty liberal values. */

		switch (pstro->aspect) {

		case cs_STERO_NORTH:

			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_One;
			csprm->max_ll [LAT] = cs_K90;
			break;

		case cs_STERO_SOUTH:

			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_Km90;
			csprm->max_ll [LAT] = cs_Mone;
			break;
		}							/*lint !e744 */
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
		/* No specification in the coordinate system definition. */

		csprm->min_xy [XX] = -pstro->ka;
		csprm->max_xy [XX] =  pstro->ka;
		csprm->min_xy [YY] = -pstro->ka;
		csprm->max_xy [YY] =  pstro->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,pstro->x_off,
											   pstro->y_off,
											   pstro->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSpstroF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSpstroI;
	csprm->cs_scale = (cs_SCALE_CAST)CSpstroK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSpstroK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSpstroK;	/* Conformal */
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSpstroC;
	csprm->llchk    = (cs_LLCHK_CAST)CSpstroL;
	csprm->xychk    = (cs_XYCHK_CAST)CSpstroX;

	return;
}

/**********************************************************************
**	rtn_val = CSpstroF (pstro,xy,ll);
**
**	struct cs_Pstro_ *pstro;	structure which defines the parameters
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
**	This function calculates the Polar Sterographic
**	projection and returns values in the coordinate system
**	described by the cs_Pstro_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 154-163.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSpstroF (Const struct cs_Pstro_ *pstro,double xy [2],Const double ll [2])

{
	extern char csErrnam [MAXPATH];

	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Pi_o_2;			/* PI over 2 */
	extern double cs_Pi_o_4;			/* PI over 4 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_Huge;				/* Huge value, but not too huge. */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double x;
	double y;

	double del_lng;
	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;

	double t;
	double rho;
	double tmp1;
	double tmp2;
	double tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and one for the
	   ellipsoid.  If the ecentricity of the dataum in use is 0.0
	   exactly, we shall use the spherical formulae. */

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
	
	/* We first adjust the following so that we can use much
	   of the code below for the southern polar aspect without
	   duplicating it with minor sign changes. */

	if (pstro->aspect == cs_STERO_SOUTH && pstro->ecent != 0.0)
	{
		lat = -lat;
		del_lng = pstro->org_lng - lng;
	}
	else
	{
		del_lng = lng - pstro->org_lng;
	}
	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);

	/* See if the datum is a sphere or an ellipsoid. */

	if (pstro->ecent == 0.0)
	{
		/* Here for the sphere. */

		switch (pstro->aspect) {

		case cs_STERO_NORTH:

			tmp1 = cs_Pi_o_4 - (lat * cs_Half);
			tmp1 = pstro->two_ka * tan (tmp1);

			x = tmp1 * sin_del_lng;
			y = -tmp1 * cos_del_lng;
			break;

		case cs_STERO_SOUTH:

			tmp1 = cs_Pi_o_4 + (lat * cs_Half);
			tmp1 = pstro->two_ka * tan (tmp1);

			x = tmp1 * sin_del_lng;
			y = tmp1 * cos_del_lng;
			break;
	
		default:
			x = y = -cs_Huge;
			CS_stncp (csErrnam,"CS_pstro:1",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}									/*lint !e744 */
	}
	else
	{
		/* Here for an ellipsoid model of the earth. */

		tmp1 = pstro->ecent * sin_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,pstro->e_o_2);
		tmp3 = lat * cs_Half;
		t       = tan (cs_Pi_o_4 - tmp3) / tmp2;

		switch (pstro->aspect) {

		case cs_STERO_NORTH:

			rho = pstro->two_ka * t / pstro->e_term;

			x = rho * sin_del_lng;
			y = -rho * cos_del_lng;
			break;

		case cs_STERO_SOUTH:

			rho = pstro->two_ka * t / pstro->e_term;

			x = -rho * sin_del_lng;
			y = rho * cos_del_lng;
			break;

		default:

			x = y = -cs_Huge;
			CS_stncp (csErrnam,"CS_pstro:1",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}									/*lint !e744 */
	}
	
	/* Adjust for the selected quadrant, and, finally, add false
	   easting and false northing. */

	xy [XX] = x;							/*lint !e644 */
	xy [YY] = y;							/*lint !e644 */

	if (pstro->quad == 0)
	{
		xy [XX] = x + pstro->x_off;
		xy [YY] = y + pstro->y_off;
	}
	else
	{
		CS_quadF (xy,x,y,pstro->x_off,pstro->y_off,pstro->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSpstroI (pstro,ll,xy);
**
**	struct cs_Pstro_ *pstro;	structure which defines the parameters
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
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 154-163.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSpstroI (Const struct cs_Pstro_ *pstro,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi_o_2;			/* PI over 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double x;
	double y;

	double c;
	double t;
	double chi;
	double rho;
	double cos_c;

	rtn_val = cs_CNVRT_NRML;

	/* Adjust for a non-standard quadrant. */

	if (pstro->quad == 0)
	{
		x = xy [XX] - pstro->x_off;
		y = xy [YY] - pstro->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,pstro->x_off,pstro->y_off,pstro->quad);
	}

	/* If x and y are now both zero, or very close to it,
	   we must just set the result to the origin of the
	   projection.  We don't want to divide by rho if it's
	   zero. */

	rho = sqrt (x * x + y * y);
	if (rho <= pstro->one_mm)
	{
		/* The coordinate is essentially zero.  Return
		   the origin of the coordinate system. */

		ll [LNG] = pstro->org_lng * cs_Radian;
		ll [LAT] = pstro->org_lat * cs_Radian;
		return (rtn_val);
	}
		
	/* Now we can convert back to lat/longs. */

	if (pstro->ecent == 0.0)
	{
		/* Here for the sphere.  Note, we have already
		   filtered out cases where rho is zero (or very
		   close to zero).

		   Note, rho can approach infinity, although considering
		   values greater than two_ka out of the domain would
		   be generally acceptable. Read c as the angular distance
		   from the origin. */

		c = cs_Two * atan (rho / pstro->two_ka);
		cos_c = cos (c);

		switch (pstro->aspect) {

		default:
		case cs_STERO_NORTH:

			ll [LNG] = pstro->org_lng + atan2 (x,-y);
			ll [LAT] = asin (cos_c);
			break;

		case cs_STERO_SOUTH:

			ll [LNG] = -pstro->org_lng + atan2 (-x,y);
			ll [LAT] = -asin (cos_c);
			break;
		}
	}
	else
	{
		/* Here for the ellisoid. */

		switch (pstro->aspect) {

		default:
		case cs_STERO_NORTH:

			ll [LNG] = pstro->org_lng + atan2 (x,-y);
			t = rho * pstro->e_term / pstro->two_ka;
			chi = cs_Pi_o_2 - (cs_Two * atan (t));
			ll [LAT] = CSchiIcal (&pstro->chicofI,chi);
			break;

		case cs_STERO_SOUTH:

			ll [LNG] = pstro->org_lng - atan2 (-x,y);
			t = rho * pstro->e_term / pstro->two_ka;
			chi = (cs_Two * atan (t)) - cs_Pi_o_2;
			ll [LAT] = CSchiIcal (&pstro->chicofI,chi);
			break;
		}
	}
	if (fabs (ll [LAT]) > cs_NPTest && rtn_val == cs_CNVRT_NRML)	/*lint !e774 */
	{
		rtn_val = cs_CNVRT_INDF;
	}

	/* Convert the results to degrees. */

	ll [LNG] *= cs_Radian;
	ll [LAT] *= cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSpstroC (pstro,ll);
**
**	struct cs_Pstro_ *pstro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Polar Sterographic
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double gamma;				returns the computed convergence angle for
**								the coordinate system as the specified
**								location in degrees east of north.
**
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

double EXP_LVL9 CSpstroC (Const struct cs_Pstro_ *pstro,Const double ll [2])
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

	/* We have no analytical formulas for any of the aspects/variations
	   supported. Therefore, we can handle all cases with the simple
	   empirical algorithm below. */

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
	status = CSpstroF (pstro,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSpstroF (pstro,xy2,my_ll);
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
	if ((fabs (del_xx) + fabs (del_yy)) >= pstro->one_mm)
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
**	kk = CSpstroK (pstro,ll);
**
**	struct cs_Pstro_ *pstro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Polar Sterographic
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	The Polar Sterographic projection is conformal, therefore, the
**	grid scale factor returned by this function is valid in
**	all directions.  Thus, there is no CSpstroH function.
**********************************************************************/

double EXP_LVL9 CSpstroK (Const struct cs_Pstro_ *pstro,Const double ll [2])
{
	extern double cs_Pi_o_2;			/*  PI / 2.0 */
	extern double cs_Pi_o_4;			/*  PI / 4.0 */
	extern double cs_Degree;			/*  1.0 / 57.29577... */
	extern double cs_Half;				/*  0.5 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc short
										   of the south pole in
										   radians. */

	double m;
	double t;
	double kk;
	double lng;
	double lat;
	double tmp1;
	double tmp2;
	double rho;
	double sin_lat;
	double cos_lat;
	double esin_lat;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}
	sin_lat = sin (lat);
	if (pstro->aspect == cs_STERO_NORTH)
	{
		if (lat < cs_SPTest)
		{
			return (cs_SclInf);
		}
	}
	else
	{
		if (lat > cs_NPTest)
		{
			return (cs_SclInf);
		}

		/* By revering the sign of the inputs we can use the
		   the same code for both aspects. */

		lng = -lng;
		lat = -lat;
	}
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	if (pstro->ecent == 0.0)
	{
		/* Here for the spherical case. */

		kk = pstro->two_k0 / (cs_One + sin_lat);
	}
	else
	{
		/* Here for the ellipsoid case. */

		esin_lat = pstro->ecent * sin_lat;
		tmp1 = (cs_One - esin_lat) / (cs_One + esin_lat);
		tmp1 = pow (tmp1,pstro->e_o_2);
		tmp2 = lat * cs_Half;
		t       = tan (cs_Pi_o_4 - tmp2) / tmp1;

		/* Careful, m is zero at the poles. */

		m = cos_lat / sqrt (cs_One - esin_lat * esin_lat);

		rho = pstro->two_ka * t / pstro->e_term;
		tmp1 = pstro->e_rad * m;
		if (tmp1 < pstro->one_mm)
		{
			kk = cs_Half * pstro->two_ka;
		}
		else
		{	
			kk = rho / tmp1;
		}
	}
	return (kk);
}

/**********************************************************************
**	status = CSpstroL (pstro,cnt,pnts);
**
**	struct cs_Pstro_ *pstro;	the coordinate system against which the check is
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

int EXP_LVL9 CSpstroL (Const struct cs_Pstro_ *pstro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSpstroF (pstro,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSpstroX (pstro,cnt,pnts);
**
**	struct cs_Pstro_ *pstro;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSpstroL to check lat/long
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

int EXP_LVL9 CSpstroX (Const struct cs_Pstro_ *pstro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* The domain is contiguous, there are no singularities
	   within the basic range of the projection. If all points
	   are within the domain, then all lines and regions will
	   be within the domain. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSpstroI (pstro,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
