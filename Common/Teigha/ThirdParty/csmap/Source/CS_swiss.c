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

int EXP_LVL9 CSswissQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_SclRedMin;		/*    0.75 */
	extern double cs_SclRedMax;		/*    1.1 */

	int err_cnt;
	
	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Swiss National or Generalized Oblique Cylindrical specific stuff. */

	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	if (prj_code == cs_PRJCOD_OBQCYL)
	{
		/* Check for the two additional parameters here: Scale reduction and
		   Normal Parallel. */

		if (cs_def->prj_prm1 < cs_MinLat || cs_def->prj_prm1 > cs_MaxLat)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
		}
		if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
		}
	}

	/* That's it for the Swiss National/Oblique Cylindrical projection. */

	return (err_cnt + 1);
}

/**********************************************************************
**	CSswissS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSswissS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/*  1.0 / RADIAN  */
	extern double cs_Radian;			/*  57.27....  */
	extern double cs_Pi_o_4;			/*  Pi over 4 */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_Half;				/*  0.5 */
	extern double cs_One;				/*  1.0 */
	extern double cs_K15;				/*  45.0 */
	extern double cs_K45;				/*  45.0 */
	extern double cs_Km45;				/* -45.0 */
	extern double cs_MinLatFz;			/* -89.999... */
	extern double cs_MaxLatFz;			/* +89.99... */
	extern double cs_AnglTest;			/* 0.001 seconds of arc in
										   radians. */

	struct cs_Swiss_ *swiss;

	double one_esq;
	double sin_B0, cos_B0;
	double sin_Bn, cos_Bn, cos_Bn4;
	double tmp1, tmp2, tmp3, tmp4;

	double test_ll [3];
	double test_xy [3];


	swiss = &csprm->proj_prms.swiss;

	/* Transfer the necessary arguments to the "swiss" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */


	swiss->L0 = csprm->csdef.org_lng * cs_Degree;
	swiss->B0 = csprm->csdef.org_lat * cs_Degree;
	swiss->k = csprm->csdef.scale;
	swiss->x_off = csprm->csdef.x_off;
	swiss->y_off = csprm->csdef.y_off;
	swiss->ecent = csprm->datum.ecent;
	swiss->e_sq = swiss->ecent * swiss->ecent;
	swiss->e_ovr_2 = swiss->ecent * cs_Half;

	/* Handle the differences between the two variations we support. */
	switch (csprm->prj_code) {
	case cs_PRJCOD_OBQCYL:
		swiss->m0 = csprm->csdef.scl_red;
		swiss->Bn = csprm->csdef.prj_prm1 * cs_Degree;
		break;
	default:
	case cs_PRJCOD_SWISS:
		swiss->m0 = cs_One;
		swiss->Bn = swiss->B0;
		break;
	}

	/* Finish up with standard stuff. */

	swiss->kR = csprm->datum.e_rad * swiss->k;
	swiss->resolve = cs_AnglTest;
	swiss->one_mm = 0.001 * swiss->k;
	swiss->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Set up the calculations. */

	if (swiss->ecent == 0.0)
	{
		/* Here if given a sphere. */

		swiss->R = swiss->kR * swiss->m0;
		swiss->M0 = swiss->kR;
		swiss->N0 = swiss->kR;
		swiss->bb0 = swiss->B0;
		swiss->sin_bb0 = sin (swiss->bb0);
		swiss->cos_bb0 = cos (swiss->bb0);
		swiss->alpha = cs_One;
		swiss->alpha_e2 = cs_One;
		swiss->K = cs_Zero;
	}
	else
	{
		sin_Bn = sin (swiss->Bn);
		one_esq = cs_One - swiss->e_sq;

		tmp1 = swiss->ecent * sin_Bn;
		tmp2 = cs_One - tmp1 * tmp1;
		tmp3 = sqrt (tmp2);
		swiss->N0 = swiss->kR / tmp3;
		swiss->M0 = swiss->kR * one_esq / (tmp2 * tmp3);
		swiss->R = sqrt (swiss->M0 * swiss->N0) * swiss->m0;

		cos_Bn = cos (swiss->Bn);
		cos_Bn4 = cos_Bn * cos_Bn * cos_Bn * cos_Bn;
		swiss->alpha = sqrt (cs_One + (swiss->e_sq / one_esq) * cos_Bn4);

		swiss->alpha_e2 = swiss->alpha * swiss->ecent * cs_Half;

		sin_B0 = sin (swiss->B0);
		cos_B0 = cos (swiss->B0);
		swiss->sin_bb0 = sin_B0 / swiss->alpha;
		swiss->cos_bb0 = sqrt (cs_One - swiss->sin_bb0 * swiss->sin_bb0);
		swiss->bb0 = asin (swiss->sin_bb0);

		tmp1 = swiss->ecent * sin_B0;
		tmp2 = tan (cs_Pi_o_4 + swiss->bb0 * cs_Half);
		tmp3 = tan (cs_Pi_o_4 + swiss->B0  * cs_Half);
		tmp4 = (cs_One + tmp1) / (cs_One - tmp1);
		swiss->K = log (tmp2) - swiss->alpha * log (tmp3) +
							   swiss->alpha_e2 * log (tmp4);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = swiss->L0 * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		csprm->min_ll [LNG] = cs_Km45;
		csprm->max_ll [LNG] = cs_K45;
		csprm->min_ll [LAT] = (swiss->B0 * cs_Radian) - cs_K15;
		if (csprm->min_ll [LAT] < cs_MinLatFz) csprm->min_ll [LAT] = cs_MinLatFz;
		csprm->max_ll [LAT] = (swiss->B0 * cs_Radian) + cs_K15;
		if (csprm->max_ll [LAT] > cs_MaxLatFz) csprm->max_ll [LAT] = cs_MaxLatFz;
	}
	else
	{
		csprm->min_ll [LNG] = CS_adj180 (csprm->csdef.ll_min [LNG] - csprm->cent_mer);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj180 (csprm->csdef.ll_max [LNG] - csprm->cent_mer);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the X's and Y's. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
		csprm->csdef.xy_max [XX] == 0.0)
	{
		test_ll [LNG] = csprm->min_ll [LNG];
		test_ll [LAT] = csprm->min_ll [LAT];
		CSswissF (swiss,test_xy,test_ll);			/*lint !e534 */
		csprm->min_xy [XX] = test_xy [XX];
		csprm->min_xy [YY] = test_xy [YY];

		test_ll [LNG] = csprm->max_ll [LNG];
		test_ll [LAT] = csprm->max_ll [LAT];
		CSswissF (swiss,test_xy,test_ll);			/*lint !e534 */
		csprm->max_xy [XX] = test_xy [XX];
		csprm->max_xy [YY] = test_xy [YY];
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSswissF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSswissI;
	csprm->cs_scale = (cs_SCALE_CAST)CSswissK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSswissK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSswissK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSswissC;
	csprm->llchk    = (cs_LLCHK_CAST)CSswissL;
	csprm->xychk    = (cs_XYCHK_CAST)CSswissX;

	return;
}																/*lint !e550 */

