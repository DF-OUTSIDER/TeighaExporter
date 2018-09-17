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
**	err_cnt = CSazmeaQ (cs_def,proj_code,err_list,list_sz);
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

int EXP_LVL9 CSazmeaQ (Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)

{
	extern double cs_MinLng;	/* -180.0 */
	extern double cs_MinLat;	/* - 90.0 */
	extern double cs_MaxLng;	/* +180.0 */
	extern double cs_MaxLat;	/* + 90.0 */

	int err_cnt;
	
	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Equal Area Azimuthal specific stuff. */
	
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

	/* That's it for the Lambert Azimuthal Equal Area projection. */
	
	return (err_cnt + 1);
}
/**********************************************************************
**	CSazmeaS (csprm);
**	qq = CSazmeaqq (azmea,sin_lat,sin_sq);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**	struct cs_Azmea_ *azema;	pointer to the initialized (at least paritally
**								so) structure defining a coordinate system
**								based on the Azmuthal Equal Area Projection.
**	double sin_lat;				the sine of the latitude at which q is to be
**								computed.
**	double *sin_sq;				location at which 1 - e_sq * sin_lat is returned
**								to prevent duplicate calculation of this term.
**	double qq;					the q value at the provided latitude.
**
**	The calculation of the q value is required in at least four
**	separate places, therefore, we have a function CSazmeaqq to
**	to this.
**********************************************************************/

