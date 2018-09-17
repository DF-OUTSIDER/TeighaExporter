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
**	err_cnt = CStrmerQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	indicates the specific variation for which the
**								parameters are to be checked.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;				the size of the array pointed to be err_list,
**								may be zero.
**	int pnt_cnt;				the number of points used to define the great
**								circle:   point and azimuth := pnt_cnt = 1;
**								two points := mode = 2.
**	int rect_flg;				TRUE says the coordinate system is rectified to
**								cartesian X and Y, were Y is due north.
**	int err_cnt;				returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greennwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**
**	The rectification flag has no effect on the validity of the
**	coordinate system as far as we know.  However, we have included
**	it in thew calling sequence in case we get smart some time in
**	the future.
**********************************************************************/

int EXP_LVL9 CSoblqmQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_Km180;			/* -180.0 */
	extern double cs_K360;			/*  360.0 */
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLngFz;		/* + 179.99 */
	extern double cs_MinLatFz;		/* - 89.99 */
	extern double cs_MaxLatFz;		/* + 89.99 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */
	extern double cs_ParmTest;		/* .1 seconds of arc in degrees */

	int err_cnt;

	int one_pnt;

	/* Extract the essentials of the specific variation at hand. */

	switch (prj_code) {
	
	case cs_PRJCOD_HOM2UV:
	case cs_PRJCOD_HOM2XY:
		one_pnt = FALSE;
		break;

	default:
	case cs_PRJCOD_HOM1UV:
	case cs_PRJCOD_HOM1XY:
	case cs_PRJCOD_RSKEW:
	case cs_PRJCOD_RSKEWC:
	case cs_PRJCOD_RSKEWO:
		one_pnt = TRUE;
		break;
	}

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* We could check for the consistency of the variations. However, since
	   this is all hard coded above, such as error would be a software
	   error and not a user error. Therefore, we trust the programmer and
	   ignore such checks. */

	/* Check the Oblique Mercator specific stuff.  Prj_prm1 and prj_prm2
	   are always a lat long pair.  Can't allow a pole for the
	   latitude. */
	   
	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LNG;
	}
	if (cs_def->prj_prm2 <= cs_MinLatFz || cs_def->prj_prm2 >= cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LAT;
	}

	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* The stuff specific to the two alternatives. */
	
	if (one_pnt)
	{
		/* In this case, prj_prm3 is an azimuth.  We treat it like
		   a longitude.  However, values of 0.0, +-90.0, and +-180.0
		   are not good.
		   
		   Actually, values of +- 90 should be OK. However, we reject
		   them here and suggest that the user use Swiss Oblique Mercator. */

		if (fabs (cs_def->prj_prm3) < cs_ParmTest ||
				  cs_def->prj_prm3 >= cs_K360 ||
				  cs_def->prj_prm3 <= cs_Km180)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_AZM;
		}
		if (fabs (fabs (cs_def->prj_prm3) - cs_MaxLatFz) < cs_ParmTest ||
			fabs (fabs (cs_def->prj_prm3) - cs_MaxLngFz) < cs_ParmTest)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_USESW;
		}
		
		/* Also, the great circle defined must not experience maximum
		   or minimum latitude at the defined point.  This will be
		   tricky.  I don't know how to check that yet. */
	}
	else
	{
		if (cs_def->org_lat <= cs_MinLatFz || cs_def->org_lat > cs_MaxLatFz)
		{
			/* Can't allow either pole. */
			
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LAT;
		}
		if (cs_def->prj_prm3 <= cs_MinLng || cs_def->prj_prm3 > cs_MaxLng)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LNG;
		}
		if (cs_def->prj_prm4 <= cs_MinLatFz || cs_def->prj_prm4 > cs_MaxLatFz)
		{
			/* Can't allow either pole. */
			
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LAT;
		}
		if (fabs (cs_def->prj_prm2) < cs_ParmTest)
		{
			/* First point can't be on the equator. */
			
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LAT;
		}
		if (fabs (cs_def->prj_prm2 - cs_def->prj_prm4) <= cs_ParmTest)
		{
			/* Latitudes can't be equal; use Mercator. */

			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_MRCAT;
		}

		/* Compute the angular distance from point 1 to point2.
		   If this distance is greater than 90 degrees, we probably
		   have a problem.  We use spherical trig for this calculation.
		   That should be close enough. */

		/* Compute the azimuth between the two points.  An azimuth
		   of 0.0 or +- 90.0 or +-180.0 is also unsupported. */

		/* Now for a toughie.  In traverseing the great circle
		   from point 1 to point2, we may not expericence a
		   maximum, or minimum, of latitude.  Think about it,
		   this is possible with an oblique great circle.  Don't
		   know how to test this just yet; but we'll think
		   about it. */
	}

	/* That's it for the Oblique Mercator. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSoblqmS (csprm,mode,rect_flg);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**	int mode;					specifies the number of points used to
**								define the projection.  1 means one
**								point and an azimuth, 2 means two points
**								and a central latiude.
**	int rect_flg;				TRUE indicates that rectified X and Y coordinates
**								are to be returned.  FALSE indicates that U and V
**								coordinates are to be returned.
**
**	The Oblique Mercator used is the Hotine version thereof.
**
**	There are several variations of this projection. The specific
**	variation desired is specified by the prj_code element of the
**	cs_Csprm_ structure which is passed as the only argument to
**	this function. There are six unique values of prj_code which
**	are understood by this module:
**
**	cs_PRJCOD_HOM1UV
**		Defined by a single point and an azimuth, the
**		projection produces unrectified UV cooridnates with
**		the origin at the intersection of the central
**		geodesic with the equator of the aposphere. Not
**		a very useful projection, but preserved for
**		historical reasons.
**	cs_PRJCOD_HOM2UV
**		Defined by two points, the projection the
**		projection produces unrectified UV cooridnates with
**		the origin at the intersection of the central
**		geodesic with the equator of the aposphere. Not
**		a very useful projection, but preserved for
**		historical reasons.
**	cs_PRJCOD_HOM1XY
**		Defined by a single point and an azimuth, the
**		projection produces rectified X and Y coordinates
**		with the origin at the intersection of the central
**		geodesic with the equator of the aposphere. This is
**		the variation which produces the correct results for
**		Zone 1 of the Alaska state plane system.
**	cs_PRJCOD_HOM2XY
**		Defined by two points, the projection the
**		projection produces rectified XY cooridnates with
**		the origin at the intersection of the central
**		geodesic with the equator of the aposphere.  This
**		variation uses the azimuth of the central line at
**		the projection origin (i.e. the single point) as
**		the rotation angle in the rectification process.
**		Not a very useful projection, but preserved for
**		historical reasons.
**	cs_PRJCOD_RSKEW
**		Defined by a single point and an azimuth. The
**		projection produces rectified XY coordinates with
**		the origin at the intersection of the central
**		geodesic and the equator of the aposphere. This
**		is the variation which produces the results most
**		folks expect from a Rectified Skew Orthomorphic
**		projection.  Note, that this variation differs
**		from the RSKEWO variation as the azimuth
**		parameter is the asimuth of the central geodesic
**		at the projection center (rather than the
**		projection origin).
**	cs_PRJCOD_RSKEWC
**		Defined by a single point and an azimuth. The
**		projection produces rectufued XY coordinates with
**		the origin at the central point (i.e. the single
**		defining point). This is the variation which would
**		be most useful to a cartographer defining his/her
**		own coordinate system.
**	cs_PRJCOD_RSKEWO
**		Defined by a single point and an azimuth. The
**		projection produces rectified XY coordinates with
**		the origin at the central point (i.e. the single
**		defining point).  This variation is the same as
**		the popular RSKEW with the single difference being
**		that the azimuth parameter is the Skew Azimuth of
**		the central geodesic at the projection origin
**		(rather than the projection center).
**********************************************************************/