int EXP_LVL9 CSswissF (Const struct cs_Swiss_ *swiss,double xy [2],Const double lnglat [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Two_pi;			/* 6.28.... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Pi_o_4;			/* Pi over 4 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the south pole,
										   in radians. */

	int rtn_val;

	double L;					/* The given longitude, after conversion
								   to radians. */
	double B;					/* The given latitude after conversion
								   to radians. */
	double ll;					/* Longitude of given point on the
								   conformal sphere, relative to the
								   central point. */
	double bb;					/* Latitude of the given point on the
								   conformal sphere. */
	double ll_;					/* Longitude of given point on the
								   oblique conformal sphere. */
	double bb_;					/* Latitude of the given point on the
								   oblique conformal sphere. */
	double Q;

	double sin_B;
	double del_lng;
	double sin_ll, cos_ll;
	double sin_bb, cos_bb;
	double sin_ll_, cos_ll_;
	double sin_bb_, cos_bb_;


	double tmp1, tmp2;

	rtn_val = cs_CNVRT_NRML;

	L = lnglat [LNG] * cs_Degree;
	B = lnglat [LAT] * cs_Degree;

	del_lng = L - swiss->L0;
	if      (del_lng > cs_Pi  && swiss->L0 < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && swiss->L0 > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		/* The abnormal del_lng will be adjusted automatically
		   by the trigonometry below. */
	}

	/* Need to assure that B is a valid latitude. If not, the
	   cs_Pi_o_4 + 1/2 B be low produces a bogus value. Also,
	   cannot allow an exact value of either pole. Such a value
	   will require taking the log of zero or infinity below.
	   That ain't good either. */

	if (fabs (B) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_RNG;
		B = CS_adj1pi (B);
		if      (B > cs_NPTest) B = cs_NPTest;
		else if (B < cs_SPTest) B = cs_SPTest;
	}

	/* If we have a spherical reference, no need to map to a
	   conformal sphere. */

	if (swiss->ecent == 0.0)
	{
		ll = del_lng;
		bb = B;
	}
	else
	{
		/* Map the given lat/long to a conformal sphere. Note,
		   radius of this sphere is the geometric mean of
		   M0 and N0. */

		ll = swiss->alpha * del_lng;

		sin_B = sin (B);
		tmp1 = tan (cs_Pi_o_4 + B * cs_Half);
		tmp2 = swiss->ecent * sin_B;
		tmp2 = (cs_One + tmp2) / (cs_One - tmp2);
		Q = swiss->alpha * log (tmp1) - swiss->alpha_e2 * log (tmp2) + swiss->K;
		bb = cs_Two * atan (exp (Q)) - cs_Pi_o_2;
	}

	/* From here out, its the same for ellipsoid or sphere.
	   Convert the lng/lat of the conformal sphere to the
	   oblique sphere. */

	sin_ll = sin (ll);
	cos_ll = cos (ll);
	sin_bb = sin (bb);
	cos_bb = cos (bb);

	sin_bb_ = swiss->cos_bb0 * sin_bb - swiss->sin_bb0 * cos_bb * cos_ll;
	cos_bb_ = sqrt (cs_One - sin_bb_ * sin_bb_);
	bb_ = asin (sin_bb_);

	sin_ll_ = cos_bb * sin_ll / cos_bb_;
	cos_ll_ = (swiss->sin_bb0 * sin_bb + swiss->cos_bb0 * cos_bb * cos_ll) / cos_bb_;
	ll_ = asin (sin_ll_);
	if (cos_ll_ < 0.0)
	{
		if (sin_ll_ > 0.0) ll_ += cs_Pi_o_2;
		else			   ll_ -= cs_Pi_o_2;
	}

	/* Now, convert the oblique lng/lat to a cylinder ala Mercator.
	   This is the place we need to be careful. If bb_ == +- pi/2,
	   we have an indeterminate value. */

	xy [XX] = swiss->R * ll_;

	if (fabs (bb_) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_RNG;
		if (bb_ > cs_NPTest)
		{
			bb_ = cs_NPTest;
		}
		else if (bb_ < cs_SPTest)
		{
			bb_ = cs_SPTest;
		}
	}
	tmp1 = tan (cs_Pi_o_4 + bb_ * cs_Half);
	xy [YY] = swiss->R * log (tmp1);

	/* Apply false origin and quadrants. */

	if (swiss->quad == 0)
	{
		xy [XX] += swiss->x_off;
		xy [YY] += swiss->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],swiss->x_off,swiss->y_off,swiss->quad);
	}
	return (rtn_val);
}