void EXP_LVL9 CSazmeaS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;		/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;		/* 1.0 / RADIAN  */
	extern double cs_Radian;		/* 57.27.....  */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Mone;			/* -1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Three;			/* 3.0 */
	extern double cs_K15;			/* 15.0 */
	extern double cs_K30;			/* 30.0 */
	extern double cs_K75;			/* 75.0 */
	extern double cs_K180;			/*  180.0 */
	extern double cs_Km180;			/* -180.0 */
	extern double cs_K90;			/*   90.0 */
	extern double cs_Km90;			/*  -90.0 */
	extern double cs_AnglTest;		/* 0.001 arc seconds,
									   in radians. */

	struct cs_Azmea_ *azmea;

	double q1;
	double m1;
	double sin_esq_term;

	double tmp;

	azmea = &csprm->proj_prms.azmea;

	/* Transfer the necessary arguments to the
	   "azmea" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	azmea->org_lng = csprm->csdef.org_lng * cs_Degree;
	azmea->org_lat = csprm->csdef.org_lat * cs_Degree;
	azmea->k = csprm->csdef.scale;
	azmea->x_off = csprm->csdef.x_off;
	azmea->y_off = csprm->csdef.y_off;
	azmea->ecent = csprm->datum.ecent;
	azmea->e_sq = azmea->ecent * azmea->ecent;
	azmea->one_esq = cs_One - azmea->e_sq;
	azmea->ka = csprm->datum.e_rad * azmea->k;
	azmea->cos_org_lat = cos (azmea->org_lat);
	azmea->sin_org_lat = sin (azmea->org_lat);
	azmea->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Setup the proper Y axis. */

	azmea->Az = csprm->csdef.prj_prm1 * cs_Degree;
	if (fabs (azmea->Az) < cs_AnglTest)
	{
		azmea->sin_Az = cs_Zero;
		azmea->cos_Az = cs_One;
	}
	else
	{
		azmea->sin_Az = sin (azmea->Az);
		azmea->cos_Az = cos (azmea->Az);
	}

	/* If we are very close to either a north or south polar
	   aspect, set the sine's and cosine's to their correct
	   values.  The sine and cosine functions tend to leave
	   us just a bit short on these numbers.  We also set the
	   polar flag, which is used in the sphereical inverse
	   equations (perhaps elsewhere later on).  The polar
	   variable is zero if the aspect is oblique. */

	azmea->polar = cs_AZMEA_OBLIQUE;
	if (fabs (azmea->cos_org_lat) < cs_AnglTest)
	{
		/* Polar aspect, which one? */

		azmea->cos_org_lat = cs_Zero;
		if (azmea->sin_org_lat < 0.0)
		{
			/* South polar aspect. */

			azmea->polar = cs_AZMEA_SOUTH;
			azmea->sin_org_lat = cs_Mone;
		}
		else
		{
			/* North polar aspect. */

			azmea->polar = cs_AZMEA_NORTH;
			azmea->sin_org_lat = cs_One;
		}
	}
	else if (fabs (azmea->sin_org_lat) < cs_AnglTest)
	{
		/* Equatorial Aspect. */

		azmea->polar = cs_AZMEA_EQUATOR;
		azmea->sin_org_lat = cs_Zero;
		azmea->cos_org_lat = cs_One;
	}

	/* Set up a value in the units of the cartesian system
	   which represents one millimeter.  The extra stuff is for
	   the test coordinate systems based on the unit sphere
	   and its siblings. */

   	azmea->one_mm = 0.001 * azmea->k;
	if (csprm->datum.e_rad == cs_One ||			/*lint !e777 */
	    csprm->datum.e_rad == cs_Three)			/*lint !e777 */
	{
	    	azmea->one_mm = 2.0E-10;
	}

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */

	if (azmea->ecent == 0.0)
	{
		/* Not much to do here. */

		azmea->two_ka = azmea->ka * cs_Two;
	}
	else
	{
		/* Here for the ellipsoidal calculations.
		
		   CSazmeaqq uses the following value in the
		   cs_Azmea_ structure, therefore, this must be
		   done first. */

		azmea->one_o_2e = cs_One / (azmea->ecent * cs_Two);

		/* Compute q value for the origin latitude. */

		q1 = CSazmeaqq (azmea,azmea->sin_org_lat,&sin_esq_term);

		/* Qp is the same as above, but lat = 90, therefore,
		   the sine of lat is 1.0.  We could do this inline
		   faster, but this is the one time setup function. */

		azmea->qp = CSazmeaqq (azmea,cs_One,&tmp);

		/* There could be some performance improvments here.
		   Let's get it working first. */

		if (azmea->polar == cs_AZMEA_OBLIQUE ||
		    azmea->polar == cs_AZMEA_EQUATOR)
		{
			azmea->beta1 = asin (q1 / azmea->qp);
			azmea->sin_beta1 = sin (azmea->beta1);
			azmea->cos_beta1 = cos (azmea->beta1);

			azmea->Rq = azmea->ka * sqrt (azmea->qp * cs_Half);

			m1 = azmea->cos_org_lat / sqrt (sin_esq_term);
			azmea->D = (azmea->ka * m1) / (azmea->Rq * azmea->cos_beta1);

			azmea->Rsq_ovr_a = azmea->Rq * azmea->Rq / azmea->ka;
			azmea->D_cos_b1 = azmea->D * azmea->cos_beta1;
			azmea->Dsq_sin_b1 = azmea->D * azmea->D * azmea->sin_beta1;

			azmea->two_Rq = azmea->Rq * cs_Two;
		}

		tmp = (cs_One - azmea->ecent) / (cs_One + azmea->ecent);
		tmp = azmea->one_o_2e * log (tmp);
		tmp = cs_One - (azmea->one_esq * tmp);
		azmea->asq_qconv = azmea->ka * azmea->ka * tmp;
		azmea->two_asq = azmea->asq_qconv + azmea->asq_qconv;

		/* Set up the authalic latitude (beta) series. */

		CSbtIsu (&azmea->btcoefI,azmea->e_sq);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = azmea->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  For azimuthals, there's not much
		   we can do, given the rectangular nature of the min/max
		   technique currently available. So, we open up the
		   whole world, and let the CSazmeaL function weed out
		   the bad points. */

		csprm->min_ll [LNG] = -cs_K15;
		csprm->max_ll [LNG] = cs_K15;

		switch (azmea->polar) {

		case cs_AZMEA_NORTH:

			csprm->cent_mer = cs_Zero;
			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_K75;
			csprm->max_ll [LAT] = cs_K90;
			break;

		case cs_AZMEA_SOUTH:

			csprm->cent_mer = cs_Zero;
			csprm->min_ll [LNG] = cs_Km180;
			csprm->max_ll [LNG] = cs_K180;
			csprm->min_ll [LAT] = cs_Km90;
			csprm->max_ll [LAT] = -cs_K75;
			break;

		case cs_AZMEA_EQUATOR:

			csprm->min_ll [LNG] = cs_Km90;
			csprm->max_ll [LNG] = cs_K90;
			csprm->min_ll [LAT] = -cs_K30;
			csprm->max_ll [LAT] = cs_K30;
			break;

		default:
		case cs_AZMEA_OBLIQUE:

			csprm->min_ll [LNG] = -cs_K30;
			csprm->max_ll [LNG] = cs_K30;
			csprm->min_ll [LAT] = azmea->org_lat * cs_Radian;
			csprm->max_ll [LAT] = csprm->min_ll [LAT];
			csprm->min_ll [LAT] -= cs_K30;
			if (csprm->min_ll [LAT] < cs_Km90)
			{
				csprm->min_ll [LAT] = cs_Km90;
			}
			csprm->max_ll [LNG] += cs_K30;
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
		/* No specification in the coordinate system definition.
		   The setup is virtually complete, so we can use CSazmeaF
		   to calculate some values as necessary.  We compute a
		   square which will enclose a circle with a radius
		   of azmea->ka, centered at the origin. */

		csprm->max_xy [XX] =  azmea->ka;
		csprm->min_xy [XX] = -azmea->ka;
		csprm->max_xy [YY] =  azmea->ka;
		csprm->min_xy [YY] = -azmea->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,azmea->x_off,
						       azmea->y_off,
						       azmea->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSazmeaF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSazmeaI;
	csprm->cs_scale = (cs_SCALE_CAST)CSazmeaK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSazmeaK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSazmeaH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSazmeaC;
	csprm->llchk    = (cs_LLCHK_CAST)CSazmeaL;
	csprm->xychk    = (cs_XYCHK_CAST)CSazmeaX;

	return;
}
/*
	The following computes q and the sine term associated
	with it.  Since this is repeated in four places, we have
	isolated it into a function.
*/
double EXP_LVL9 CSazmeaqq (Const struct cs_Azmea_ *azmea,double sin_lat,double *sin_sq)
{
	extern double cs_One;

	double qq;
	double tmp;

	tmp = azmea->ecent * sin_lat;
	*sin_sq = cs_One - (tmp * tmp);
	tmp = log ((cs_One - tmp) / (cs_One + tmp));
	tmp = (sin_lat / *sin_sq) - (azmea->one_o_2e * tmp);
	qq = azmea->one_esq * tmp;
	return (qq);
}
/**********************************************************************
**	rtn_val = CSazmeaF (azmea,xy,ll);
**
**	struct cs_Azmea_ *azmea;	structure which defines the parameters
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
**	This function calculates the Azimuthal Equal Area
**	projection and returns values in the coordinate system
**	described by the cs_Azmea_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 182-190.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSazmeaF (Const struct cs_Azmea_ *azmea,double xy [2],Const double ll [2])
{
	extern double cs_Degree;		/* 1.0 / RADIAN  */
	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Mone;			/*-1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Pi;			/* PI */
	extern double cs_Mpi;			/* -PI */
	extern double cs_Two_pi;		/* 2 PI */
	extern double cs_Pi_o_2;		/* PI over 2 */
	extern double cs_Pi_o_4;		/* PI over 4 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc,
									   in radians. */
	extern double cs_NPTest;		/* 0.001 seconds of arc
									   short of the north pole,
									   in radians. */
	extern double cs_Mhuge;			/* -1.0E+32 */

	int rtn_val;

	double lng;		/* The given longitude, after conversion
					   to radians. */
	double lat;		/* The given latitude after conversion
					   to radians. */
	double x;
	double y;

	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;
	double cos_lat;

	double del_lng;
	double B;
	double q;
	double k_p_ka;
	double rho;
	double beta;
	double sin_beta = 0.0;		/* initialized to keep gcc happy */
	double cos_beta = 0.0;		/* initialized to keep gcc happy */
	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

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

	del_lng = lng - azmea->org_lng;
	if      (del_lng > cs_Pi  && azmea->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && azmea->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	if (azmea->ecent == 0.0)
	{
		/* Here for the sphere. */

		switch (azmea->polar) {

		case cs_AZMEA_OBLIQUE:

			/* Read k_p_ka as ka times k'. */

			tmp1 = cos_lat * cos_del_lng;

			/* Tmp2 approaches zero as the angular distance
			   from the origin to the point approaches
			   Pi. */

			tmp2 = cs_One + (azmea->sin_org_lat * sin_lat) +
					(azmea->cos_org_lat * tmp1);
			if (tmp2 < cs_AnglTest)
			{
				/* Point we are converting is very close
				   to the point opposite the origin on
				   the sphere. */

				rtn_val = cs_CNVRT_RNG;
				tmp2 = cs_AnglTest;
			}
			k_p_ka = azmea->ka * sqrt (cs_Two / tmp2);

			x = k_p_ka * cos_lat * sin_del_lng;
			y = k_p_ka * ((azmea->cos_org_lat * sin_lat) -
					      (azmea->sin_org_lat * tmp1));
			break;

		case cs_AZMEA_EQUATOR:

			/* This is a faster version for the equatorial
			   aspect.  Read k_p_ka as k' times ka. */

			tmp1 = cs_One + (cos_lat * cos_del_lng);
			if (tmp1 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp1 = cs_AnglTest;
			}
			k_p_ka = azmea->ka * sqrt (cs_Two / tmp1);
			x = k_p_ka * cos_lat * sin_del_lng;
			y = k_p_ka * sin_lat;
			break;

		case cs_AZMEA_NORTH:

			tmp1 = cs_Pi_o_4 - (lat * cs_Half);
			tmp1 = azmea->two_ka * sin (tmp1);

			x = tmp1 * sin_del_lng;
			y = -tmp1 * cos_del_lng;
			break;

		case cs_AZMEA_SOUTH:

			tmp1 = cs_Pi_o_4 - (lat * cs_Half);
			tmp1 = azmea->two_ka * cos (tmp1);

			x = tmp1 * sin_del_lng;
			y = tmp1 * cos_del_lng;
			break;

		default:

			rtn_val = cs_CNVRT_DOMN;
			x = cs_Mhuge;
			y = cs_Mhuge;
		}											/*lint !e744 */

	}
	else
	{
		/* Here for the ellisoid.  Again, there are several
		   different cases.  We always need q however.  We
		   don't need the intermediary value which CSazmeaqq
		   also returns. q is valid for all legitimate
		   latitudes.  For the purposes of analyzing for
		   busts, consider q to be equal to sin (lat). */

		q = CSazmeaqq (azmea,sin_lat,&tmp1);

		/* We need beta for the oblique and equatorial case. */

		if (azmea->polar == cs_AZMEA_OBLIQUE ||
		    azmea->polar == cs_AZMEA_EQUATOR)
		{
			/* azmea->qp is q evaluated for 90 degrees, and
			   is never equal to zero.  However, due to
			   noise in low end bits, the quotient can
			   be greater than one and cause a floating
			   point exception. */

			sin_beta = q / azmea->qp;
			if (fabs (sin_beta) >= cs_One)
			{
				sin_beta = (sin_beta > 0.0) ? cs_One : cs_Mone;
			}
			beta = asin (sin_beta);
			cos_beta = cos (beta);
		}

		/* For rough analysis purposes, i.e. looking for busts,
		   consider q to be sin (lat), qp to be 1.0, and beta
		   to be equivalent to lat.

		   Now for the code which is distinct among the
		   various aspects of this projection. */

		switch (azmea->polar) {

		case cs_AZMEA_OBLIQUE:

			/* Read tmp1 as 1 + cos (c), c being the angular
			   distance from the origin to the point being
			   converted. This approaches zero as c approaches
			   pi. */

			tmp1 = cs_One +
			       (azmea->sin_beta1 * sin_beta) +
			       (azmea->cos_beta1 * cos_beta * cos_del_lng);		/*lint !e644*/
			if (tmp1 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp1 = cs_AnglTest;
			}
			B = azmea->Rq * sqrt (cs_Two / tmp1);

			x = B * azmea->D * cos_beta * sin_del_lng;

			/* D is zero only if the origin is one of the poles,
			   not the case here. */

			tmp2 = (azmea->cos_beta1 * sin_beta) -
			       (azmea->sin_beta1 * cos_beta * cos_del_lng);
			y = tmp2 * (B / azmea->D);
			break;

		case cs_AZMEA_EQUATOR:

			tmp1 = cs_One + (cos_beta * cos_del_lng);
			if (tmp1 < cs_AnglTest)
			{
				/* Point is opposite the origin. */

				rtn_val = cs_CNVRT_RNG;
				tmp1 = cs_AnglTest;
			}
			tmp1 = sqrt (cs_Two / tmp1);

			x = azmea->ka * cos_beta * sin_del_lng * tmp1;
			y = azmea->Rsq_ovr_a * sin_beta * tmp1;
			break;

		case cs_AZMEA_NORTH:

			rho = azmea->ka * sqrt (azmea->qp - q);

			x = rho * sin_del_lng;
			y = -rho * cos_del_lng;
			break;

		case cs_AZMEA_SOUTH:

			rho = azmea->ka * sqrt (azmea->qp + q);

			x = rho * sin_del_lng;
			y = rho * cos_del_lng;
			break;

		default:

			rtn_val = cs_CNVRT_DOMN;
			x = cs_Mhuge;
			y = cs_Mhuge;
		}										/*lint !e744 */
	}
	
	/* Add the rotation, adjust for the selected quadrant,
	   and, finally, add false easting and false northing. */

	if (azmea->Az != 0.0)
	{
		xy [XX] = azmea->cos_Az * x - azmea->sin_Az * y;		/*lint !e644 */
		xy [YY] = azmea->cos_Az * y + azmea->sin_Az * x;		/*lint !e644 */
	}
	else
	{
		xy [XX] = x;											/*lint !e644 */
		xy [YY] = y;											/*lint !e644 */
	}

	if (azmea->quad == 0)
	{
		xy [XX] += azmea->x_off;
		xy [YY] += azmea->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],azmea->x_off,azmea->y_off,
							  azmea->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSazmeaI (azmea,ll,xy);
**
**	struct cs_Azmea_ *azmea;	structure which defines the parameters
**								in effect for the transformation.
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, are returnrd here.
**	double xy [2];				the coordinates to be converted, x ([0])
**								and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**							projection.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 182-190.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSazmeaI (Const struct cs_Azmea_ *azmea,double ll [2],Const double xy [2])
{
	extern double cs_Radian;		/*  57.29577..... */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_NPTest;		/* 0.001 seconds of arc
									   short of the north pole,
									   in radians. */

	int rtn_val;

	double x;
	double y;

	double c;
	double ce;
	double rho;
	double lat;
	double del_lng;
	double sin_c;
	double cos_c;
	double sin_ce = 0.0;		/* initialized to keep gcc happy */
	double cos_ce = 0.0;		/* initialized to keep gcc happy */
	double rho_sq;
	double beta;

	double tmp1;
	double tmp2;

	double my_xy [2];

	rtn_val = cs_CNVRT_NRML;

	if (azmea->quad == 0)
	{
		x = xy [XX] - azmea->x_off;
		y = xy [YY] - azmea->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,azmea->x_off,azmea->y_off,azmea->quad);
	}
	if (azmea->Az != 0.0)
	{
		my_xy [XX] = x;
		my_xy [YY] = y;
		x = azmea->cos_Az * my_xy [XX] + azmea->sin_Az * my_xy [YY];
		y = azmea->cos_Az * my_xy [YY] - azmea->sin_Az * my_xy [XX];
	}

	/* If x and y are now both zero, or very close to it,
	   we must just set the result to the origin of the
	   projection.  The arctan of (0/0) is indeterminant. */

	if (fabs (x) < azmea->one_mm && fabs (y) < azmea->one_mm)
	{
		del_lng = cs_Zero;
		lat = azmea->org_lat;
	}
	else if (azmea->ecent == 0.0)
	{
		/* Here for the sphere.  Note, we have already
		   filtered out cases where rho is zero (or very
		   close to zero). C is the angular distance from
		   the origin to the point.  Thus, a value of
		   rho which is greater than two_ka is outside
		   the range of the coordinate system. */

		rho = sqrt (x * x + y * y);
		if (rho > azmea->two_ka)
		{
			rtn_val = cs_CNVRT_RNG;
			rho = azmea->two_ka;
		}
		c = cs_Two * asin (rho / azmea->two_ka);
		sin_c = sin (c);
		cos_c = cos (c);

		tmp1 = cos_c * azmea->sin_org_lat;
		tmp2 = y * sin_c * azmea->cos_org_lat / rho;
		lat = asin (tmp1 + tmp2);

		switch (azmea->polar) {

		case cs_AZMEA_OBLIQUE:

			/* We've dealt with rho = 0 above, thus at
			   least one argument to atan2 will be
			   non-zero. */

			tmp1 = x * sin_c;
			tmp2 = (rho * azmea->cos_org_lat * cos_c) -
			       (y * azmea->sin_org_lat * sin_c);
			del_lng = atan2 (tmp1,tmp2);

			tmp1 = cos_c * azmea->sin_org_lat;
			tmp2 = y * sin_c * azmea->cos_org_lat / rho;
			lat = asin (tmp1 + tmp2);
			break;

		case cs_AZMEA_EQUATOR:

			/* Same as above, but cos_org_lat = 1.0 and
			   sin_org_lat = 0.0 */

			tmp1 = x * sin_c;
			tmp2 = rho * cos_c;
			del_lng = atan2 (tmp1,tmp2);

			tmp1 = y * sin_c / rho;
			lat = asin (tmp1);
			break;

		case cs_AZMEA_NORTH:

			del_lng = atan2 (x,-y);
			lat = asin (cos_c);
			break;

		case cs_AZMEA_SOUTH:

			del_lng = atan2 (x,y);
			lat = asin (-cos_c);
			break;
		
		default:
		
			rtn_val = cs_CNVRT_NRML;
			del_lng = cs_Zero;
			lat = cs_Zero;
			break;
		}										/*lint !e744 */
	}
	else
	{
		/* Here for the ellisoid. */

		if (azmea->polar == cs_AZMEA_OBLIQUE ||
		    azmea->polar == cs_AZMEA_EQUATOR)
		{
			/* D is zero only if the origin is one of the
			   poles, not the case here.  In the normal
			   case, D is a value very close to 1. Thus,
			   if x and y weren't zero above, rho won't
			   be zero here. */

			tmp1 = x / azmea->D;
			tmp2 = y * azmea->D;
			rho_sq = tmp1 * tmp1 + tmp2 * tmp2;
			rho =  sqrt (rho_sq);

			if (rho > azmea->two_Rq)
			{
				rtn_val = cs_CNVRT_RNG;
				rho = azmea->two_Rq;
			}

			ce = cs_Two * asin (rho / azmea->two_Rq);
			sin_ce = sin (ce);
			cos_ce = cos (ce);

			tmp1 = cos_ce * azmea->sin_beta1;
			tmp2 = azmea->D_cos_b1 * y * sin_ce / rho;
			beta = asin (tmp1 + tmp2);
		}
		else
		{
			rho_sq = x * x + y * y;
			if (rho_sq > azmea->two_asq)
			{
				rtn_val = cs_CNVRT_RNG;
				rho_sq = azmea->two_asq;
			}
			rho = sqrt (rho_sq);

			beta = asin (cs_One - (rho_sq / azmea->asq_qconv));

			/* Adjust the sign for the north/south aspects. */

			beta *= azmea->sin_org_lat;
		}

		switch (azmea->polar) {

		case cs_AZMEA_OBLIQUE:
		case cs_AZMEA_EQUATOR:

			tmp1 = x * sin_ce;							/*lint !e644 */
			tmp2 = azmea->D_cos_b1 * rho * cos_ce;		/*lint !e644 */
			tmp2 -= azmea->Dsq_sin_b1 * y * sin_ce;
			del_lng = atan2 (tmp1,tmp2);
			break;

		case cs_AZMEA_NORTH:

			del_lng = atan2 (x,-y);
			break;

		case cs_AZMEA_SOUTH:

			del_lng = atan2 (x,y);
			break;
		
		default:
		
			rtn_val = cs_CNVRT_NRML;
			del_lng = cs_Zero;
			break;
		}												/*lint !e744 */
		lat = CSbtIcalPrec (&azmea->btcoefI,beta);
	}

	/* Convert the results to degrees.  If the resulting point is
	   a pole, the longitude is quite arbitrary. */

	if (fabs (lat) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
	    	rtn_val = cs_CNVRT_INDF;
	}
	ll [LNG] = (del_lng + azmea->org_lng) * cs_Radian;	/*lint !e644 */
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSazmeaC (azmea,ll);
**
**	struct cs_Azmea_ *azmea;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert Azimuthal
**								Equal Area projection.
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

double EXP_LVL9 CSazmeaC (Const struct cs_Azmea_ *azmea,Const double ll [2])
{
	extern double cs_Radian;		/* 57.2957... */
	extern double cs_Km360;			/* -360.0, the value which
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
	status = CSazmeaF (azmea,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSazmeaF (azmea,xy2,my_ll);
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
**	kk = CSazmeaK (azmea,ll);
**
**	struct cs_Azmea_ *azmea;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal Equal
**								Area projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the normal grid scale of the
**								coordinate system at the specified point.
**
**	This function calculates what is referred to as the
**	K prime scale factor.  That is, the grid scale factor
**	along a geodesic which is normal to the radial line
**	from the projection origin.
**********************************************************************/

double EXP_LVL9 CSazmeaK (Const struct cs_Azmea_ *azmea,Const double ll [2])
{
	extern double cs_Pi_o_2;		/* PI / 2.0 */
	extern double cs_Pi_o_4;		/* PI / 4.0 */
	extern double cs_Degree;		/* 1.0 / 57.29577... */
	extern double cs_Radian;		/* 57.29577... */
	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Mone;			/* -1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_SclInf;		/* 9.9E+04, the value we
									   return for infinite
									   scale. */
	extern double cs_NPTest;		/* 0.001 seconds of arc
									   short of the north pole,
									   in radians. */
	extern double cs_SPTest;		/* 0.001 seconds of arc
									   short of the south pole,
									   in radians. */
	extern double cs_AnglTest;		/* 0.001 seconds of arc, in
									   radians. */

	int status;

	double q;
	double m;
	double kk;
	double lng;
	double lat;
	double tmp;
	double sin_sq;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double xy_dd;
	double ll_dd;
	double del_xx;
	double del_yy;

	double oll [2];
	double ll1 [2];
	double ll2 [2];
	double xy1 [2];
	double xy2 [2];

	/* This is common stuff. */

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		lat = fmod (lat,cs_Pi_o_2);
	}

	if (azmea->ecent == 0.0)
	{
		/* Here for the spherical case.  See if we have
		   a polar aspect. */

		switch (azmea->polar) {

		case cs_AZMEA_NORTH:

			if (lat > cs_SPTest)
			{
				kk = cs_One / cos (cs_Pi_o_4 - (lat * cs_Half));
			}
			else
			{
				kk = cs_SclInf;
			}
			break;

		case cs_AZMEA_SOUTH:

			if (lat < cs_NPTest)
			{
				kk = cs_One / sin (cs_Pi_o_4 - (lat * cs_Half));
			}
			else
			{
				kk = cs_SclInf;
			}
			break;

		case cs_AZMEA_EQUATOR:
		case cs_AZMEA_OBLIQUE:

			lng = ll [LNG] * cs_Degree;
			del_lng = lng - azmea->org_lng;
			tmp = azmea->cos_org_lat * cos (lat) * cos (del_lng);
			tmp += cs_One + (azmea->sin_org_lat * sin (lat));
			if (tmp > cs_AnglTest)
			{
				kk = sqrt (cs_Two / tmp);
			}
			else
			{
				kk = cs_SclInf;
			}
			break;

		default:

			kk = cs_SclInf;
			break;
		}
	}
	else
	{
		/* Here for the ellipsoid case. */

		switch (azmea->polar) {

		case cs_AZMEA_NORTH:
		case cs_AZMEA_SOUTH:

			sin_lat = sin (lat);
			cos_lat = cos (lat);
			q = CSazmeaqq (azmea,sin_lat,&sin_sq);
			if (azmea->polar == cs_AZMEA_NORTH) q = -q;
			m = cos_lat / sqrt (sin_sq);

			/* sin_sq, as returned from CSazmeaqq, is
			   1 - e_sq * sin_sq; so it is never zero.
			   m, however, can be zero. */

			if (fabs (m) > cs_AnglTest)
			{
				kk = sqrt (azmea->qp + q) / m;
			}
			else
			{
				/* We're pretty much at the origin pole. */

				kk = cs_One;
			}
			break;

		case cs_AZMEA_EQUATOR:
		case cs_AZMEA_OBLIQUE:

			/* Here for the equatorial and oblique case.  Until
			   we locate an exact analytical formula, we use the
			   following empirical calculation.

			   CSllnrml computes the lat/long of two points
			   which form a geodesic of one arc second which
			   is normal to the geodesic from the origin
			   (oll) to the point in question (ll). */

			oll [0] = azmea->org_lng * cs_Radian;
			oll [1] = azmea->org_lat * cs_Radian;
			CSllnrml (oll,ll,ll1,ll2);

			/* Convert the resulting lat/longs to cartesian
			   coordinates. */

			status = CSazmeaF (azmea,xy1,ll1);
			if (status != cs_CNVRT_NRML)
			{
				return (cs_Mone);
			}
			status = CSazmeaF (azmea,xy2,ll2);
			if (status != cs_CNVRT_NRML)
			{
				return (cs_Mone);
			}

			/* Calculate the geodetic distance between the two
			   lat/long points.  Note, we provide the geodetic
			   calculator with the scaled radius of the earth
			   so that the distance it computes will be in the
			   same units as the X and Y coordinates. */

			CS_llazdd (azmea->ka,azmea->e_sq,ll1,ll2,&ll_dd);

			/* Calculate the grid distance between the two
			   points. */

			del_xx = xy1 [XX] - xy2 [XX];
			del_yy = xy1 [YY] - xy2 [YY];
			xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

			/* Return the ratio of the two distances as the
			   scale factor (K prime) at the indicated point. */

			if (ll_dd > cs_AnglTest)
			{
				kk = xy_dd / ll_dd;
			}
			else
			{
				kk = cs_SclInf;
			}
			break;

		default:

			kk = cs_SclInf;
			break;
		}
	}
	return (kk);						/*lint !e644 */
}

/**********************************************************************
**	hh = CSazmeaH (azmea,ll);
**
**	struct cs_Azmea_ *azmea;	structure which carries all parameters
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
**	If they were all this easy, I'd be out of a job.
**********************************************************************/

double EXP_LVL9 CSazmeaH (Const struct cs_Azmea_ *azmea,Const double ll [2])
{
	extern double cs_Zero;			/* 0.0 */
	extern double cs_One;			/* 1.0 */
	extern double cs_SclInf;		/* 9.9E+04, the value we
									   return for infinite
									   scale. */

	double kk;
	double hh;

	kk = CSazmeaK (azmea,ll);
	if (kk < 0.0)
	{
		/* The lat/longs are bogus. */

		hh = kk;
	}
	else if (kk >= cs_SclInf)
	{
		/* The kk scale is infinite, thus the hh scale
		   is really zero. */

		hh = cs_Zero;
	}
	else
	{
		/* The normal case. */

		hh = cs_One / kk;
	}
	return (hh);
}
/**********************************************************************
**	status = CSazmeaL (azmea,cnt,pnts);
**
**	struct cs_Azmea_ *azmea;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be checked.
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

int EXP_LVL9 CSazmeaL (Const struct cs_Azmea_ *azmea,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double test_xy [3];

	/* Check all the points.  The domain is continuous, with
	   no singularity points.  Thus, if all the points are
	   OK, all lines, and or regions will be OK. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSazmeaF (azmea,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSazmeaX (azmea,cnt,pnts);
**
**	struct cs_Azmea_ *azmea;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSazmeaL to check lat/long
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

int EXP_LVL9 CSazmeaX (Const struct cs_Azmea_ *azmea,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.  No
	   special checks are required for lines and/or regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSazmeaI (azmea,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