void EXP_LVL9 CSoblqmS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577...       */
	extern double cs_Pi;				/*  3.14159...       */
	extern double cs_Pi_o_2;			/* pi over two       */
	extern double cs_Zero;				/* 0.0               */
	extern double cs_Half;				/* 0.5               */
	extern double cs_One;				/* 1.0               */
	extern double cs_Two;				/* 2.0               */
	extern double cs_Ten;				/* 10.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   short of pi/2 in
										   radians. */
	extern double cs_AnglTest1;			/* 1.0 - cs_AnglTest */

	struct cs_Oblqm_ *oblqm;

	double t_0;
	double t_1;
	double t_2;
	double D, Dsq;
	double F;
	double G;
	double H;
	double J;
	double L;
	double P;
	double cos_lat_0;
	double sin_lat_0;

	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;
	double tmp5;
	double tmp6;
	double zone_wd;

	oblqm = &csprm->proj_prms.oblqm;

	/* Extract the essentials of the specific variation at hand. */

	switch (csprm->prj_code) {
	
	case cs_PRJCOD_HOM1UV:
		oblqm->one_pnt = TRUE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_UNRECT;
		oblqm->azmIsSkew = FALSE;
		break;
	
	case cs_PRJCOD_HOM1XY:
		oblqm->one_pnt = TRUE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_ALASKA;
		oblqm->azmIsSkew = FALSE;
		break;
	
	case cs_PRJCOD_HOM2UV:
		oblqm->one_pnt = FALSE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_UNRECT;
		oblqm->azmIsSkew = FALSE;
		break;
	
	case cs_PRJCOD_HOM2XY:
		oblqm->one_pnt = FALSE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_ALASKA;  /* for historical purposes */
		oblqm->azmIsSkew = FALSE;
		break;

	default:
	case cs_PRJCOD_RSKEW:
		oblqm->one_pnt = TRUE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_RECT;
		oblqm->azmIsSkew = FALSE;
		break;
	
	case cs_PRJCOD_RSKEWC:
		oblqm->one_pnt = TRUE;
		oblqm->uv_ctr = TRUE;
		oblqm->rect_flg = csOBLQM_RECT;
		oblqm->azmIsSkew = FALSE;
		break;

	case cs_PRJCOD_RSKEWO:
		oblqm->one_pnt = TRUE;
		oblqm->uv_ctr = FALSE;
		oblqm->rect_flg = csOBLQM_RECT;
		oblqm->azmIsSkew = TRUE;
		break;
	}
	
	/* Transfer the necessary arguments to the "oblqm"
	   structure. */

	if (oblqm->one_pnt)
	{
		/* In the one point mode. */

		oblqm->lng_c = csprm->csdef.prj_prm1 * cs_Degree;
		oblqm->lat_c = csprm->csdef.prj_prm2 * cs_Degree;
		oblqm->lng_0 = oblqm->lng_c;
		oblqm->lat_0 = oblqm->lat_c;
		/* Legacy dictates that parm3 is the azimuth of the cntral line
		   at the projection origin.  As of 11.03, parm3 could be
		   the "skew azimuth of the rectified grid" at the projection
		   origin. */
		oblqm->az = csprm->csdef.prj_prm3 * cs_Degree;
		oblqm->gamma_0 = csprm->csdef.prj_prm3 * cs_Degree;
	}
	else
	{
		/* In the two point mode. */

		oblqm->lng_1 = csprm->csdef.prj_prm1 * cs_Degree;
		oblqm->lat_1 = csprm->csdef.prj_prm2 * cs_Degree;
		oblqm->lng_2 = csprm->csdef.prj_prm3 * cs_Degree;
		oblqm->lat_2 = csprm->csdef.prj_prm4 * cs_Degree;
		oblqm->lng_c = (oblqm->lng_1 + oblqm->lng_2) * cs_Half;
		oblqm->lat_c = csprm->csdef.org_lat * cs_Degree;
		oblqm->lat_0 = oblqm->lat_c;
	}

	oblqm->k0 = csprm->csdef.scl_red;
	oblqm->k = csprm->csdef.scale * csprm->csdef.scl_red;
	oblqm->x_off = csprm->csdef.x_off;
	oblqm->y_off = csprm->csdef.y_off;
	oblqm->uc = cs_Zero;					/* until we know different */
	oblqm->ecent = csprm->datum.ecent;
	oblqm->e_sq = oblqm->ecent * oblqm->ecent;
	oblqm->e_rad = csprm->datum.e_rad;
	oblqm->ak = oblqm->e_rad * oblqm->k;
	oblqm->e_ovr_2 = oblqm->ecent * cs_Half;
	oblqm->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Much of the following is used only in the spherical mode,
	   but is useful stuff for general computations, such as
	   limit checking. */

	if (oblqm->one_pnt == 1)
	{
		/* If we've been given one point and an azimuth.  Remember, much of
		   this is valid only for the spherical case. */

		oblqm->sin_az = sin (oblqm->az);
		oblqm->cos_az = cos (oblqm->az);

		tmp1 = sin (oblqm->lat_c);
		tmp2 = cos (oblqm->lat_c);
		oblqm->lat_p = asin (tmp2 * oblqm->sin_az);
		tmp3 = -tmp1 * oblqm->sin_az;
		tmp4 = atan2 (-oblqm->cos_az,tmp3) + oblqm->lng_c;
		oblqm->lng_p = CS_adj2pi (tmp4);
	}
	else
	{
		/* If we've been given two points and a
		   central latitude. */

		tmp1 = cos (oblqm->lat_1) * sin (oblqm->lat_2) * cos (oblqm->lng_1);
		tmp2 = sin (oblqm->lat_1) * cos (oblqm->lat_2) * cos (oblqm->lng_2);
		tmp3 = sin (oblqm->lat_1) * cos (oblqm->lat_2) * sin (oblqm->lng_2);
		tmp4 = cos (oblqm->lat_1) * sin (oblqm->lat_2) * sin (oblqm->lng_1);

		oblqm->lng_p = atan2 (tmp1 - tmp2,tmp3 - tmp4);
		oblqm->lng_p = CS_adj2pi (oblqm->lng_p);

		tmp5 = oblqm->lng_p - oblqm->lng_1;
		tmp5 = -cos (tmp5) / tan (oblqm->lat_1);
		oblqm->lat_p = atan (tmp5);

		tmp1 = CS_adj2pi (oblqm->lng_2 - oblqm->lng_1);
		tmp2 = sin (tmp1 * cs_Half);
		tmp2 = tmp2 * tmp2;
		tmp3 = CS_adj1pi (oblqm->lat_2 - oblqm->lat_1) * cs_Half;
		tmp3 = sin (tmp3);
		tmp3 = tmp3 * tmp3;
		tmp4 = tmp3 + cos (oblqm->lat_2) * cos (oblqm->lat_1) * tmp2;
		tmp4 = sqrt (tmp4);
		tmp4 = sin (cs_Two * asin (tmp4));

		oblqm->sin_az = sin (tmp1) * cos (oblqm->lat_2) / tmp4;

		tmp5 = sin (oblqm->lat_1) * cos (oblqm->lat_2) * cos (tmp1);
		oblqm->cos_az = cos (oblqm->lat_1) * sin (oblqm->lat_2) - tmp5;
		oblqm->cos_az /= tmp4;
	}

	/* Select the northern most pole. */

	if (oblqm->lat_p < 0.0)
	{
		oblqm->lat_p = -oblqm->lat_p;
		oblqm->lng_p += CS_adj2pi (oblqm->lng_p + cs_Pi);
	}

	/* Compute the sine and cosine of the pole latitude. */

	oblqm->sin_lat_p = sin (oblqm->lat_p);
	oblqm->cos_lat_p = cos (oblqm->lat_p);

	if (oblqm->ecent == 0.0)
	{
		/* lat_0,lng_0 is the point where the great circle
		   intersects the equator.  This is the origin of
		   the U V system of coordinates in the spherical
		   case. */

		tmp1 = oblqm->lng_p + cs_Pi_o_2;
		oblqm->lng_0 = CS_adj2pi (tmp1);
		oblqm->lat_0 = cs_Zero;

		/* Vv_max is what we use for the V coordinate of the
		   oblique pole.  Uu_max is what we use for the U
		   coordinate of the oblique pole. In the following
		   three lines, read cs_AnglTest1 as the cosine of
		   the angular distance from the pole to the point
		   in question (i.e. a point very close,but not
		   exactly at, the pole). */

		tmp1 = (cs_One + cs_AnglTest1) / (cs_One - cs_AnglTest1);
		oblqm->vv_max = cs_Half * oblqm->ak * log (tmp1);

		oblqm->uu_max = oblqm->ak * (cs_Pi - cs_AnglTest);

		/* That's it for the spherical case. */
	}
	else
	{
		/* Now for the real stuff.  Much of the calcuations
		   are the same for either 1 or 2 point mode.  Of
		   course, there are some differences.  The differences
		   are, however, limited to the setup function.  Note,
		   that lat_c and lat_0 are the same thing.  This
		   was taken care of above where the projection
		   parameters were extracted from the coordinate
		   system definition.
		   
		   First, we compute some values which are used more
		   than once. */

		cos_lat_0 = cos (oblqm->lat_0);
		sin_lat_0 = sin (oblqm->lat_0);
		oblqm->e_ovr_2 = oblqm->ecent * cs_Half;
		tmp1 = cos_lat_0 * cos_lat_0;
		tmp1 = tmp1 * tmp1 * oblqm->e_sq;
		tmp2 = cs_One - oblqm->e_sq;
		tmp3 = sqrt (tmp2);
		tmp4 = oblqm->ecent * sin_lat_0;
		tmp5 = cs_One - (sin_lat_0 * sin_lat_0 * oblqm->e_sq);

		/* Now for some of the real projection constants.
		   A, B, t_0, and D are the same for both modes. */

		oblqm->B = sqrt (cs_One + (tmp1 / tmp2));
		oblqm->one_ovr_B = cs_One / oblqm->B;
		oblqm->A = oblqm->ak * oblqm->B * tmp3 / tmp5;
		oblqm->A_ovr_B = oblqm->A * oblqm->one_ovr_B;

		tmp6 = (cs_One - tmp4) / (cs_One + tmp4);
		tmp6 = pow (tmp6,oblqm->e_ovr_2);
		t_0 = tan (PI_OVR_4 - (oblqm->lat_0 * cs_Half)) / tmp6;

		tmp6 = sqrt (cs_One - tmp4 * tmp4);
		D = oblqm->B * tmp3 / (cos_lat_0 * tmp6);
		Dsq = D * D;
		if (Dsq < cs_One) Dsq = cs_One;

		/* Now for some stuff which is different between
		   the two modes. */

		if (oblqm->one_pnt)
		{
			tmp6 = sqrt (Dsq - 1);
			if (oblqm->lat_0 < 0.0) tmp6 = -tmp6;
			F = D + tmp6;

			oblqm->E = F * pow (t_0,oblqm->B);
			G = (F - (cs_One / F)) * cs_Half;

			/* Now for the azimuth.  In one variation, we are given
			   the "skew azimuth of the rectified grid" which is
			   essentially gamma0.  In this case, we need to compute
			   az (which is often called "alpha C").  In the normal
			   case (normal for CS-MAP anyway) we are given "alpha C",
			   which is the azimuth of the central geodesic at the
			   projection center, and we need to compute gamma0. */
			if (oblqm->azmIsSkew)
			{
				oblqm->az = asin (D * sin (oblqm->gamma_0));
				oblqm->sin_az = sin (oblqm->az);
				oblqm->cos_az = cos (oblqm->az);
			}
			else
			{
				oblqm->gamma_0 = asin (oblqm->sin_az / D);
			}
			tmp6 = G * tan (oblqm->gamma_0);
			oblqm->lng_0 = oblqm->lng_c - (asin (tmp6) / oblqm->B);

			/* Compute the value of u at the projection center. We do
			   this only for the  */

			if (oblqm->uv_ctr)
			{
				if (fabs (oblqm->cos_az) > cs_AnglTest)
				{
					tmp2 = sqrt (Dsq - cs_One);
					oblqm->uc = oblqm->A_ovr_B * atan (tmp2 / oblqm->cos_az);
					if (oblqm->lat_0 < 0.0) oblqm->uc = -oblqm->uc;
				}
				else
				{
					/* Primarily for Switzerland/Hungary */
					
					oblqm->uc = oblqm->A * (oblqm->lng_c - oblqm->lng_0);
				}
			}
		}
		else
		{
			/* Here for the two point case.  These are significantly
			   different from the above. */

			tmp4 = oblqm->ecent * sin (oblqm->lat_1);
			tmp5 = (cs_One - tmp4) / (cs_One + tmp4);
			tmp6 = pow (tmp5,oblqm->e_ovr_2);
			t_1 = tan (PI_OVR_4 - (oblqm->lat_1 * cs_Half)) / tmp6;
			tmp4 = oblqm->ecent * sin (oblqm->lat_2);
			tmp5 = (cs_One - tmp4) / (cs_One + tmp4);
			tmp6 = pow (tmp5,oblqm->e_ovr_2);
			t_2 = tan (PI_OVR_4 - (oblqm->lat_2 * cs_Half)) / tmp6;

			tmp4 = sqrt (Dsq - cs_One);
			if (oblqm->lat_0 < 0.0) tmp4 = -tmp4;
			oblqm->E = (D + tmp4) * pow (t_0,oblqm->B);
			H = pow (t_1,oblqm->B);
			L = pow (t_2,oblqm->B);
			F = oblqm->E / H;
			G = (F - (cs_One / F)) * cs_Half;
			tmp4 = oblqm->E * oblqm->E;
			tmp5 = L * H;
			J = (tmp4 - tmp5) / (tmp4 + tmp5);
			P = (L - H) / (L + H);
			tmp4 = (oblqm->lng_1 + oblqm->lng_2) * cs_Half;
			tmp5 = oblqm->lng_1 - oblqm->lng_2;
			tmp5 = CS_adj2pi (tmp5);
			tmp5 *= oblqm->B * cs_Half;
			tmp5 = J * tan (tmp5);
			oblqm->lng_0 = tmp4 - atan2 (tmp5,P) * oblqm->one_ovr_B;
			oblqm->lng_0 = CS_adj2pi (oblqm->lng_0);
			tmp4 = oblqm->lng_1 - oblqm->lng_0;
			tmp4 = CS_adj2pi (tmp4);
			tmp4 = sin (oblqm->B * tmp4) / G;
			oblqm->gamma_0 = atan (tmp4);
			oblqm->az = asin (D * sin (oblqm->gamma_0));
			oblqm->sin_az = sin (oblqm->az);
			oblqm->cos_az = cos (oblqm->az);
		}

		/* For the inverse, we need the following, which are
		   the same for all four variations. */

		oblqm->sin_gam_0 = sin (oblqm->gamma_0);
		oblqm->cos_gam_0 = cos (oblqm->gamma_0);

		/* Vv_max is what we use for the V coordinate of the
		   oblique pole.  Uu_max is what we use for the U
		   coordinate of the oblique pole.

		   In the following three lines, read cs_AnglTest1 as
		   the cosine of the angular distance from the pole
		   to the point in question (i.e. a point very close
		   to the pole). */

		tmp1 = (cs_One + cs_AnglTest1) / (cs_One - cs_AnglTest1);
		oblqm->vv_max = cs_Half * oblqm->A_ovr_B * log (tmp1);

		oblqm->uu_max = oblqm->A_ovr_B * (cs_Pi - cs_AnglTest);

		/* Calculate the isometric latitude series coefficients
		   for this ellipsoid. */

		CSchiIsu (&oblqm->chicofI,oblqm->e_sq);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian, which is where the central great circle crosses
	   the equator. */

	csprm->cent_mer = oblqm->lng_c * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate the useful range.  Since the
		   range is very orthogonal (i.e. a min/max) and the
		   projection is oblique, this is very subjective. */

		zone_wd = (cs_Two * acos (oblqm->k0) * cs_Radian) + 0.25;
		tmp1 = cs_Ten * fabs (oblqm->sin_az) + zone_wd * fabs (oblqm->cos_az);
		csprm->min_ll [LNG] = -tmp1;
		csprm->max_ll [LNG] =  tmp1;
		tmp1 = cs_Ten * fabs (oblqm->cos_az) + zone_wd * fabs (oblqm->sin_az);
		csprm->min_ll [LAT] = oblqm->lat_0 * cs_Radian - tmp1;
		csprm->max_ll [LAT] = oblqm->lat_0 * cs_Radian + tmp1;
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
	   calculate some appropriate values.  When calculating values,
	   we have to consider if this coordinate system is rectified
	   or not. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		/* No specification in the coordinate system definition.
		   If not rectified, we'll just use uu_max and vv_max.
		   If rectified, we'll use the rectified versions of
		   these values. */

		if (oblqm->rect_flg)
		{
			csprm->min_xy [XX] = -oblqm->vv_max * oblqm->cos_az +
						 		 -oblqm->uu_max * oblqm->sin_az;
			csprm->min_xy [YY] = -oblqm->uu_max * oblqm->cos_az -
								 -oblqm->vv_max * oblqm->sin_az;
			csprm->max_xy [XX] =  oblqm->vv_max * oblqm->cos_az +
						 		  oblqm->uu_max * oblqm->sin_az;
			csprm->max_xy [YY] =  oblqm->uu_max * oblqm->cos_az -
								  oblqm->vv_max * oblqm->sin_az;
		}
		else
		{
			csprm->min_xy [XX] = -oblqm->vv_max;
			csprm->min_xy [YY] = -oblqm->uu_max;
			csprm->max_xy [XX] =  oblqm->vv_max;
			csprm->max_xy [YY] =  oblqm->uu_max;
		}
		CS_quadMM (csprm->min_xy,csprm->max_xy,oblqm->x_off,
											   oblqm->y_off,
											   oblqm->quad);
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
	   addresses and we are done.  All four projections use
	   the same functions.  Since the projection is conformal,
	   the k and h scale factors are the same. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSoblqmF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSoblqmI;
	csprm->cs_scale = (cs_SCALE_CAST)CSoblqmK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSoblqmK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSoblqmK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSoblqmC;
	csprm->llchk    = (cs_LLCHK_CAST)CSoblqmL;
	csprm->xychk    = (cs_XYCHK_CAST)CSoblqmX;

	return;
}