int EXP_LVL9 CSswissI (Const struct cs_Swiss_ *swiss,double lnglat [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Pi;				/*  Pi */
	extern double cs_Mpi;				/*  -Pi */
	extern double cs_Pi_o_2;			/*  Pi over 2 */
	extern double cs_Pi_o_4;			/*  Pi over 4 */
	extern double cs_Half;				/*  0.5 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Two;				/*  2.0 */
	extern double cs_Ten;				/*  10.0 */

	int rtn_val;
	int nn;

	double xx;
	double yy;
	double ll_, bb_;			/* Lng/lat on the oblique sphere. */
	double ll, bb;				/* Lng/lat on the conformal sphere */
	double L, B;				/* Lng/Lat on the ellipsoid. */

	double sin_ll_, cos_ll_;
	double sin_bb_, cos_bb_;
	double sin_ll, cos_ll;
	double sin_bb, cos_bb;

	double Q;
	double yy_;
	double lastB;

	double tmp1, tmp2;

	rtn_val = cs_CNVRT_NRML;

	if (swiss->quad == 0)
	{
		xx = xy [XX] - swiss->x_off;
		yy = xy [YY] - swiss->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,swiss->x_off,swiss->y_off,swiss->quad);
	}

	/* Convert from the cylinder to the oblique sphere. There are some
	   singularities which we need to be careful of. */

	ll_ = xx / swiss->R;
	if (fabs (ll_) >= cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		if (ll_ > 0.0) ll_ = cs_Pi;
		else	       ll_ = cs_Mpi;
	}

	/* If yy_ is very large, it approximates a pole on the
	   cylinder, which is an indeterminate position. Since we will
	   be taking the atan of the exp, any large number will do.
	   Since this is really outside of the range of the projection,
	   accuracy isn't that important. The value of 10 here is
	   arbitrarily choosen. */

	yy_ = yy / swiss->R;
	if (fabs (yy_) > cs_Ten)
	{
		rtn_val = cs_CNVRT_RNG;
		if (yy_ > 0.0) yy_ = cs_Ten;
		else	       yy_ = -cs_Ten;
	} 
	yy_ = exp (yy_);
	bb_ = cs_Two * atan (yy_) - cs_Pi_o_2;

	sin_ll_ = sin (ll_);
	cos_ll_ = cos (ll_);
	sin_bb_ = sin (bb_);
	cos_bb_ = cos (bb_);

	sin_bb = swiss->cos_bb0 * sin_bb_ + swiss->sin_bb0 * cos_bb_ * cos_ll_;
	cos_bb = sqrt (cs_One - sin_bb * sin_bb);
	bb = asin (sin_bb);
	
	sin_ll = cos_bb_ * sin_ll_ / cos_bb;
	cos_ll = (swiss->cos_bb0 * cos_bb_ * cos_ll_ - swiss->sin_bb0 * sin_bb_) / cos_bb;
	ll = asin (sin_ll);
	if (cos_ll < 0.0)
	{
		if (sin_ll > 0.0) ll += cs_Pi_o_2;
		else		  ll -= cs_Pi_o_2;
	}

	/* Convert from the conformal sphere to the ellipsoid. If the
	   reference is a sphere, don't really need to do anything. */

	if (swiss->ecent == 0.0)
	{
		L = ll;
		B = bb;
	}
	else
	{
		L = ll / swiss->alpha;

		tmp1 = tan (cs_Pi_o_4 + bb * cs_Half);
		tmp1 = (log (tmp1) - swiss->K) / swiss->alpha;
		B = swiss->B0;
		nn = 20;
		do
		{
			lastB = B;
			tmp2 = swiss->ecent * sin (lastB);
			tmp2 = (cs_One + tmp2) / (cs_One - tmp2);
			Q = tmp1 + swiss->e_ovr_2 * log (tmp2);
			B = cs_Two * atan (exp (Q)) - cs_Pi_o_2;
		} while (--nn >= 0 && fabs (B - lastB) > swiss->resolve);
	}

	/* Convert the results to degrees. */

	lnglat [LNG] = (L + swiss->L0) * cs_Radian;
	lnglat [LAT] = B * cs_Radian;
	return (rtn_val);
}

