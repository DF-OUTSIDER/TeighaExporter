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

/*lint -esym(715,prj_code,winkl) */		/* parameters not referenced */

#include "cs_map.h"

/*lint -esym(613,err_list)  possible use of null pointer; but not really */

/* Std Lat == 50.467 for Winkel, 40.0 for Bartholomew. */

int EXP_LVL9 CSwinklQ (Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Mercator specific stuff. */
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* We divide by cos(prj_prm1).  Therefore, either pole is invalid.
	   We can let it get pretty close though.  88 is a rather arbitary
	   limit. */
	if (cs_def->prj_prm1 <= -88.0 || cs_def->prj_prm1 >= 88.0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDLAT;
	}

	/* That's it for Winkel-Tripel. */
	return (err_cnt + 1);
}
void EXP_LVL9 CSwinklS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/*   57.2957...  */
	extern double cs_Degree;			/*    1.0 / 57.2957..  */
	extern double cs_Pi;				/*    3.14.159... */
	extern double cs_Mpi;				/*   -3.14.159... */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole, in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the south
										   pole, in radians. */
	struct cs_Winkl_ *winkl;
	double tmp;

	winkl = &csprm->proj_prms.winkl;

	/* Transfer the necessary arguments to the "winkl" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */
	winkl->org_lng = csprm->csdef.org_lng * cs_Degree;
	winkl->ref_lat = csprm->csdef.prj_prm1 * cs_Degree;
	winkl->k = csprm->csdef.scale;
	winkl->e_rad = csprm->datum.e_rad;
	winkl->ka = csprm->datum.e_rad * winkl->k;
	winkl->x_off = csprm->csdef.x_off;
	winkl->y_off = csprm->csdef.y_off;
	winkl->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* There isn't much to do for this one. */
	winkl->twoKa = winkl->ka + winkl->ka;
	winkl->cos_ref_lat = cos (winkl->ref_lat);
	winkl->Rcos_ref_lat = winkl->ka * winkl->cos_ref_lat;
	winkl->yy_max =  winkl->ka * cs_Pi_o_2;
   	winkl->one_mm = 0.001 * winkl->k;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */
	csprm->cent_mer = winkl->org_lng * cs_Radian;
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
		/* No specification in the coordinate system definition. */
		csprm->min_xy [XX] = cs_Mpi * winkl->Rcos_ref_lat;
		csprm->max_xy [XX] = cs_Pi  * winkl->Rcos_ref_lat;

		tmp = cs_SPTest;
		csprm->min_xy [YY] = tmp * winkl->ka;
		tmp = cs_NPTest;
		csprm->max_xy [YY] = tmp * winkl->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,winkl->x_off,
											   winkl->y_off,
											   winkl->quad);
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
	csprm->ll2cs    = (cs_LL2CS_CAST)CSwinklF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSwinklI;
	csprm->cs_scale = (cs_SCALE_CAST)CSwinklK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSwinklK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSwinklH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSwinklC;
	csprm->llchk    = (cs_LLCHK_CAST)CSwinklL;
	csprm->xychk    = (cs_XYCHK_CAST)CSwinklX;

	return;
}
int EXP_LVL9 CSwinklB (Const struct cs_Winkl_ *winkl,double* xx,
													 double* yy,
													 double del_lng,
													 double lat)
{
	extern double cs_Zero;
	extern double cs_Half;
	extern double cs_One;
	extern double cs_Mone;
	extern double cs_AnglTest;

	int rtn_val;

	double sinLat;
	double cosLat;

	double xAitoff;
	double yAitoff;
	double xRectangular;
	double yRectangular;
	double dd, signedD, cc;

	rtn_val = cs_CNVRT_NRML;

	// Compute the Equirectangular values.
	xRectangular = winkl->ka * del_lng * winkl->cos_ref_lat;
	yRectangular = winkl->ka * lat;

	// Compute the Aitoff values.,
	sinLat = sin (lat);
	cosLat = cos (lat);
	// dd (i.e. D) will always be positive if fabs (deltaLng) <= 180.
	dd = acos (cos (del_lng * cs_Half) * cosLat);
	if (dd <= cs_AnglTest)
	{
		xAitoff = yAitoff = cs_Zero;
	}
	else
	{
		signedD = (del_lng >= 0.0) ? dd : -dd;
		cc = sinLat / sin (dd);
		/* When del_lng is zero, computation noise  often produces a value of cc
		   which is greater than 1.0 (or less than -1.0) which will then produce
		   a floating point exception. */
		if (fabs (cc) > cs_One)
		{
			cc = (cc < 0.0) ? cs_Mone : cs_One;
		}
		xAitoff = winkl->twoKa * signedD * sqrt (cs_One - (cc * cc));
		yAitoff = winkl->ka * dd * cc;
	}

	// Average the two independent results.
	*xx = cs_Half * (xAitoff + xRectangular);
	*yy = cs_Half * (yAitoff + yRectangular);
	return rtn_val;
}
int EXP_LVL9 CSwinklF (Const struct cs_Winkl_ *winkl,double xy [2],Const double ll [2])
{
	extern double cs_Degree;
	extern double cs_Pi;
	extern double cs_Pi_o_2;
	extern double cs_3Pi_o_2;

	int rtn_val;

	double del_lng;
	double lng;
	double lat;

	double xx;
	double yy;

	rtn_val = cs_CNVRT_NRML;

	/* For this projection, we only support the spherical form,
	   therefore there is only one set of equations.  These are
	   pretty simple. */
	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}

	lng = ll [LNG] * cs_Degree;
	del_lng = lng - winkl->org_lng;
	
	/* For this projection, we assure that the central meridian is the center of
	   the  map; this implying that delta longitude is always less than or equal
	   to pi, and greater than minus pi. */
	if (fabs (del_lng) > cs_3Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
	}
	if (fabs (del_lng) > cs_Pi)
	{
		del_lng = CS_adj2pi (del_lng);
	}
	rtn_val = CSwinklB (winkl,&xx,&yy,del_lng,lat);
	
	if (winkl->quad == 0)
	{
		xy [XX] = xx + winkl->x_off;
		xy [YY] = yy + winkl->y_off;
	}
	else
	{
		CS_quadF (xy,xx,yy,winkl->x_off,winkl->y_off,winkl->quad);
	}
	return rtn_val;	
}
int EXP_LVL9 CSwinklI (Const struct cs_Winkl_ *winkl,double ll [2],Const double xy [2])
{
	extern double cs_One;
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_AnglTest;
	extern double cs_Mpi;
	extern double cs_Pi;

	static double cnvrgK = 0.90;

	int rtn_val;
	int itr_cnt;

	double xx;
	double yy;
	double dd;
	double lat;
	double lng;
	double del_lng;
	double newX;
	double newY;
	double deltaX;
	double deltaY;

	rtn_val = cs_CNVRT_NRML;

	if (winkl->quad == 0)
	{
		xx = xy [XX] - winkl->x_off;
		yy = xy [YY] - winkl->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,winkl->x_off,winkl->y_off,winkl->quad);
	}

	// Compute a first guess for the latitude and longitude using the
	// Equirectangular Inverse (which is easy enough) and an approximation of
	// the difference of the two.
	lat     = (yy / winkl->ka);
	del_lng  = xx / ( winkl->Rcos_ref_lat);			// equidistant
	del_lng *= (cs_One + 0.38 * cos (lat));			// Aitoff adjustment (approx)
	if (del_lng < cs_Mpi) del_lng = cs_Mpi + cs_AnglTest;
	else if (del_lng > cs_Pi) del_lng = cs_Pi - cs_AnglTest;

	itr_cnt = 0;
	do
	{
		rtn_val = CSwinklB (winkl,&newX,&newY,del_lng,lat);
		deltaX = newX - xx;
		deltaY = newY - yy;
		dd = sqrt (deltaX * deltaX + deltaY * deltaY);
		lat -= (deltaY / winkl->ka) * cnvrgK;
		del_lng -= (deltaX / winkl->Rcos_ref_lat) * cnvrgK;
		itr_cnt += 1;
	} while (itr_cnt < 40 && dd > winkl->one_mm);
	if (itr_cnt >= 40)
	{
		rtn_val = cs_CNVRT_ERR;
	}
	lng = (winkl->org_lng + del_lng);
	if (fabs (lng) >= cs_Pi)
	{
		lng = CS_adj2pi (lng);
	}

	ll [LNG] = lng * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return rtn_val;
}
double EXP_LVL9 CSwinklC (Const struct cs_Winkl_ *winkl,Const double ll [2])
{
	extern double cs_Radian;
	extern double cs_Km360;

	int status;

	double del_xx;
	double del_yy;
	double gamma;

	double xy1 [3];
	double xy2 [3];
	double my_ll [3];

	my_ll [LNG] = ll [LNG];
	my_ll [LAT] = ll [LAT];

	my_ll [LAT] -= 0.00005;
	status = CSwinklF (winkl,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSwinklF (winkl,xy2,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	
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
double EXP_LVL9 CSwinklK (Const struct cs_Winkl_ *winkl,Const double ll [2])
{
	extern double cs_Degree;
	extern double cs_Mone;
	extern double cs_HlfSecDeg;
	extern double cs_SclInf;
	extern double cs_Sin1Sec;

	int status;

	double kk;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	ll0 [LNG] = ll [LNG] - cs_HlfSecDeg;
	ll0 [LAT] = ll [LAT];
	ll1 [LNG] = ll [LNG] + cs_HlfSecDeg;
	ll1 [LAT] = ll [LAT];

	status = CSwinklF (winkl,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSwinklF (winkl,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	dd_ll = winkl->ka * cs_Sin1Sec * cos (ll0 [1] * cs_Degree);

	if (dd_ll > 0.0) kk = dd_xy / dd_ll;
	else		 kk = cs_SclInf;
	return (kk);
}
double EXP_LVL9 CSwinklH (Const struct cs_Winkl_ *winkl,Const double ll [2])
{
	extern double cs_Mone;
	extern double cs_SclInf;

	int status;

	double hh;

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	ll0 [LNG] = ll [LNG];
	ll0 [LAT] = ll [LAT] - 0.00005;
	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] + 0.00005;

	status = CSwinklF (winkl,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSwinklF (winkl,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	dd_ll = winkl->ka * 1.74532925E-06;
	if (dd_ll > winkl->one_mm)
	{
		hh = dd_xy / dd_ll;
	}
	else
	{
		hh = cs_SclInf;
	}
	return (hh);
}
int EXP_LVL9 CSwinklL (Const struct cs_Winkl_ *winkl,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;
	extern double cs_Pi_o_2;

	int ii;
	int status;

	double lat;

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}
int EXP_LVL9 CSwinklX (Const struct cs_Winkl_ *winkl,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSwinklI (winkl,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