/**********************************************************************
**	rtn_val = CSoblqmF (oblqm,xy,ll);
**
**	struct cs_Oblqm_ *oblqm;	structure which defines the parameters
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
**	This function calculates the Oblique Mercator
**	projection and returns values in the coordinate system
**	described in the cs_Oblqm_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 73-84.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSoblqmF (Const struct cs_Oblqm_ *oblqm,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi_o_2;			/* pi over two       */
	extern double cs_Pi;				/* 3.14159...        */
	extern double cs_Mpi;				/* 3.14159...        */
	extern double cs_Two_pi;			/* 2 Pi              */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole in radians. */

	int rtn_val;
	int rtn_valUV;

	double lng;			/* The given longitude, after conversion
						   to radians. */
	double lat;			/* The given latitude after conversion
						   to radians. */
	double del_lng;		/* delta longitude from the longitude of
						   the origin point. */

	double uu;		/* unrectified U coordinate */
	double vv;		/* unrectified V coordinate */

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.  There
	   is a miminal amount of stuff which is
	   common to both which we perform first.

	   Convert the latitude and longitude to radians. */

	lng = cs_Degree * ll [LNG];
	lat = cs_Degree * ll [LAT];

	if (fabs (lat) > cs_NPTest)
	{
		/* The actual true poles themselves have definite
		   coordinates.  However, longitude at the poles
		   is undefined mathematically. */

		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	del_lng = lng - oblqm->lng_0;
	if      (del_lng > cs_Pi  && oblqm->lng_0 < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && oblqm->lng_0 > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	/* uu and vv calculations are isolated in a separate function, as
	   CSoblqmK also needs access to them. Thus, substantial code
	   duplication is avoided. */

	rtn_valUV = CSoblqmFuv (oblqm,lng,lat,del_lng,&uu,&vv);
	if (rtn_valUV != 0) rtn_val = rtn_valUV;

	/* Apply the automatic centereing if active. Note, that this
	   applies only to the one point ellipsoidal forms. In all
	   other cases, oblqm->uc is set to 0.0 */

	uu -= oblqm->uc;

	/* Rectify, as appropriate. */

	switch (oblqm->rect_flg) {

	case csOBLQM_ALASKA:

		xy [XX] = vv * oblqm->cos_az + uu * oblqm->sin_az;
		xy [YY] = uu * oblqm->cos_az - vv * oblqm->sin_az;
		break;

	case csOBLQM_RECT:

		xy [XX] = vv * oblqm->cos_gam_0 + uu * oblqm->sin_gam_0;
		xy [YY] = uu * oblqm->cos_gam_0 - vv * oblqm->sin_gam_0;
		break;

	default:

		/* Not rectified. */

		xy [XX] = uu;
		xy [YY] = vv;
		break;
	}

	/* Apply non-standard quadrant. */

	if (oblqm->quad == 0)
	{
		xy [XX] += oblqm->x_off;
		xy [YY] += oblqm->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],oblqm->x_off,oblqm->y_off,oblqm->quad);
	}

	/* That's that. */

	return (rtn_val);
}