double EXP_LVL9 CSswissK (Const struct cs_Swiss_ *swiss,Const double ll [2])

{
	extern double cs_SclInf;		/* 9.9E+04, the value we return
									   for an infinite scale
									   factor. */
	extern double cs_HlfSecDeg;		/* One half arc second in
									   degrees. */

	double kk;
	double ll_dd;
	double xy_dd;
	double del_xx, del_yy;

	double xy1 [2];
	double xy2 [2];
	double ll1 [2];
	double ll2 [2];

	/* Establish two points along the parallel which are
	   about 1 second (about 30 meters) apart from each
	   other, with the point in question in the middle.
	   Then convert each point to the equivalent grid
	   coordinates. */

	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] - cs_HlfSecDeg;
	CSswissF (swiss,xy1,ll1);					/*lint !e534 */

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + cs_HlfSecDeg;
	CSswissF (swiss,xy2,ll2);					/*lint !e534 */

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	CS_llazdd (swiss->kR,swiss->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point.  I don't think
	   ll_dd can be zero since we know that ll1 id not the
	   same as ll0.  However, just to be safe: */

	if (fabs (ll_dd) > swiss->one_mm)
	{
		kk = xy_dd / ll_dd;
	}
	else
	{
		kk = cs_SclInf;
	}
	return (kk);
}

double EXP_LVL9 CSswissC (Const struct cs_Swiss_ *swiss,Const double ll [2])
{
	extern double cs_Radian;		/* 57.2957... */
	extern double cs_Km360;			/* -360.0, the value which
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
	status = CSswissF (swiss,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSswissF (swiss,xy2,my_ll);
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

int EXP_LVL9 CSswissL (Const struct cs_Swiss_ *swiss,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double test_xy [3];

	/* NOTE: WE DO NOT, as yet, CHECK LINES AND REGIONS. In future,
	   we need to add some spherical trigonometry to see if any
	   great circle arc, or region includes either of the oblique
	   poles. */

	/* There are two singularity points; either pole on the
	   oblique sphere. In order to test this, we have to do
	   almost as much work as converting the point. So we
	   convert each point.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSswissF (swiss,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

int EXP_LVL9 CSswissX (Const struct cs_Swiss_ *swiss,int cnt,Const double pnts [][3])
{
	int rtn_val;

	rtn_val = cs_CNVRT_OK;

	/* Assuming that atan will return a rational result, i.e.
	   pi over 2, for even the most astronomical number, there
	   are no singularity points for X and Y.
	   
	   We leave the code necessary to handle the list, COMMENTED
	   OUT, just in case we learn different in the future. */

/******************
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSswissI (swiss,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
*******************/
	return (rtn_val);
}										/*lint !e715 */
