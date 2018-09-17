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
**	err_cnt = CSazmedQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSazmedQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_ElevElMin;		/* -25000.0 */
	extern double cs_ElevElMax;		/*  50000.0 */

	int err_cnt;
	
	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Azimuthal general stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 >= cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_AZMTH;
	}

	if (prj_code == cs_PRJCOD_AZEDE)
	{
		if (cs_def->prj_prm2 < cs_ElevElMin || cs_def->prj_prm2 > cs_ElevElMax)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ELEVEL;
		}
	}

	/* That's it for the Lambert Azimuthal Equidistant projection. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSazmedS (csprm,form);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**	int form;					this variable indicates the form of the
**								azimuthal projection which is to be
**								used: cs_AZMED_POINT or cs_AZMED_ANGLE.
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
**
**********************************************************************/

void EXP_LVL9 CSazmedS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;		/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Pi;			/* 3.14159.... */
	extern double cs_Pi_o_2;		/*  PI / 2.0     */
	extern double cs_Mpi_o_2;		/*  PI / 2.0     */
	extern double cs_Radian;		/* 180.0 / pi */
	extern double cs_Degree;		/* 1.0 / RADIAN  */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Mone;			/* -1.0 */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc
									   in radians. */
	extern double cs_NPTest;		/* 0.001 seconds of arc
									   short of the north pole,
									   in radians. */
	extern double cs_SPTest;		/* 0.001 seconds of arc
									   short of the south pole,
									   in radians. */
	extern double cs_Km180;			/* -180.0 */
	extern double cs_K180;			/*  180.0 */
	extern double cs_Km90;			/* -90.0 */
	extern double cs_K90;			/*  90.0 */
	extern double cs_K60;			/*  60.0 */
	extern double cs_Ten;			/*  10.0 */

	struct cs_Azmed_ *azmed;

	double height;
	double tmp1;

	azmed = &csprm->proj_prms.azmed;

	height = cs_Zero;
	if (csprm->prj_code == cs_PRJCOD_AZEDE)
	{
		height = csprm->csdef.prj_prm2 * csprm->csdef.unit_scl;
	}

	/* Transfer the necessary arguments to the
	   "azmed" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	azmed->org_lng = csprm->csdef.org_lng * cs_Degree;
	azmed->org_lat = csprm->csdef.org_lat * cs_Degree;
	azmed->k = csprm->csdef.scale;
	azmed->x_off = csprm->csdef.x_off;
	azmed->y_off = csprm->csdef.y_off;
	azmed->ecent = csprm->datum.ecent;
	azmed->e_sq = azmed->ecent * azmed->ecent;
	azmed->one_esq = cs_One - azmed->e_sq;
	azmed->rt_one_esq = sqrt (azmed->one_esq);
	azmed->ka = (csprm->datum.e_rad + height) * azmed->k;
	azmed->two_ka = azmed->ka + azmed->ka;
	azmed->cos_org_lat = cos (azmed->org_lat);
	azmed->sin_org_lat = sin (azmed->org_lat);
	azmed->Az = csprm->csdef.prj_prm1 * cs_Degree;
	azmed->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
	azmed->aspect = 0;
	azmed->one_mm = 0.001 * csprm->csdef.scale;

	/* The following fix is required for the test cases from
	   Synder which use the unit sphere, and a sphere with
	   a radius of 3. */

	if (csprm->datum.e_rad <= 3.0)
	{
		azmed->one_mm = azmed->ka / 6.3E+09;
	}

	/* Compute the sine and cosine of the Y axis azimuth. These will
	   not change. */

	if (fabs (azmed->Az) < cs_AnglTest)
	{
		azmed->sin_Az = cs_Zero;
		azmed->cos_Az = cs_One;
	}
	else
	{
		azmed->sin_Az = sin (azmed->Az);
		azmed->cos_Az = cos (azmed->Az);
	}

	/* If we are very close to either a north or south polar
	   aspect, set the sine's and cosine's to their correct
	   values.  The sine and cosine functions tend to leave
	   us just a bit short on these numbers.  We also set the
	   polar flag, which is used in the sphereical inverse
	   equations (perhaps elsewhere later on).  The polar
	   variable is zero if the aspect is oblique. */

	if (fabs (azmed->org_lat) < cs_AnglTest)
	{
		/* Equatorial Aspect */

		azmed->aspect      = cs_AZMED_EQUATOR;
		azmed->org_lat     = cs_Zero;
		azmed->cos_org_lat = cs_One;
		azmed->sin_org_lat = cs_Zero;
	}
	else if (azmed->org_lat > cs_NPTest)
	{
		azmed->aspect      = cs_AZMED_NORTH;
		azmed->org_lat     = cs_Pi_o_2;
		azmed->cos_org_lat = cs_Zero;
		azmed->sin_org_lat = cs_One;
	}
	else if (azmed->org_lat < cs_SPTest)
	{
		azmed->aspect      = cs_AZMED_SOUTH;
		azmed->org_lat     = cs_Mpi_o_2;
		azmed->cos_org_lat = cs_Zero;
		azmed->sin_org_lat = cs_Mone;
	}
	else
	{
		azmed->aspect      = cs_AZMED_OBLIQUE;
		azmed->cos_org_lat = cos (azmed->org_lat);
		azmed->sin_org_lat = sin (azmed->org_lat);
	}

	/* If the ecentricity is zero, we have a sphere, and
	   nothing special to do here. */

	if (azmed->ecent == 0.0)
	{
		azmed->max_rho = azmed->ka * cs_Pi;
	}
	else
	{
		/* Here only for an ellipsoid. */

		CSmmFsu (&azmed->mmcofF,azmed->ka,azmed->e_sq);
		CSmmIsu (&azmed->mmcofI,azmed->ka,azmed->e_sq);

		azmed->Mp = CSmmFcal (&azmed->mmcofF,cs_Pi_o_2,cs_One,cs_Zero);
		azmed->M1 = CSmmFcal (&azmed->mmcofF,azmed->org_lat,
						     azmed->sin_org_lat,
						     azmed->cos_org_lat);

		azmed->e_sin_p1 = azmed->ecent * azmed->sin_org_lat;
		azmed->e_cos_p1 = azmed->ecent * azmed->cos_org_lat;

		azmed->e_sq_sin_sq = azmed->e_sin_p1 * azmed->e_sin_p1;
		azmed->e_sq_cos_sq = azmed->e_cos_p1 * azmed->e_cos_p1;

		azmed->N1 = azmed->ka / sqrt (cs_One - azmed->e_sq_sin_sq);

		azmed->psi_t1 = azmed->e_sq * azmed->N1 * azmed->sin_org_lat;
		azmed->sin_cos = azmed->sin_org_lat * azmed->cos_org_lat;
		azmed->G = azmed->ecent * azmed->sin_org_lat /
					   azmed->rt_one_esq;
		azmed->G_sq_3 = azmed->G * azmed->G * 3.0;
		azmed->max_rho = azmed->Mp * cs_Two;
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the origin
	   longitude. */

	csprm->cent_mer = azmed->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate the useful range.  The useful
		   range is within 1200 KM of the origin.  We use
		   the approximation of 10 degrees of longitude at
		   the equator equals 1200 KM. */

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_K60;
			csprm->max_ll [LAT] = cs_K90;
			break;

		case cs_AZMED_SOUTH:

			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_Km90;
			csprm->max_ll [LAT] = -cs_K60;
			break;

		default:
		case cs_AZMED_EQUATOR:
		case cs_AZMED_OBLIQUE:
		case cs_AZMED_GUAM:

			csprm->min_ll [LNG] = -cs_Ten;
			csprm->max_ll [LNG] = cs_Ten;
			tmp1 = azmed->org_lat * cs_Radian;
			csprm->min_ll [LAT] = tmp1 - cs_Ten;
			if (csprm->min_ll [LAT] < cs_Km90)
			{
				csprm->min_ll [LAT] = cs_Km90;
			}
			csprm->max_ll [LAT] = tmp1 + cs_Ten;
			if (csprm->max_ll [LAT] > cs_K90)
			{
				csprm->max_ll [LAT] = cs_K90;
			}
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
		/* Choose an arbitrary limit of the equivalant of
		   1,200,000 meters from the origin. */

		tmp1 = 1200000.0 * csprm->csdef.scale;
		csprm->min_xy [XX] = -tmp1;
		csprm->min_xy [YY] = -tmp1;
		csprm->max_xy [XX] =  tmp1;
		csprm->max_xy [YY] =  tmp1;

		CS_quadMM (csprm->min_xy,csprm->max_xy,azmed->x_off,
						       azmed->y_off,
						       azmed->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSazmedF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSazmedI;
	csprm->cs_scale = (cs_SCALE_CAST)CSazmedK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSazmedK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSazmedH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSazmedC;
	csprm->llchk    = (cs_LLCHK_CAST)CSazmedL;
	csprm->xychk    = (cs_XYCHK_CAST)CSazmedX;

	return;
}

/**********************************************************************
**	rtn_val = CSazmedF (azmed,xy,ll);
**
**	struct cs_Azmed_ *azmed;	structure which defines the parameters
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
**	This function calculates the Azimuthal Equidistant
**	projection and returns values in the coordinate system
**	described in the cs_Azmed_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 191-202.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSazmedF (Const struct cs_Azmed_ *azmed,double xy [2],Const double ll [2])
{
	extern char csErrnam [MAXPATH];

	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Two_pi;			/* 2 PI */
	extern double cs_Mpi;				/* -3.14159.... */
	extern double cs_Pi_o_2;			/* PI over 2 */
	extern double cs_Mpi_o_2;			/* Minus PI over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/*-1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Six;				/* 6.0 */
	extern double cs_Seven;				/* 7.0 */
	extern double cs_Eight;				/* 8.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */
	extern double cs_AnglTest1;			/* 1.0 - 0.001 seconds of
										   arc in radians.  Could
										   also be considered to
										   be the cosine of
										   cs_AnglTest. */
	extern double cs_LlNoise;			/* 1.0e-12; about the resolution of an
										   angle in degrees when one accounts 
										   for calculation noise. Any angle
										   less then this can usually be
										   considered to be zero. */

	int rtn_val;

	double lng;		/* The given longitude, after conversion
					   to radians. */
	double lat;		/* The given latitude after conversion
					   to radians. */
	double x = 0.0;			/* initialized to keep the gcc compiler happy */
	double y = 0.0;			/* initialized to keep the gcc compiler happy */

	double M;
	double N;
	double H;
	double H_sq;
	double H_sq_7;
	double GH;
	double rho;
	double psi;
	double del_lng;
	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;
	double cos_lat;
	double cos_c;
	double c;
	double Az;
	double sin_Az;
	double cos_Az;
	double s;
	double s_sq;
	double s_third;
	double s_fourth;
	double s_fifth;
	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.  There
	   is a miminal amount of stuff which is
	   common to both which we perform first. */

	lng = cs_Degree * ll [LNG];
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

	del_lng = lng - azmed->org_lng;
	if      (del_lng > cs_Pi  && azmed->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && azmed->org_lng > 0.0) del_lng += cs_Two_pi;
	if (del_lng > cs_Pi || del_lng <= cs_Mpi)
	{
		/* Note, the inverse can't distinguish between -180
		   and +180; therefore we return a range status for
		   -180. */

		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* See if the datum is a sphere or an ellipsoid. */

	if (azmed->ecent == 0.0)
	{
		/* Here for the sphere. */

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			tmp1 = (cs_Pi_o_2 - lat) * azmed->ka;
			x = tmp1 * sin_del_lng;
			y = -tmp1 * cos_del_lng;
			break;

		case cs_AZMED_SOUTH:

			tmp1 = (cs_Pi_o_2 + lat) * azmed->ka;
			x = tmp1 * sin_del_lng;
			y = tmp1 * cos_del_lng;
			break;

		case cs_AZMED_EQUATOR:

			cos_c = cos_lat * cos_del_lng;
			c = acos (cos_c);
			if (fabs (c) < cs_AnglTest)
			{
				/* At the origin. */
				tmp1 = cs_Zero;
			}
			else if (cos_c < -cs_AnglTest1)
			{
				/* AntiPodal */
				rtn_val = cs_CNVRT_RNG;
				tmp1 = azmed->ka * cs_Pi;
			}
			else
			{
				/* Normal */
				tmp1 = azmed->ka * c / sin (c);
			}

			x = tmp1 * cos_lat * sin_del_lng;
			y = tmp1 * sin_lat;
			break;

		case cs_AZMED_GUAM:

			x = azmed->ka * del_lng * cos_lat;
			tmp1 = x * x * tan (lat) / azmed->two_ka;
			y = azmed->ka * (lat - azmed->org_lat) + tmp1;
			break;

		case cs_AZMED_OBLIQUE:

			cos_c = azmed->sin_org_lat * sin_lat +
				azmed->cos_org_lat * cos_lat * cos_del_lng;
			c = acos (cos_c);

			/* C is the angular distance from the origin.  A
			   zero value indicates the orgin which would cause
			   a division by sin (0); but otherwise is a
			   perfectly normal value for this projection.

			   As c approaches PI, however, we approach the
			   point opposite the origin, and this point is
			   not well defined. */

			if (fabs (c) <= cs_AnglTest)
			{
				/* The origin. */
				tmp1 = cs_Zero;
			}
			else if (cos_c < -cs_AnglTest1)
			{
				/* Antipodal to the origin. */
				rtn_val = cs_CNVRT_RNG;
				tmp1 = azmed->ka * cs_Pi;
			}
			else
			{
				/* Normal, We can divide by sin (c). */
				tmp1 = azmed->ka * (c / sin (c));
			}

			x = tmp1 * cos_lat * sin_del_lng;
			y = tmp1 * (azmed->cos_org_lat * sin_lat -
				    azmed->sin_org_lat * cos_lat * cos_del_lng);
			break;
	
		default:

			CS_stncp (csErrnam,"CS_azmed:1",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}
	}
	else
	{
		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			M = CSmmFcal (&azmed->mmcofF,lat,sin_lat,cos_lat);
			rho = azmed->Mp - M;
			x =  rho * sin_del_lng;
			y = -rho * cos_del_lng;
			break;

		case cs_AZMED_SOUTH:

			M = CSmmFcal (&azmed->mmcofF,lat,sin_lat,cos_lat);
			rho = azmed->Mp + M;
			x = rho * sin_del_lng;
			y = rho * cos_del_lng;
			break;

		case cs_AZMED_GUAM:

			M = CSmmFcal (&azmed->mmcofF,lat,sin_lat,cos_lat);
			tmp1 = sqrt (azmed->e_sq * sin_lat * sin_lat);
			x = azmed->ka * del_lng * cos_lat / tmp1;
			tmp2 = tan (lat) * x * x * tmp1 / azmed->two_ka;
			y = M - azmed->M1 + tmp2;							  /*lint !e834 */
			break;

		case cs_AZMED_EQUATOR:
		case cs_AZMED_OBLIQUE:

			/* Compute Az, the azimuth of the point with respect
			   to the origin on the ellipsoid.

			   Psi is the latitude of the point, adjusted for
			   the ellipsoid.  This is zero or cs_Pi_o_2 if the
			   latitude is a pole (don't want to divide by
			   cos (lat) in this case. */

			if (fabs (lat) > cs_NPTest)
			{
				/* Point is a pole. */
				psi = (lat > 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
			}
			else
			{
				/* Cosine (lat) will not be zero, we can
				   compute psi safely. */

				tmp1 = azmed->e_sq * sin_lat * sin_lat;
				N = azmed->ka / sqrt (cs_One - tmp1);
				tmp2 = (azmed->one_esq * tan (lat)) + azmed->psi_t1 /
							      (N * cos_lat);
				psi = atan (tmp2);
			}
			tmp3 = (azmed->cos_org_lat * tan (psi)) -
				(azmed->sin_org_lat * cos_del_lng);

			/* Tmp3 and sin_del_lng will both be zero if the
			   point is the origin.  Most atan2's (not all) will
			   bomb if both args are zero. */
			if ((fabs (sin_del_lng) < cs_LlNoise) &&
				(fabs (tmp3) < cs_LlNoise))
			{
				/* We are approaching the origin.  Do a very rudimentary
				   equivalent to atan2 on the values.  Note, tmp3 is
				   essentially the sine of delta latitude. In this context,
				   cs_LlNoise is roughly 6 microns. */
				if (fabs (sin_del_lng) > fabs (tmp3))
				{
					/* We'll use sin_del_lng. */
					if (sin_del_lng < cs_Zero)
					{
						Az = cs_Mpi_o_2;
						sin_Az = cs_Mone;
						cos_Az = cs_Zero;
					}
					else
					{
						Az = cs_Pi_o_2;
						sin_Az = cs_One;
						cos_Az = cs_Zero;
					}
				}
				else
				{
					/* Here to use tmp3 (delta Latitude). */
					if (tmp3 < cs_Zero)
					{
						Az = cs_Mpi;
						sin_Az = cs_Zero;
						cos_Az = cs_Mone;
					}
					else
					{
						Az = cs_Zero;
						sin_Az = cs_Zero;
						cos_Az = cs_One;
					}
				}
			}
			else
			{
				/* At least one of the arguments is not zero.  Atan2 can
				   re relied upon to produce vaalid results. */
				Az = atan2 (sin_del_lng,tmp3);
				sin_Az = sin (Az);
				cos_Az = cos (Az);
			}

			/* Ok, we have the (an) azimuth.  Need to compute the
			   angular distance to the point on the ellipsoid.
			   In the case of small angles, the sine and the
			   angle are equivalent. */

			if (fabs (sin_Az) < cs_AnglTest)
			{
				/* Here to avoid a divide by sin (Az). */

				tmp4 = azmed->cos_org_lat * sin (psi) -
				       azmed->sin_org_lat * cos (psi);
				s = asin (tmp4);
				if (cos_Az < 0.0)
				{
					s = -s;
				}
			}
			else
			{
				s = asin (sin_del_lng * cos (psi) / sin_Az);
			}

			/* S is essentially the angular distance over a
			   sphere. Convert to c, the linear distance over
			   the ellipsoid, via a power series calculation. */

			s_sq = s * s;
			s_third = s_sq * s;
			s_fourth = s_sq * s_sq;
			s_fifth = s_fourth * s;

			H = azmed->e_cos_p1 * cos_Az / azmed->rt_one_esq;
			H_sq = H * H;
			H_sq_7 = cs_Seven * H_sq;
			GH = azmed->G * H;

			tmp2 = H_sq * (cs_Four - H_sq_7);
			tmp2 -= azmed->G_sq_3 * (cs_One - H_sq_7);

			tmp1 = cs_One;
			tmp1 -= s_sq * H_sq * (cs_One - H_sq) / cs_Six;
			tmp1 += (s_third / cs_Eight) * GH * (cs_One - cs_Two * H_sq);
			tmp1 += (s_fourth / 120.0) * tmp2;
			tmp1 -= (s_fifth / 48.0) * GH;
			c = azmed->N1 * s * tmp1;

			x = c * sin_Az;
			y = c * cos_Az;
			break;

		default:
			CS_stncp (csErrnam,"CS_azmed:2",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}
	}

	/* Add the rotation, quad effect, and offset. */
	if (rtn_val >= 0)
	{
		xy [XX] = azmed->cos_Az * x - azmed->sin_Az * y;
		xy [YY] = azmed->cos_Az * y + azmed->sin_Az * x;
		if (azmed->quad == 0)
		{
			xy [XX] += azmed->x_off;
			xy [YY] += azmed->y_off;
		}
		else
		{
			CS_quadF (xy,xy [XX],xy [YY],azmed->x_off,azmed->y_off,
								azmed->quad);
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSazmedI (azmed,ll,xy);
**
**	struct cs_Azmed_ *azmed;	structure which defines the parameters
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
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 191-202.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSazmedI (Const struct cs_Azmed_ *azmed,double ll [2],Const double xy [2])
{
	extern char csErrnam [];

	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/*-1.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_Six;				/* 6.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;
	int itr_cnt;

	double x = 0.0;			/* initialized to keep the gcc compiler happy */
	double y = 0.0;			/* initialized to keep the gcc compiler happy */
	double c;
	double lat = 0.0;		/* initialized to keep the gcc compiler happy */
	double del_lng;
	double rho;
	double sin_c;
	double cos_c;
	double Az;
	double sin_Az;
	double cos_Az;
	double cos_psi;
	double last_lat;
	double A;
	double B;
	double D;
	double D_third;
	double D_fourth;
	double E;
	double E_sq;
	double E_third;
	double sin_E;
	double F;
	double M;
	double psi;
	double tmp1;
	double tmp2;

	double my_xy [2];

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity of the
	   datum in use is 0.0 exactly, we shall use the
	   spherical formulae.

	   The calculations which immediately follow are required
	   by both cases.  First, we remove the false origin, return
	   to the normal X/Y quadrant, and unrotate the coordinates. */

	if (azmed->quad == 0)
	{
		my_xy [XX] = xy [XX] - azmed->x_off;
		my_xy [YY] = xy [YY] - azmed->y_off;
	}
	else
	{
		CS_quadI (&my_xy [XX],&my_xy [YY],xy,azmed->x_off,
						     azmed->y_off,
						     azmed->quad);
	}
	x = azmed->cos_Az * my_xy [XX] + azmed->sin_Az * my_xy [YY];
	y = azmed->cos_Az * my_xy [YY] - azmed->sin_Az * my_xy [XX];

	/* Rho is the radius of the circle whose center is at the
	   origin and goes thorugh the current point. */

	rho = sqrt (x * x + y * y);
	if (rho < azmed->one_mm)
	{
		/* The coordinate is essentially the origin.  We
		   do the following to avoid a atan2(0,0).

		   If the origin is either pole, the longitude
		   will be undefined. */

		if (azmed->aspect == cs_AZMED_NORTH ||
		    azmed->aspect == cs_AZMED_SOUTH)
		{
			rtn_val = cs_CNVRT_INDF;
		}
		ll [LNG] = azmed->org_lng * cs_Radian;
		ll [LAT] = azmed->org_lat * cs_Radian;
		return (rtn_val);
	}
	if (rho >= azmed->max_rho)
	{
		/* Opps!!! The x and/or y is so large it is outside
		   the circle which defines the extent of the
		   coordinate system.  Convert xy to lie on the
		   bounding circle at the apropriate azimuth. */

		rtn_val = cs_CNVRT_RNG;
		x *= azmed->max_rho / rho;
		y *= azmed->max_rho / rho;
		rho = azmed->max_rho;
	}

	/* Now we can convert back to lat/longs.  We know that rho
	   is not zero which implies that at least one of x and y is
	   not zero. */

	del_lng = cs_Zero;
	if (azmed->ecent == 0.0)
	{
		/* Here for the sphere.  The sine and cosine functions
		   shouldnever return a value greater than one.  Thus,
		   the asin ()'s here should be OK without any checking. */

		c = rho / azmed->ka;
		sin_c = sin (c);
		cos_c = cos (c);

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			del_lng = atan2 (x,-y);
			lat     = asin  (cos_c);
			break;

		case cs_AZMED_SOUTH:

			del_lng =  atan2 (x,y);
			lat     = -asin  (cos_c);
			break;

		case cs_AZMED_EQUATOR:

			c = rho / azmed->ka;
			tmp1 = x * sin (c);
			tmp2 = rho * cos (c);
			del_lng = atan2 (tmp1,tmp2);
			tmp1  = (y / rho) * sin_c;
			if (fabs (tmp1) > cs_One)
			{
				tmp1 = (tmp1 >= 0.0) ? cs_One : cs_Mone;
			}
			lat = asin (tmp1);
			break;

		case cs_AZMED_GUAM:

			lat = azmed->org_lat;
			itr_cnt = 16;
			for (;;)
			{
				last_lat = lat;
				tmp1 = x * x * tan (lat) / azmed->two_ka;
				lat = azmed->org_lat + (y - tmp1) / azmed->ka;
				if (fabs (lat - last_lat) < cs_AnglTest)
				{
					break;
				}
				if (--itr_cnt <= 0)
				{
					rtn_val = cs_CNVRT_RNG;
					break;
				}
			}

			/* Longitude is undefined at the poles. */

			if (fabs (lat) < cs_NPTest)
			{
				del_lng = x / (azmed->ka * cos (lat));
			}
			break;

		case cs_AZMED_OBLIQUE:

			tmp1  = cos_c * azmed->sin_org_lat;
			tmp1 += (y / rho) * sin_c * azmed->cos_org_lat;
			if (fabs (tmp1) > cs_One)
			{
				tmp1 = (tmp1 >= 0.0) ? cs_One : cs_Mone;
			}
			lat = asin (tmp1);

			tmp1  = x * sin_c;
			tmp2  = rho * cos_c * azmed->cos_org_lat;
			tmp2 -= y * sin_c * azmed->sin_org_lat;
			del_lng = atan2 (tmp1,tmp2);
			break;

		default:
			CS_stncp (csErrnam,"CS_azmed:3",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}
	}
	else
	{
		/* Here for the ellisoid.  We've already dealt
		   with x and y == 0. */

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			del_lng = atan2 (x,-y);
			lat = CSmmIcal (&azmed->mmcofI,(azmed->Mp - rho));
			break;

		case cs_AZMED_SOUTH:

			del_lng = atan2 (x,y);
			lat = CSmmIcal (&azmed->mmcofI,(rho - azmed->Mp));
			break;

		case cs_AZMED_GUAM:

			itr_cnt = 16;
			lat = azmed->org_lat;
			for (;;)
			{
				last_lat = lat;
				tmp1 = sin (lat);
				tmp1 = cs_One - azmed->e_sq * tmp1 * tmp1;
				tmp1 = x * x * tan (lat) * sqrt (tmp1);
				tmp1 = y - tmp1 / azmed->two_ka;
				M = azmed->M1 + tmp1;
				lat = CSmmIcal (&azmed->mmcofI,M);
				if (fabs (lat - last_lat) < cs_AnglTest)
				{
					break;
				}
				if (--itr_cnt <= 0)
				{
					rtn_val = cs_CNVRT_RNG;
					break;
				}
			}

			/* Longitude is undefined at the poles. */

			if (fabs (lat) < cs_NPTest)
			{
				tmp1 = sin (lat);
				tmp1 = cs_One - azmed->e_sq * tmp1 * tmp1;
				del_lng = x * sqrt (tmp1) / (azmed->ka * cos (lat));
			}
			break;

		case cs_AZMED_EQUATOR:
		case cs_AZMED_OBLIQUE:

			Az = atan2 (x,y);
			sin_Az = sin (Az);
			cos_Az = cos (Az);
			A = -azmed->e_sq_cos_sq * cos_Az * cos_Az / azmed->one_esq;
			B = cs_Three * azmed->e_sq * (cs_One - A) * azmed->sin_cos *
						cos_Az / azmed->one_esq;
			D = rho / azmed->N1;
			D_third = D * D * D;
			D_fourth = D_third * D;
			E = D;
			E -= A * (cs_One + A) * D_third / cs_Six;
			E -= B * (cs_One + cs_Three * A) * D_fourth / 24.0;
			E_sq = E * E;
			E_third = E_sq * E;
			sin_E = sin (E);
			F = cs_One - (A * E_sq * cs_Half) - (B * E_third / cs_Six);		/*lint !e834 */
			tmp1 = azmed->sin_org_lat * cos (E);
			tmp2 = azmed->cos_org_lat * sin_E * cos_Az;
			psi = asin (tmp1 + tmp2);

			/* Psi is essentially the latitude of the point
			   adjusted for the affect of the ellipsoid.  Thus,
			   it can be zero, and it can be the equivalent of
			   +-90 degrees.

			   In analyzing the following, note that:
			   1) E is the ellipsoidally adjusted angular
			      distance from the origin to the point;
			   2) the formula is actually equation 5-4 of Synder,
			      reorganized to produce the delta longitude.

			   Thus, I conclude that delta longitude goes to
			   zero as cos(psi) approaches zero. */

			cos_psi = cos (psi);
			if (fabs (cos_psi) > cs_AnglTest)
			{
				del_lng = asin (sin_Az * sin_E / cos (psi));
			}

			/* Now, the latitude.  We have a similar problem,
			   but now with sin (psi) == 0. As psi approaches
			   zero, this whole mess reduces to lat = psi,
			   which is zero, of course. */

			if (fabs (psi) > cs_AnglTest)
			{
				tmp1 = azmed->e_sq * F * azmed->sin_org_lat / sin (psi);
				tmp2 = tan (psi) / azmed->one_esq;
				lat = atan ((cs_One - tmp1) * tmp2);
			}
			else
			{
				lat = cs_Zero;
			}
			break;

		default:

			CS_stncp (csErrnam,"CS_azmed:",MAXPATH);
			CS_erpt (cs_ISER);
			rtn_val = -1;
			break;
		}
	}

	/* Longitude is undefined, mathematically, at either pole. */
	if (rtn_val >= 0)
	{
		if (fabs (lat) > cs_NPTest) rtn_val = cs_CNVRT_INDF;

		/* Longitudes are always normalized for this projection. */

		ll [LNG] = (del_lng + azmed->org_lng) * cs_Radian;
		ll [LAT] = lat * cs_Radian;
	}
	return (rtn_val);
}
/**********************************************************************
**	gamma = CSazmedC (azmed,ll);
**
**	struct cs_Azmed_ *azmed;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert Azimuthal
**								Equidistant projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double gamma;				returns the computed convergence angle for
**								the coordinate system as the specified
**								location in degrees east of north.
**
**	We have not as yet found or deduced an analytical equation
**	for the convergence angle for this projection.  We calculate
**	it empicially. The convergence angle is defined as the
**	arctangent of the partial derivative of Y with respect to
**	latitude (read delta Y when the latitude changes a skosh)
**	divied by the partial derivative of X with repsect to
**	latitude (i.e. delta X).  See Synder/Bugayevskiy, page 16.
**********************************************************************/

double EXP_LVL9 CSazmedC (Const struct cs_Azmed_ *azmed,Const double ll [2])
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
	status = CSazmedF (azmed,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSazmedF (azmed,xy2,my_ll);
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
**	kk = CSazmedK (azmed,ll);
**
**	struct cs_Azmed_ *azmed;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the normal grid scale of the
**								coordinate system at the specified point.
**
**********************************************************************/

double EXP_LVL9 CSazmedK (Const struct cs_Azmed_ *azmed,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Pi_o_2;			/* PI / 2.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_NPTest;			/* 0.001 arc seconds short
										   of the north pole, in
										   radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds short
										   of the south pole, in
										   radians. */
	extern double cs_AnglTest1;			/* 1 - sin of 0.001 seconds
										   of arc. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */

	int status;

	double kk;
	double lng;
	double lat;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double cos_cc;
	double cc;
	double xy_dd;
	double ll_dd;
	double del_xx;
	double del_yy;
	double rho;
	double tmp;
	double M;

	double oll [2];
	double ll1 [2];
	double ll2 [2];
	double xy1 [2];
	double xy2 [2];

	if (azmed->ecent == 0.0)
	{
		/* Here for the spherical case. */

		lat = ll [LAT] * cs_Degree;
		lng = ll [LNG] * cs_Degree;
		del_lng = CS_adj2pi (lng - azmed->org_lng);

		/* What aspect? */

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			if (lat >= cs_NPTest)
			{
				kk = cs_One;
			}
			else if (lat <= cs_SPTest)
			{
				kk = cs_SclInf;
			}
			else
			{
				kk = (cs_Pi_o_2 - lat) / cos (lat);
			}
			break;

		case cs_AZMED_SOUTH:

			if (lat <= cs_SPTest)
			{
				kk = cs_One;
			}
			else if (lat >= cs_NPTest)
			{
				kk = cs_SclInf;
			}
			else
			{
				kk = (cs_Pi_o_2 + lat) / cos (lat);
			}
			break;

		default:
		case cs_AZMED_GUAM:
		case cs_AZMED_EQUATOR:
		case cs_AZMED_OBLIQUE:

			lng = ll [LNG] * cs_Degree;
			del_lng = CS_adj2pi (lng - azmed->org_lng);

			/* Read cc as the angular distance to the
			   point in question, from the origin. */

			cos_cc = azmed->sin_org_lat * sin (lat) +
					azmed->cos_org_lat * cos (lat) *
					cos (del_lng);

			if (cos_cc > cs_AnglTest1)
			{
				/* The origin. */
				kk = cs_One;
			}
			else if (cos_cc < -cs_AnglTest1)
			{
				/* AntiPodal */
				kk = cs_SclInf;
			}
			else
			{
				/* We can divide by sin (cc). */
				cc = acos (cos_cc);
				kk = cc / sin (cc);
			}
			break;
		}
	}
	else
	{
		/* Here for the ellipsoidal case. */

		switch (azmed->aspect) {

		case cs_AZMED_NORTH:

			lat = ll [LAT] * cs_Degree;
			if (lat >= cs_NPTest)
			{
				kk = cs_One;
			}
			else if (lat < cs_SPTest)
			{
				kk = cs_SclInf;
			}
			else
			{
				/* Here when we can divide by cos (lat) */

				sin_lat = sin (lat);
				cos_lat = cos (lat);
				M = CSmmFcal (&azmed->mmcofF,lat,sin_lat,cos_lat);
				rho = azmed->Mp - M;
				tmp = cs_One - azmed->e_sq * sin_lat * sin_lat;
				kk = rho * sqrt (tmp) / (cos_lat * azmed->ka);
			}
			break;

		case cs_AZMED_SOUTH:

			lat = ll [LAT] * cs_Degree;
			if (lat <= cs_SPTest)
			{
				kk = cs_One;
			}
			else if (lat >= cs_NPTest)
			{
				kk = cs_SclInf;
			}
			else
			{
				/* Here when we can divide by cos (lat) */

				sin_lat = sin (lat);
				cos_lat = cos (lat);
				M = CSmmFcal (&azmed->mmcofF,lat,sin_lat,cos_lat);
				rho = M + azmed->Mp;
				tmp = cs_One - azmed->e_sq * sin_lat * sin_lat;
				kk = rho * sqrt (tmp) / (cos_lat * azmed->ka);
			}
			break;

		default:
		case cs_AZMED_GUAM:
		case cs_AZMED_EQUATOR:
		case cs_AZMED_OBLIQUE:

			/* We do not have analytical formulas for these aspects,
			   so until we do:

			   CSllnrml computes the lat/long of two points
			   which form a geodesic of one arc second which
			   is normal to the geodesic from the origin
			   (oll) to the point in question (ll). */

			oll [0] = azmed->org_lng * cs_Radian;
			oll [1] = azmed->org_lat * cs_Radian;
			CSllnrml (oll,ll,ll1,ll2);

			/* Convert the resulting lat/longs to cartesian
			   coordinates. */

			status = CSazmedF (azmed,xy1,ll1);
			if (status != cs_CNVRT_NRML)
			{
				return (cs_Mone);
			}
			status = CSazmedF (azmed,xy2,ll2);
			if (status != cs_CNVRT_NRML)
			{
				return (cs_Mone);
			}

			/* Calculate the geodetic distance between the two
			   lat/long points.  Note, we provide the geodetic
			   calculator with the scaled radius of the earth
			   so that the distance it computes will be in the
			   same units as the X and Y coordinates. */

			CS_llazdd (azmed->ka,azmed->e_sq,ll1,ll2,&ll_dd);

			/* Calculate the grid distance between the two
			   points. */

			del_xx = xy1 [XX] - xy2 [XX];
			del_yy = xy1 [YY] - xy2 [YY];
			xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

			/* Return the ratio of the two distances as the
			   parallel scale factor at the indicated point. */

			kk = xy_dd / ll_dd;
			break;
		}
	}
	return (kk);
}

/**********************************************************************
**	hh = CSazmedH (azmed,ll);
**
**	struct cs_Azmed_ *azmed;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal
**								Equidistant projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double hh;					returns the radial grid scale of the
**								projected coordinate system at that
**								point.
**
**	I wish they were all this easy.
**********************************************************************/

double EXP_LVL9 CSazmedH (Const struct cs_Azmed_ *azmed,Const double ll [2])
{
	extern double cs_One;

	double hh;

	hh = cs_One;

	return (hh);
}											/*lint !e715 */

/**********************************************************************
**	status = CSazmedL (azmed,cnt,pnts);
**
**	struct cs_Azmed_ *azmed;	the coordinate system against which the check is
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

int EXP_LVL9 CSazmedL (Const struct cs_Azmed_ *azmed,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi;			/* 3.14159....  */
	extern double cs_Pi_o_2;		/* PI over 2 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc
									   in radians. */

	int ii;
	int status;

	double lat;
	double lng;
	double del_lng;

	/* Just check all points.  Only one point, the opposite
	   of the origin is a bust. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		if (fabs (lat + azmed->org_lat) > cs_AnglTest)
		{
			/* Latitude is definitely not anti-podal.
			   This point is OK. */

			continue;
		}
		lng = pnts [ii][LNG] * cs_Degree;
		del_lng = CS_adj2pi (lng - azmed->org_lng);
		if (fabs (fabs (del_lng) - cs_Pi) < cs_AnglTest)
		{
			/* Point is definitely opposite to the
			   origin.  It's a bust. */

			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSazmedX (azmed,cnt,pnts);
**
**	struct cs_Azmed_ *azmed;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSazmedL to check lat/long
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

int EXP_LVL9 CSazmedX (Const struct cs_Azmed_ *azmed,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double dd;
	double xx, yy;

	rtn_val = cs_CNVRT_OK;

	/* Simply check that all X's and Y's are within the maximum
	   radius of the projection.  No special checks are required
	   for lines and/or regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		if (azmed->quad == 0)
		{
			xx = pnts [ii][XX] - azmed->x_off;
			yy = pnts [ii][YY] - azmed->y_off;
		}
		else
		{
			CS_quadI (&xx,&yy,pnts [ii],azmed->x_off,
						    azmed->y_off,
						    azmed->quad);
		}

		dd = sqrt (xx * xx + yy * yy);
		if (dd > azmed->max_rho)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