/*
	To prevent a large scale duplication of code, we have the following
	calculation of u and v in a separate function. This is necessary as
	the scale function (CSoblqmK) needs to be able to calculate u.

	Note, for this function, lat, lng, and del_lng arguments are in
	radians, and have already been checked and normalized.
*/

int EXP_LVL9 CSoblqmFuv (Const struct cs_Oblqm_ *oblqm,double lng,double lat,double del_lng,double* uu,double *vv)
{
	extern double cs_Pi_o_4;			/* pi over four      */
	extern double cs_Half;				/* 0.5               */
	extern double cs_One;				/* 1.0               */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole in radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */
	extern double cs_AnglTest1;			/* 1.0 - cs_AnglTest */

	int rtn_val;

	double sin_lat;
	double cos_lat;

	double sin_del;
	double cos_del;

	double t;
	double A;
	double Bdel;
	double Q;
	double S;
	double T;
	double U;
	double V;

	double tmp1;
	double tmp2;
	double tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.  There
	   is a miminal amount of stuff which is
	   common to both which we perform first.

	   Some other values common to both the spherical
	   and ellipsoidal. */

	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* Are we to perform the spherical calculations. */

	if (oblqm->ecent == 0.0)
	{
		/* Here for the sphere. */

		sin_del = sin (del_lng);
		cos_del = cos (del_lng);

		/* The uu value.  (tmp1 + tmp2) AND cos_del will BOTH
		   be zero at the oblique pole. */

		tmp1 = tan (lat) * oblqm->cos_lat_p;
		tmp2 = oblqm->sin_lat_p * sin_del;

		if (fabs (cos_del) < cs_AnglTest)
		{
			/* Adjust cos_del by just enough to keep things
			   from blowing up. The tmp3 result is going to
			   be pi/2 regardless. */

			rtn_val = cs_CNVRT_RNG;
			cos_del = cs_AnglTest;
			if ((lng - oblqm->lng_0) < 0.0)
			{
				cos_del = -cos_del;
			}
		}

		tmp3 = atan2 ((tmp1 + tmp2),cos_del);
		*uu = oblqm->ak * tmp3;

		/* Now for vv.  Problems if A == 1.0.  This
		   corresponds to 90 degrees of angular distance
		   from the central line, i.e. the oblique pole.
		   Since A is the sine of an angular distance,
		   cs_AnglTest1 (1.0 - cs_AnglTest) is an appropriate
		   test value. */

		A = oblqm->sin_lat_p * sin_lat -
		    oblqm->cos_lat_p * cos_lat * sin_del;

		if (fabs (A) > cs_AnglTest1)
		{
			rtn_val = cs_CNVRT_RNG;
			if (A > 0.0) *vv =  oblqm->vv_max;
			else	     *vv = -oblqm->vv_max;
		}
		else
		{
			tmp1 = (cs_One + A) / (cs_One - A);
			*vv = cs_Half * oblqm->ak * log (tmp1);
		}
	}
	else
	{
		/* Here for the ellisoid.  If the latitude is either
		   plus 90 or minus 90, we have a default condition. */

		if (fabs (lat) > cs_NPTest)
		{
			/* Here if latitude is very close to 90
			   degrees. */

			tmp1 = oblqm->gamma_0 * cs_Half;
			if (lat > 0.0) tmp1 = - tmp1;
			tmp2 = tan (cs_Pi_o_4 + tmp1);
			*vv = oblqm->A_ovr_B * log (tmp2);
			*uu = oblqm->A_ovr_B * lat;
		}
		else
		{
			/* Here for all other latitudes. */

			tmp1 = tan (cs_Pi_o_4 - (lat * cs_Half));
			tmp2 = oblqm->ecent * sin_lat;
			tmp3 = (cs_One - tmp2) / (cs_One + tmp2);
			tmp3 = pow (tmp3,oblqm->e_ovr_2);
			t = tmp1 / tmp3;

			/* B is never zero for a vaild ellipsoid.
			   E should never be zero either. */

			Q = oblqm->E / pow (t,oblqm->B);
			tmp1 = cs_One / Q;
			S = cs_Half * (Q - tmp1);
			T = cs_Half * (Q + tmp1);
			Bdel = oblqm->B * del_lng;
			V = sin (Bdel);

			U = (S * oblqm->sin_gam_0 - V * oblqm->cos_gam_0) / T;

			if (fabs (U) > cs_AnglTest1)
			{
				rtn_val = cs_CNVRT_RNG;
				if (U > 0.0) *vv =  oblqm->vv_max;
				else         *vv = -oblqm->vv_max;
			}
			else
			{
				tmp1 = (cs_One - U) / (cs_One + U);
				*vv = cs_Half * oblqm->A_ovr_B * log (tmp1);
			}
			tmp1 = S * oblqm->cos_gam_0 +
				   V * oblqm->sin_gam_0;
			tmp2 = cos (Bdel);
			if (fabs (tmp2) < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				*uu = oblqm->A * Bdel;
			}
			else
			{
				/* Due to the above, tmp2 is never zero. */

				*uu = oblqm->A_ovr_B * atan2 (tmp1,tmp2);
			}
		}
	}
	return rtn_val;
}

