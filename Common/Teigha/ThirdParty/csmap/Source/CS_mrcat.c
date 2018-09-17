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

#define cs_MRCATPV_MAXLAT  1.4839134260634313982384914804048 /* 85.02 degrees in radians */
#define cs_MRCATPV_MINLAT -1.4839134260634313982384914804048 /* 85.02 degrees in radians */

/**********************************************************************
**	err_cnt = CSmrcatQ (cs_def,prj_code,err_list,list_sz);
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
**	and in the range of greater than or equal to -180/-90, and less
**  than or equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CSmrcatQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_SclRedMin;		/*    0.5 */
	extern double cs_SclRedMax;		/*    1.5 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Mercator specific stuff. */

	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	if (prj_code == cs_PRJCOD_MRCAT)
	{
		/* We use the cosine of the standard parallel quite a lot.  It
		   should not be zero.  The value 80 is an arbitrary value which
		   should several times larger than any normal value.  Theoreticaly,
		   this value could be 89. */
		if (cs_def->prj_prm2 <= -80.0 || cs_def->prj_prm2 >= 80.0)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDLAT;
		}
	}
	else if (prj_code == cs_PRJCOD_MRCATK)
	{
		if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
		}
	}

	/* That's it for Mercator. */

	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CSmrcatS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**								system parameters.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CSmrcatS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Zero;
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/*  Pi, i.e.  3.14159 */
	extern double cs_Mpi;				/* -Pi, i.e. -3.14159 */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km90;				/* -90.0  */
	extern double cs_K90;				/*  90.0  */

	struct cs_Mrcat_ *mrcat;

	double sin_sp;
	double tmp1;

	double ll_test [3];
	double xy_test [3];

	mrcat = &csprm->proj_prms.mrcat;

	mrcat->prj_code = csprm->prj_code;
	mrcat->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
	mrcat->x_off = csprm->csdef.x_off;
	mrcat->y_off = csprm->csdef.y_off;
	mrcat->k = csprm->csdef.scale;
	mrcat->ecent = csprm->datum.ecent;
	mrcat->e_sq = mrcat->ecent * mrcat->ecent;
	mrcat->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Set up the longitude range for this definition.  We need to capture
	   these values here as the CSmrcatI and CSmrcatF functions do not have
	   access to the cs_Csdef_ structure.  These values are used to determine
	   if a longitude which outside the +/- 180 range is to be converted
	   without warning.  Thus, a definition can specify a useful range which
	   extends past the +/- 180 degree crack. */
	if ((fabs (csprm->csdef.ll_max [0]) < 1.0E-12) &&		/* csprm->csdef.ll_max [0] == 0.0 */
		(fabs (csprm->csdef.ll_min [0]) < 1.0E-12))			/* csprm->csdef.ll_min [0] == 0.0 */
	{
		/* A useful range was not specified in the definition.  We default
		   to the standard values. */
		mrcat->eastLimit = cs_Pi;
		mrcat->westLimit = cs_Mpi;
	}
	else
	{
		mrcat->eastLimit = (csprm->csdef.ll_max [0] - csprm->csdef.prj_prm1) * cs_Degree;
		mrcat->westLimit = (csprm->csdef.ll_min [0] - csprm->csdef.prj_prm1) * cs_Degree;
	}

	/* Handle the two (currently) variations to this projection. */

	switch (csprm->prj_code) {

	default:
	case cs_PRJCOD_MRCAT:
		/* Here for the normal case, a standard parallel, usually zero. */
		mrcat->std_lat = csprm->csdef.prj_prm2 * cs_Degree;
		mrcat->cos_sp = cos (mrcat->std_lat);
		if (mrcat->ecent != 0.0)
		{
			sin_sp = sin (mrcat->std_lat);
			tmp1 = cs_One - (mrcat->e_sq * sin_sp * sin_sp);
			mrcat->cos_sp /= sqrt (tmp1);
		}
		break;

	case cs_PRJCOD_MRCATK:
		/* Here for the scale reduction case. */
		mrcat->std_lat = cs_Zero;
		mrcat->cos_sp = csprm->csdef.scl_red;
		break;

	case cs_PRJCOD_MRCATPV:
		/* Here for the Google Earth (Popular Visualisation Pseudo Mercator).
		   We need to use the spherical formulas for the projection
		   calculations, even if the definition is referenced to an ellipsoid.
		   We can't just set mrcat->ecent to zero, as the scale calculations
		   still need the eccentricity.  This sounds very strange, I know.
		   But the definition is still referenced to an ellipsoid, so the
		   scale calculations still include the shape of the spheoid in use.
		   It's the projection calculations that are wierd, essentially a
		   major screw up by Google!!! */
		mrcat->std_lat = cs_Zero;		/* reference parallel and scale reduction not used */
		mrcat->cos_sp = cs_One;			/* reference parallel and scale reduction not used */
		break;
	}

	/* Compute the maximum Y value, i.e. the value we will use
	   for the poles rather than the technically correct
	   infinite value. */

	/* We need to do a little extra for the ellipsoidal case. */

	if (mrcat->ecent != 0.0 && mrcat->prj_code != cs_PRJCOD_MRCATPV)
	{
		/* Set up the latitude power series coefficients. */
		CSchiIsu (&mrcat->chicofI,mrcat->e_sq);
	}

	/* The following apply for either form. */

	mrcat->Rk = csprm->datum.e_rad * mrcat->k;
	mrcat->Rk_ovr_2 = mrcat->Rk * cs_Half;
	mrcat->Rfact = mrcat->Rk * mrcat->cos_sp;
	mrcat->Rfact_2 = mrcat->Rfact * cs_Half;

	/* Compute the maximum Y value, i.e. the value we will use
	   for the poles rather than the technically correct
	   infinite value.  Since the setup is just about complete,
	   we can use the CSmrcatF function to do this. */

	ll_test [LNG] = mrcat->cent_lng;
	ll_test [LAT] = cs_NPTest * cs_Radian;
	CSmrcatF (mrcat,xy_test,ll_test);			/*lint !e534 */
	mrcat->yy_max = xy_test [YY];

	/* Setup the range checking.  Note, these values are considered
	   to be useful range values. */

	csprm->cent_mer = mrcat->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* Here if the definition does not specify; we calculate
		   some reasonable values. */
		csprm->cent_mer = cs_Zero;
		csprm->min_ll [LNG] = cs_Km180;
		csprm->max_ll [LNG] = cs_K180;
		csprm->min_ll [LAT] = cs_Km90;
		csprm->max_ll [LAT] = cs_K90;
	}
	else
	{
		/* Use whatever the user provides, without checking.
		   The user specifies values in absolute terms. */
		csprm->min_ll [LNG] = csprm->csdef.ll_min [LNG] - csprm->cent_mer;
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = csprm->csdef.ll_max [LNG] - csprm->cent_mer;
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the X's and Y's */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [YY] == 0.0)
	{
		/* Here if the definition does not specify; we calculate
		   some reasonable values.  This is one of the few
		   projections where the following works; i.e. both
		   meridians and parallels ae straight lines. */

		ll_test [LNG] = csprm->min_ll [LNG];
		ll_test [LAT] = csprm->min_ll [LAT];
		CSmrcatF (mrcat,xy_test,ll_test);			/*lint !e534 */
		csprm->min_xy [XX] = xy_test [XX] - mrcat->x_off;
		csprm->min_xy [YY] = xy_test [YY] - mrcat->y_off;

		ll_test [LNG] = csprm->max_ll [LNG];
		ll_test [LAT] = csprm->max_ll [LAT];
		CSmrcatF (mrcat,xy_test,ll_test);			/*lint !e534 */
		csprm->max_xy [XX] = xy_test [XX] - mrcat->x_off;
		csprm->max_xy [YY] = xy_test [YY] - mrcat->y_off;

		CS_quadMM (csprm->min_xy,csprm->max_xy,mrcat->x_off,
											   mrcat->y_off,
											   mrcat->quad);
	}
	else
	{
		/* Use whatever the user provides, without checking. */

		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}

	/* That's all the calculations we can do at this time.
	   The projection is conformal, so ha nd k scale
	   factors are the same. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSmrcatF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSmrcatI;
	csprm->cs_scale = (cs_SCALE_CAST)CSmrcatK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSmrcatK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSmrcatH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSmrcatC;
	csprm->llchk    = (cs_LLCHK_CAST)CSmrcatL;
	csprm->xychk    = (cs_XYCHK_CAST)CSmrcatX;

	return;
}

/**********************************************************************
**	rtn_val = CSmrcatF (mrcat,xy,ll);
**
**	struct cs_Mrcat_ *mrcat;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned here, X ([0])
**								and Y ([1]).
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

int EXP_LVL9 CSmrcatF (Const struct cs_Mrcat_ *mrcat,double xy [2],Const double ll [2])
{
	extern double cs_One;				/* 1.0 */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi_o_2;			/* Pi / 2.0 */
	extern double cs_Two_pi;			/* 2 pi */
	extern double cs_3Pi_o_2;			/*  3 pi over 2  */
	extern double cs_NPTest;			/* 0.001 arc seconds
										   short of the north
										   pole in radians. */
	extern double cs_SPTest;			/* 0.001 arc seconds
										   short of the south
										   pole in radians. */
	extern double cs_AnglTest;			/* 4.85E-08; about 30 centimeters on
										   the surface of the earth in radians */

	int rtn_val;

	double lng;
	double lat;
	double del_lng;

	double sin_lat;
	double esin_lat;

	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	/* Adjust the longitude value such that it falls within the range specified
	   in the definition.  We used to adjust so that the longitude was in the
	   +/1 180 range, so as to establish a one to one relationship between
	   the cartesian and sphereical coordinate systems.

	   We now adjust the supplied longitude to be within the longitude range
	   specified in the definition (i.e. the useful range).  Thus, the
	   input longitude can now be less than -180 or greater than +180.
	   
	   The inverse is adjusted to accept easting values which produce
	   such longitudes and thus will produce the original longitude
	   value as long as this longitude is within the specified range.

	   However, once these longitudes go through another transformation,
	   such as a datum transformation, it is likely that the value will
	   again be normailzed to be within +- 180, and this capability
	   will be lost. */

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	/* Deal with obviously obnoxious coordinates which should only be caused
	   by using thewrong coordinate system.  This is done here largely to
	   enable the use of a relatively simple normalization algorithm below,
	   without running the risk of a huge pregnant pause in processing. */
	if (fabs (lng) > cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lng = CS_adj2piI (lng);
	}

	/* Deal with X, it's relatively simple, mathemagically. */
	del_lng = lng - mrcat->cent_lng;
	if (del_lng < mrcat->westLimit)
	{
		rtn_val = cs_CNVRT_RNG;
		while (del_lng < mrcat->westLimit)
		{
			lng += cs_Two_pi;
			del_lng = lng - mrcat->cent_lng;
		}
	}
	if (del_lng > mrcat->eastLimit)
	{
		rtn_val = cs_CNVRT_RNG;
		while (del_lng > mrcat->eastLimit)
		{
			lng -= cs_Two_pi;
			del_lng = lng - mrcat->cent_lng;
		}
	}
	xy [XX] = mrcat->Rfact * del_lng;

	/* Bogus values of latitude can cause big problems.  We are a bit more
	   forgiving in the case of the Pseudo Mercator.  After all, it is a
	   "Pseudo" projection. */
	if (mrcat->prj_code == cs_PRJCOD_MRCATPV)
	{
	    /* We do the following special for the Pseudo Mercator as the
	       google and bing stuff is often used to extract a map/image
	       of the whole world in LL84 type coordinates.  These types
	       of things generally include latitude extents of -90 and
	       +90.  Yet users expecty to convert this to the Pseudo
	       Mercator even though the poles cannot be represented on
	       a Mercator projection.
	       
	       Since it is a "Pseudo" projection, to do the following that
	       users get what they expect even though it is technically
	       incorrect. After all, when everything is said and sone, it's
	       the users who pay for everything. */
	    if (fabs (lat) > (cs_Pi_o_2 + cs_AnglTest))
	    {
	        /* We still consider this condition to indicate a bogus
	           data point. */
		    rtn_val = cs_CNVRT_RNG;
		    lat = CS_adj1pi (lat);
		    if (lat > cs_NPTest)
		    {
			    lat = cs_NPTest;
		    }
		    if (lat < cs_SPTest)
		    {
			    lat = cs_SPTest;
		    }
	    }
        else if (lat > cs_MRCATPV_MAXLAT)
        {
            /* This represents a point above 85 degrees north in latitude.
               There is nothing (land wise) north of 84, so we process
               anything above 85 as being 85, and we do so without
               issuing a domain error. */
            lat = cs_MRCATPV_MAXLAT;
        }
        else if (lat < cs_MRCATPV_MINLAT)
        {
            /* This represents a point below 85 degrees south in latitude.
               While there is land down there, chopping this off at -85
               will enable the Pseudo Mercator to produce the normal
               image of Antarctica that usually appears on Mercator
               maps.  In fact, it might be more cosmetically attractive
               to cut this off at 80 degrees south latitude.  In any
               case, we do this without generating a domain error. */
            lat = cs_MRCATPV_MINLAT;
        }
	}
	else
	{
	    if (fabs (lat) > cs_NPTest)
	    {
		    rtn_val = cs_CNVRT_RNG;
		    lat = CS_adj1pi (lat);
		    if (lat > cs_NPTest)
		    {
			    lat = cs_NPTest;
		    }
		    if (lat < cs_SPTest)
		    {
			    lat = cs_SPTest;
		    }
	    }
	}

	/* We should be OK now. */

	sin_lat = sin (lat);
	tmp1 = (cs_One + sin_lat) / (cs_One - sin_lat);

	if (mrcat->ecent == 0.0 || mrcat->prj_code == cs_PRJCOD_MRCATPV)
	{
		/* Here for a sphere. */
		
		xy [YY] = mrcat->Rfact_2 * log (tmp1);
	}
	else
	{
		/* Here for an ellipsoid. */

		esin_lat = mrcat->ecent * sin_lat;
		tmp2 = (cs_One - esin_lat) / (cs_One + esin_lat);
		tmp2 = tmp1 * pow (tmp2,mrcat->ecent);
		xy [YY] = mrcat->Rfact_2 * log (tmp2);
	}

	if (mrcat->quad == 0)
	{
		xy [XX] += mrcat->x_off;
		xy [YY] += mrcat->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],mrcat->x_off,mrcat->y_off,mrcat->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSmrcatI (mrcat,ll,xy);