/**********************************************************************
**	rtn_val = CSoblqmI (oblqm,ll,xy);
**
**	struct cs_Oblqm_ *oblqm;	structure which defines the parameters
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
**	1984, pages 73-84.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSoblqmI (Const struct cs_Oblqm_ *oblqm,double ll [2],Const double xy [2])
{
	extern double cs_Pi;				/*  3.14159....   */
	extern double cs_Pi_o_2;			/* pi over two    */
	extern double cs_Radian;			/* 57.29577...    */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5            */
	extern double cs_One;				/* 1.0            */
	extern double cs_Mone;				/* -1.0           */
	extern double cs_Two;				/* 2.0            */
	extern double cs_AnglTest1;			/* 1.0 - cs_AnglTest */

	int rtn_val;

	double xx;
	double yy;
	double uu;
	double vv;

	double lat;
	double del_lng;

	double t;
	double chi;
	double Q;
	double S;
	double T;
	double V;
	double U;

	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;
	double tmp5;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae. */

	if (oblqm->quad == 0)
	{
		xx = xy [XX] - oblqm->x_off;
		yy = xy [YY] - oblqm->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,oblqm->x_off,oblqm->y_off,oblqm->quad);
	}

	/* First, if the coordinates are rectified, we must unrectify them. */

	switch (oblqm->rect_flg) {

	case csOBLQM_ALASKA:

		uu = yy * oblqm->cos_az + xx * oblqm->sin_az;
		vv = xx * oblqm->cos_az - yy * oblqm->sin_az;
		break;

	case csOBLQM_RECT:

		uu = yy * oblqm->cos_gam_0 + xx * oblqm->sin_gam_0;
		vv = xx * oblqm->cos_gam_0 - yy * oblqm->sin_gam_0;
		break;

	default:
	
		/* Unrectified system. */
	
		uu = xx;
		vv = yy;
		break;
	}

	/* Undo the affect of the automatic centering feature, if
	   applicable. oblqm->uc is zero in all cases where it is
	   not applicable. */

	uu += oblqm->uc;

	/* Verify that we are in suitable range.  If not, force a
	   reasonable condition. */

	if (fabs (uu) > oblqm->uu_max)
	{
		rtn_val = cs_CNVRT_RNG;
		if (uu > 0.0) uu =  oblqm->uu_max;
		else	      uu = -oblqm->uu_max;
	}
	if (fabs (vv) > oblqm->vv_max)
	{
		rtn_val = cs_CNVRT_RNG;
		if (vv > 0.0) vv =  oblqm->vv_max;
		else	      vv = -oblqm->vv_max;
	}

	/* See if we have the spherical case. */

	if (oblqm->ecent == 0.0)
	{
		/* Here for the sphere. */

		tmp1 = uu / oblqm->ak;
		tmp2 = vv / oblqm->ak;
		tmp3 = sin (tmp1);

		tmp4 = oblqm->sin_lat_p * tanh (tmp2);
		/* cosh is never zero. */
		tmp4 += oblqm->cos_lat_p * tmp3 / cosh (tmp2);
		if (fabs (tmp4) > cs_One)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp4 = (tmp4 > 0.0) ? cs_One : cs_Mone;
		}
		lat = asin (tmp4);

		tmp4 = (oblqm->sin_lat_p * tmp3) - (oblqm->cos_lat_p * sinh (tmp2));
		tmp5 = cos (tmp1);

		/* If uu is forced to be less than or equal uu_max; tmp1
		   should never be one, and tmp5 should never be zero.
		   Since atan2 will always have at least one non-zero
		   argument, it should be safe. */

		del_lng = atan2 (tmp4,tmp5);
	}
	else
	{
		/* Here for the ellisoid. */

		tmp1 = vv / oblqm->A_ovr_B;
		tmp2 = exp (tmp1);	/* exp is valid for all real numbers */
		Q = cs_One / tmp2;
		S = (Q - tmp2) * cs_Half;
		T = (Q + tmp2) * cs_Half;
		V = sin (uu / oblqm->A_ovr_B);
		U = (V * oblqm->cos_gam_0 + S * oblqm->sin_gam_0) / T;
		if (fabs (U) > cs_AnglTest1)
		{
			rtn_val = cs_CNVRT_INDF;
			del_lng = cs_Zero;
			lat = cs_Pi_o_2;
			if (U < 0.0) lat = -lat;
		}
		else
		{
			/* We handled U = -+1 above; and as a result,
			   tmp3 should never be zero. */

			tmp3 = (cs_One + U) / (cs_One - U);
			tmp3 = oblqm->E / sqrt (tmp3);
			t = pow (tmp3,oblqm->one_ovr_B);

			/* Calculate chi, the isometric latitude, and hence
			   the geographic latitude. */

			chi = cs_Pi_o_2 - cs_Two * atan (t);
			lat = CSchiIcal (&oblqm->chicofI,chi);

			tmp4 = (S * oblqm->cos_gam_0) - (V * oblqm->sin_gam_0);

			/* Magnitude of uu will never be greater than uu_max,
			   which means tmp5 will never be zero.  Close, but
			   never zero. */

			tmp5 = cos (uu / oblqm->A_ovr_B);
			del_lng = -atan2 (tmp4,tmp5) / oblqm->B;
		}
	}

	/* Convert the results to degrees. */

	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	ll [LNG] = (del_lng + oblqm->lng_0) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSoblqmC (oblqm,ll);
**
**	struct cs_Oblqm_ *oblqm;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Hotine Oblique Mercator
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

double EXP_LVL9 CSoblqmC (Const struct cs_Oblqm_ *oblqm,Const double ll [2])
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
	status = CSoblqmF (oblqm,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSoblqmF (oblqm,xy2,my_ll);
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
**	kk = CSoblqmK (oblqm,ll);
**
**	struct cs_Oblqm_ *oblqm;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale factor along a
**								parallel of the projected coordinate system
**								at the specified point.
**********************************************************************/

double EXP_LVL9 CSoblqmK (Const struct cs_Oblqm_ *oblqm,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_One;				/* 1.0               */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_SclInf;			/* 9.9E+04, the value
										   we return for infinite
										   scale. */
	double lng;
	double lat;
	double kk;

	double sin_lat;
	double cos_lat;
	double del_lng;
	double A;

	double tmp1;
	double tmp2;
	double tmp3;

	double uu;
	double vv;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	sin_lat = sin (lat);
	cos_lat = cos (lat);
	del_lng = CS_adj2pi (lng - oblqm->lng_0);

	/* Set the error condition until we know different. */

	kk = cs_SclInf;
	if (oblqm->ecent == 0.0)
	{
		/* Here for the sphere. Note, this blows up at the
		   oblique poles. */

		tmp1 = oblqm->cos_lat_p * cos_lat * sin (del_lng);
		A = oblqm->sin_lat_p * sin_lat - tmp1;
		tmp1 = cs_One - (A * A);

		/* Read tmp1 as the sine squared  of the angluar
		   distance of the point from the oblique pole. */

		if (tmp1 > cs_AnglTest)
		{
			kk = oblqm->k0 / sqrt (tmp1);
		}
	}
	else
	{
		/* Here for the ellipsoid.  We need the u coordinate
		   for this one.  We call CSoblqmF to compute it for
		   us. */

		if (CSoblqmFuv (oblqm,lng,lat,del_lng,&uu,&vv) == 0)
		{
			tmp1 = oblqm->A * cos (uu / oblqm->A_ovr_B);
			tmp2 = oblqm->ecent * sin_lat;
			tmp2 = cs_One - tmp2 * tmp2;
			tmp2 = sqrt (tmp2);
			tmp3 = cos (oblqm->B * del_lng);
			tmp3 = oblqm->ak * cos_lat * tmp3;
			if (tmp3 > cs_AnglTest)
			{
				kk = oblqm->k0 * tmp1 * tmp2 / tmp3;
			}
		}
	}
	return (kk);
}

/**********************************************************************
**	status = CSoblqmL (oblqm,cnt,pnts);
**
**	struct cs_Oblqm_ *oblqm;	the coordinate system against which the check is
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

/*
	The following function is static, not visible outside
	this source module, so the name may not adhere to the
	normal naming conventtion.

	The function verifies that the point supplied to it
	is indeed within the geographic domain of the coordinate
	system; in this case, not within a small distance of
	either pole.
*/

static int CSoblqmLP (Const struct cs_Oblqm_ *oblqm,Const double ll [2])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi;			/* PI */
	extern double cs_AnglTest;		/* 0.001 seconds of arc in
									   radians */

	double cc;						/* Angular distance between
									   two points on the sphere,
									   in radians. */

	double pole_ll [2];
	double test_ll [2];

	/* We'll compute the angular distance from the point to the
	   two poles.  If within .001 seconds of arc of a pole, we
	   report a problem.  We do this calculation on a sphere.
	   This should be OK for most every application. */

	pole_ll [LNG] = oblqm->lng_p;
	pole_ll [LAT] = oblqm->lat_p;

	test_ll [LNG] = ll [LNG] * cs_Degree;
	test_ll [LAT] = ll [LAT] * cs_Degree;

	cc = CSccsphrR (pole_ll,test_ll);
	if (cc < cs_AnglTest) return (cs_CNVRT_DOMN);

	/* The other pole. */

	pole_ll [LNG] = CS_adj2pi (pole_ll [LNG] + cs_Pi);
	pole_ll [LAT] = -pole_ll [LAT];
	cc = CSccsphrR (pole_ll,test_ll);
	if (cc < cs_AnglTest) return (cs_CNVRT_DOMN);

	return (cs_CNVRT_OK);
}