**
**	struct cs_Mrcat_ *mrcat;	structure detailing the Mercator coordinate
**								system in use.
**	double ll [2];				converted results returned here, longitude ([0])
**								and latitude ([1]), in degrees.
**	double xy [2];				coordinates to be converted are obtained from
**								this array, X ([0]) and Y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The xy and ll arrays may share the same memory.
**********************************************************************/

int EXP_LVL9 CSmrcatI (Const struct cs_Mrcat_ *mrcat,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Pi_o_2;			/* Pi / 2.0 */
	extern double cs_3Pi_o_2;			/* 3 Pi / 2.0 */
	extern double cs_Two;				/* 2.0 */

	int rtn_val;

	double xx;
	double yy;

	double chi;
	double lat;
	double del_lng;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	/* Remove whatever offsets are active. */

	if (mrcat->quad == 0)
	{
		xx = xy [XX] - mrcat->x_off;
		yy = xy [YY] - mrcat->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,mrcat->x_off,mrcat->y_off,mrcat->quad);
	}

	/* Check the Y value for range. */

	if (fabs (yy) > mrcat->yy_max)
	{
		rtn_val = cs_CNVRT_RNG;
		yy = (yy >= 0.0) ? mrcat->yy_max : -mrcat->yy_max;
	}

	/* The longitude calculation is the same for both the
	   spherical and ellipsoidal cases.  There may be a
	   slight difference if the standard parallel is not
	   the equator, but this is taken care of during set up
	   and shows up in the Rfact variable. */

	del_lng = xx / mrcat->Rfact;
	if (fabs (del_lng) >= cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	/* The following is used for sphere and ellipsoid. */

	tmp1 = exp (-yy / mrcat->Rfact);
	chi = cs_Pi_o_2 - cs_Two * atan (tmp1);

	/* Finish off the latitude as appropriate. */

	if (mrcat->ecent == 0.0 || mrcat->prj_code == cs_PRJCOD_MRCATPV)
	{
		/* Here for a sphere. */

		lat = chi;
	}
	else
	{
		/* Here for an ellipsoid.  This is a series
		   expansion used in other projections, so we
		   have a function to do this for us. */

		lat = CSchiIcal (&mrcat->chicofI,chi);
	}

	ll [LNG] = (del_lng + mrcat->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	cnvrg = CSmrcatK (mrcat,ll);
**
**	struct cs_Mrcat_ *mrcat;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**			degrees.
**	double cnvrg;				the convergence angle of the projection at the
**								specified point.
**
**	I wish they they we all this easy.
**********************************************************************/

double EXP_LVL9 CSmrcatC (Const struct cs_Mrcat_ *mrcat,Const double ll [2])
{
	extern double cs_Zero;			/* 0.0 */

	/* Since all meridians are parallel with the central
	   meridian and the Y axis, the convergence angle is
	   zero for all locations. */

	return (cs_Zero);
}							/*lint !e715 */

/**********************************************************************
**	kk = CSmrcatK (mrcat,ll);
**
**	struct cs_Mrcat_ *mrcat;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**			degrees.
**	double kk;				the true map scale factor at the specified point.
**								Should be compared with the mrcat->k.
**********************************************************************/

double EXP_LVL9 CSmrcatK (Const struct cs_Mrcat_ *mrcat,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_One;				/* 1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */
	extern double cs_NPTest;			/* .001 arc second short
										   of the north pole in
										   radians. */

	double kk;
	double lat;

	double cos_lat;
	double sin_lat;
	double tmp1;

	lat = ll [LAT] * cs_Degree;

	if (fabs (lat) > cs_NPTest)
	{
		/* Note this makes the divide by cosine (lat) below safe. */
		kk = cs_SclInf;
	}
	else
	{
		cos_lat = cos (lat);

		/* cos_lat should never be zero here. */

		if (mrcat->ecent == 0.0)
		{
			/* A sphere. */

			kk = mrcat->cos_sp / cos_lat;
		}
		else
		{
			/* An ellipsoid.  This code is also valid for the Popular
			   Visualization Pseudo Meractor (i.e. cos_sp == 1.0). */

			sin_lat = sin (lat);
			tmp1 = cs_One - (mrcat->e_sq * sin_lat * sin_lat);
			kk = mrcat->cos_sp * sqrt (tmp1) / cos_lat;
		}
		if (kk > cs_SclInf) kk = cs_SclInf;
	}
	return (kk);
}

/**********************************************************************
**	kk = CSmrcatH (mrcat,ll);
**
**	struct cs_Mrcat_ *mrcat;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**			degrees.
**	double hh;				the true map scale factor at the specified point.
**								Should be compared with the mrcat->k.
**********************************************************************/

double EXP_LVL9 CSmrcatH (Const struct cs_Mrcat_ *mrcat,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_One;				/* 1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */
	extern double cs_NPTest;			/* .001 arc second short
										   of the north pole in
										   radians. */

	double hh;
	double lat;

	double sin_lat;
	double rho;				/* radius of curvature in the merional section, sans the semi-major radius */
	double tmp1;

	if (mrcat->prj_code == cs_PRJCOD_MRCATPV)
	{
		lat = ll [LAT] * cs_Degree;
		if (fabs (lat) > cs_NPTest)
		{
			/* This also insures that cosine (lat) will never be zero below. */
			hh = cs_SclInf;
		}
		else
		{
			/* The Popular Visualisation Pseudo Mercator is not conformal. */
			sin_lat = sin (lat);
			tmp1 = cs_One - (mrcat->e_sq * sin_lat * sin_lat);
			rho = (cs_One - mrcat->e_sq) / (tmp1 * sqrt (tmp1));
			hh = cs_One / (rho * cos (lat));
			if (hh > cs_SclInf)
			{
				/* Very close to either pole. */
				hh = cs_SclInf;
			}
		}
	}
	else
	{
		/* All variations of the Mercator are conformal, except the Popular
		   Visualisation Pseudo Mercator; so we just use the 'K' calculation. */
		hh = CSmrcatK (mrcat,ll);
	}
	return (hh);
}

/**********************************************************************
**	status = CSmrcatL (mrcat,cnt,pnts);
**
**	struct cs_Mrcat_ *mrcat;	the coordinate system against which the check is
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

int EXP_LVL9 CSmrcatL (Const struct cs_Mrcat_ *mrcat,int cnt,Const double pnts [][3])

{
	extern double cs_Degree;
	extern double cs_NPTest;			/* .001 seconds of arc less
										   than the North pole in
										   radians. */
	int ii;
	int status;

	double lat;

	/* Check all the points.  As long as none have a latitude
	   beyond either pole, we're OK. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_NPTest)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);

	/* No special test are required for lines and or regions. */
}							/*lint !e715 */

/**********************************************************************
**	status = CSmrcatX (mrcat,cnt,pnts);
**
**	struct cs_Mrcat_ *mrcat;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSmrcatL to check lat/long
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

int EXP_LVL9 CSmrcatX (Const struct cs_Mrcat_ *mrcat,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double test_val;
	double dummy;

	rtn_val = cs_CNVRT_OK;

	/* All X values are within the domain of the function.  However,
	   in the cartesian frame of reference, an absolute value of
	   Y greater than yy_max is outside of the domain. */

	for (ii = 0;ii < cnt && rtn_val == cs_CNVRT_OK;ii++)
	{
		if (mrcat->quad == 0)
		{
			test_val = pnts [ii][YY] - mrcat->y_off;
		}
		else
		{
			CS_quadI (&test_val,&dummy,pnts [ii],mrcat->x_off,
												 mrcat->y_off,
												 mrcat->quad);
		}

		if (fabs (test_val) > mrcat->yy_max)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
/* Given the ecentricity of an ellipsoid, compute the latitude, in degrees,
   of the standard parallel which is euivalent to the provided scale reduction,
   ala Mercator.
  
   Essentially, the inverse (converted to degrees) of the following:
   
   k = cos (lat) / sqrt (1.0 - e_sq * sin (lat) * sin (lat));
*/
double EXP_LVL5 CSmrcatPhiFromK (double e_sq,double scl_red)
{
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Radian;
	extern double cs_K90;
	
	int itrCnt;
	double newK;
	double deltaK;
	double stdLat;
	double phi;
	double cosPhi;
	double sinPhi;

	stdLat = cs_K90;
	if (fabs (scl_red - cs_One) < 1.0E-10)
	{
		stdLat = cs_Zero;
	}
	else if (scl_red > cs_Zero && scl_red < cs_One)
	{
		itrCnt = 10;			/* usually converges in 4 iterations */
		phi = acos (scl_red);	/* First guess */
		while (itrCnt-- > 0)
		{
			cosPhi = cos (phi);
			sinPhi = sin (phi);
			newK = cosPhi / sqrt (cs_One - e_sq * sinPhi * sinPhi);
			deltaK = newK - scl_red;
			if (fabs (deltaK) < 1.0E-13)
			{
				stdLat = phi * cs_Radian;
				break;
			}
			phi = acos (cosPhi - deltaK);
		}
	}
	return stdLat;
}