int EXP_LVL9 CSoblqmL (Const struct cs_Oblqm_ *oblqm,int cnt,Const double pnts [][3])

{
	extern double cs_Degree;
	extern double cs_EETest;			/* .001 seconds of arc less
										   than PI/2 in radians. */
	extern double cs_WETest;			/* -cs_EETest */

	int ii;
	int status;

	double tmp;
	double del_lng;

	/* Check all the points. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
	{
		status = CSoblqmLP (oblqm,pnts [ii]);
	}
	if (cnt <= 1 || status != cs_CNVRT_OK) return (status);

	/* If cnt is 2, we have a line which we must check. */

	if (cnt == 2)
	{
		for (ii = 0;ii < cnt;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - oblqm->lng_0);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
				break;
			}
		}
	}
	else if (cnt == 3)
	{
		/* Can't handle a three point list, the region must
		   be closed. */

		CS_erpt (cs_RGN_PNTCNT);
		return (cs_CNVRT_ERR);
	}
	else
	{
		/* WHAT WE SHOULD BE DOING:
		   The great circle between the two singularity points
		   is a 180 degree segment of a great circle.  We should:
		   1) use the technique described for the line case
		      to see if any segment of the region actually
		      goes through either of the singularity points.
		   2) count the intersections of all segments in the
		      boundary with the great circle segment between the
		      two singularity points.  If the count is even
		      (or zero) we are OK.  Otherwise, we have a problem.
		      That is, the region includes a singularity point.

		   WHAT WE DO NOW:
		   We simply see if any of the points has a longitude
		   which puts it outside of the range of central
		   longitude +- pi/2. */

		for (ii = 0;ii < cnt;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - oblqm->lng_0);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
				break;
			}
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSoblqmX (oblqm,cnt,pnts);
**
**	struct cs_Oblqm_ *oblqm;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSoblqmL to check lat/long
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

int EXP_LVL9 CSoblqmX (Const struct cs_Oblqm_ *oblqm,int cnt,Const double pnts [][3])
{
	int rtn_val;

	rtn_val = cs_CNVRT_OK;

	return (rtn_val);
}							/*lint !e715 */
