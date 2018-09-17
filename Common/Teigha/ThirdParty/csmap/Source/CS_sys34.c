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

/*						R E M A R K S

	This "projection" is quite unusual in that it models the Danish
	System 34 technique.  This technique converts directly from UTM
	to Danish cartesian coordinates, rather than from geographic
	coordinates.

	Therefore, to implement this in the framework of CS-MAP, we
	first perform a geographic to UTM conversion, and then use
	the System 34 polynomial algorithm to convert the resulting
	UTM's to the desired Danish cartesians.

	The inverse converts from the Danish to UTM's using the
	polynomial algorithms.  We then convert the UTM's to
	geographic in the standard manner.

	The cs_Sys34_ structure closely resembles a cs_Trmer_ structure,
	somewhat tailored for UTM calculations.
*/
/*lint -e835      A zero has been given as argument to operator - */

#include "cs_map.h"
#include "CSsys34KMS.c"

static Const double cs_KmsTolerance = 0.040;

static int EXP_LVL9 TransformS34J_UTM32 (double x1, double y1,double* x2, double* y2,short vintage); 
static int EXP_LVL9 TransformS34S_UTM32 (double x1, double y1,double* x2, double* y2,short vintage);
static int EXP_LVL9 TransformS45B_UTM33 (double x1,double y1,double* x2,double* y2,short vintage);
static int EXP_LVL9 TransformUTM32_S34J (double x1, double y1,double* x2,double* y2,short vintage);
static int EXP_LVL9 TransformUTM32_S34S (double x1,double y1,double *x2, double *y2,short vintage);
static int EXP_LVL9 TransformUTM33_S45B (double x1,double y1,double *x2,double *y2,short vintage);
static int kmsPolynomial (double Xin,double Yin,double *xout,double *yout,Const double coefPtr []);

/*lint -esym(578,y1) */
/*lint -esym(613,err_list) */
/*lint -esym(715,sys34,cnt,pnts) */
/*lint -esym(534,CSsys34F,CS_llazdd) */

/**********************************************************************
**	err_cnt = CSsys34Q (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	specifies the specific variation of the projection.
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

int EXP_LVL9 CSsys34Q (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_One;			/*    1.0 */

	short ok;
	int err_cnt;
	short region;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* There is only one parameter. */
	region = (short)cs_def->prj_prm1;

	/* If the region parameter is incorrect, there's little else that makes any sense. */	
	if (region < cs_One || region > 3)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_DENRGN;
	}
	else
	{
		/* Region is valid.  Now, since KMS appears to be reluctant in granting
		   permission to Open Source the coefficients, we have some additional
		   hoops to jump through. */
		switch (region) {
		case 1:
			ok = (prj_code == cs_PRJCOD_SYS34    && C_ttj98 [0] != 0.0 && C_jtt98 [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_99 && C_ttj   [0] != 0.0 && C_jtt   [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_01 && C_ttj   [0] != 0.0 && C_ttj   [0] != 0.0);
			break;
		case 2:
			ok = (prj_code == cs_PRJCOD_SYS34    && C_tts98 [0] != 0.0 && C_stt98 [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_99 && C_tts   [0] != 0.0 && C_stt   [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_01 && C_tts   [0] != 0.0 && C_tts   [0] != 0.0);
			break;
		case 3:
			ok = (prj_code == cs_PRJCOD_SYS34    && C_ttb98 [0] != 0.0 && C_btt98 [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_99 && C_ttb99 [0] != 0.0 && C_btt99 [0] != 0.0 ) ||
				 (prj_code == cs_PRJCOD_SYS34_01 && C_ttb   [0] != 0.0 && C_ttb   [0] != 0.0);
			break;
		default:
			ok = 0;
			break;
		}
		if (!ok)
		{
			/* It appears that the coefficients for the selected vintage/region do
			   not exist.  Therefore, we declare the coordinate system definition
			   to be invalid. */
			if (err_list == NULL) list_sz = 0;
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_SYS34_NOSRC;
		}
	}
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CSsys34S (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**									system parameters.
**
**	This function sets up a cs_Sys34_ structure for use by the
**	remaining mosules in this section.  In C++ jargon, this would
**	be called a constructor.
**
**********************************************************************/
void EXP_LVL9 CSsys34S (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/*  57.29577... */
	extern double cs_Degree;			/*   1.0 / 57.29577... */
	extern double cs_Zero;				/*   0.0 */
	extern double cs_Half;				/*   0.5 */
	extern double cs_One;				/*   1.0 */
	extern double cs_Nine;				/*   9.0 */
	extern double cs_K15;				/*  15.0 */

	struct cs_Sys34_ *sys34;

	sys34 = &csprm->proj_prms.sys34;

	/* We have two varaiations.  The original variation uses
	   a set of polynomials which were devides prior to 1999.  The
	   'A' variation usues a set of polynomials devised in 1999. */
	sys34->vintage = 0;
	if (csprm->prj_code == cs_PRJCOD_SYS34_99)
	{
		sys34->vintage = 1999;
	}
	else if (csprm->prj_code == cs_PRJCOD_SYS34_01)
	{
		sys34->vintage = 2001;
	}
	
	/* The single parameter to this whole mess is the specific zone involved.
	   Since paramteres need to be numeric (actually doubles), we arbitrarily
	   have chosen:

	   prj_prm1 == 1.0  ==> jylland
	   prj_prm1 == 2.0  ==> sjælland
	   prj_prm1 == 3.0  ==> bornholm

	   For each of these zones, there are some basic UTM type parameters
	   involved.
	*/

	sys34->k0 = 0.9996;					/* Standard UTM value. */
	sys34->region = (short)csprm->csdef.prj_prm1;
	sys34->e_rad = csprm->datum.e_rad;
	sys34->ecent = csprm->datum.ecent;
	sys34->e_sq = sys34->ecent * sys34->ecent;
	sys34->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
	
	/* Just in case the System 34 polynomial coefficients are not available,
	   and the Q function test did not prevent this code from being executed
	   for some reason, we force the scale reduction factor to a ridiculous
	   number to make sure that erroneous results are not used. */
	if (C_ttj [0] == 0.0)
	{
		sys34->k0 = 0.000009;
	}

	switch (sys34->region) {
	default:
		/* Should never happen as the Q function verifies a appropriate value. */
	case 1: 
		sys34->cent_lng = cs_Nine * cs_Degree; 
		sys34->org_lat = cs_Zero;
		break;
	case 2:	 
		sys34->cent_lng = cs_Nine * cs_Degree; 
		sys34->org_lat = cs_Zero;
		break; 
	case 3:
		/* They didn't tell me about this, but this is the way it has to be if
		   you want to match the provided test results. */
		if (csprm->prj_code == cs_PRJCOD_SYS34_99 ||
		    csprm->prj_code == cs_PRJCOD_SYS34_01)
		{
			sys34->cent_lng = cs_Nine * cs_Degree; 
		}
		else
		{
			sys34->cent_lng = cs_K15 * cs_Degree; 
		}
		sys34->org_lat = cs_Zero;
		break;
	}
	sys34->k = csprm->csdef.scale * sys34->k0;
	sys34->Rk = sys34->e_rad * sys34->k;

	if (sys34->ecent == 0.0)
	{
		/* Here for a sphere. */
		sys34->Rk_ovr_2 = sys34->Rk * cs_Half;
	}
	else
	{
		/* Here for an ellipsoid. */
		sys34->eprim_sq = sys34->e_sq / (cs_One - sys34->e_sq);

		/* The coefficients for the M calculations. */
		CSmmFsu (&sys34->mmcofF,sys34->e_rad,sys34->e_sq);
		CSmmIsu (&sys34->mmcofI,sys34->e_rad,sys34->e_sq);
	}

	/* Need to compute the maximum value of X which can be
	   supported mathematically.  This is, essentially, the
	   value generated by a del_lng of a skosh less than
	   90 degrees at the equator.  The value 111,000 is a
	   good estimate of the number of meters in a degree
	   at the equator; certainly good enough for this
	   calculation. */
	sys34->xx_max = (111000.0 * 89.0) / csprm->csdef.unit_scl;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */
	csprm->cent_mer = sys34->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 && csprm->csdef.ll_min [LAT] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0 && csprm->csdef.ll_max [LAT] == 0.0)
	{
		/* We use values used by the source of the code which we use.  We
		   presume these values are good numbers.  Note, the source limits
		   were originally in UTM.  In converting them to geographic form
		   we liberalized a bit more. */
		switch (sys34->region) {
		default:
		case 1:
			csprm->min_ll [LNG] =  6.00 - csprm->cent_mer;
			csprm->min_ll [LAT] = 54.00;
			csprm->max_ll [LNG] = 10.00 - csprm->cent_mer;
			csprm->max_ll [LAT] = 58.50;
			break;
		case 2:
			csprm->min_ll [LNG] =  9.75 - csprm->cent_mer;
			csprm->min_ll [LAT] = 54.00;
			csprm->max_ll [LNG] = 13.50 - csprm->cent_mer;
			csprm->max_ll [LAT] = 56.90;
			break;
		case 3:
			if (sys34->vintage < 1999)
			{
				csprm->min_ll [LNG] = 13.50 - csprm->cent_mer;
				csprm->min_ll [LAT] = 54.15;
				csprm->max_ll [LNG] = 16.50 - csprm->cent_mer;
				csprm->max_ll [LAT] = 56.00;
			}
			else
			{
				csprm->min_ll [LNG] = 13.60 - csprm->cent_mer;
				csprm->min_ll [LAT] = 54.25;
				csprm->max_ll [LNG] = 17.00 - csprm->cent_mer;
				csprm->max_ll [LAT] = 55.70;
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
	if (csprm->csdef.xy_min [XX] == 0.0 && csprm->csdef.xy_min [YY] == 0.0 &&
		csprm->csdef.xy_max [XX] == 0.0 && csprm->csdef.xy_max [YY] == 0.0)
	{
		/* Again, these values are from the source of information on the Danish
		   system. */
		switch (sys34->region) {
		default:
		case 1:
			csprm->min_xy [XX] = -S34JXMIN;
			csprm->min_xy [YY] =  S34JYMIN;
			csprm->max_xy [XX] = -S34JXMAX;
			csprm->max_xy [YY] =  S34JYMAX;
			break;
		case 2:
			csprm->min_xy [XX] = -S34SXMIN;
			csprm->min_xy [YY] =  S34SYMIN;
			csprm->max_xy [XX] = -S34JXMAX;
			csprm->max_xy [YY] =  S34JYMAX;
			break;
		case 3:
			csprm->min_xy [XX] = -S45BXMIN;
			csprm->min_xy [YY] =  S45BYMIN;
			csprm->max_xy [XX] = -S45BXMAX;
			csprm->max_xy [YY] =  S45BYMAX;
			break;
		}
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

	/* That's all the calculations we can do at this time.
	   Set up the internal function calls.  Note, since the
	   Transverse Mercator is a conformal projection, the
	   H and K scale factors are the same.  Therefore, we
	   set all three scale function pointers to the same
	   function. */
	csprm->ll2cs    = (cs_LL2CS_CAST)CSsys34F;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSsys34I;
	csprm->cs_scale = (cs_SCALE_CAST)CSsys34K;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSsys34K;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSsys34K;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSsys34C;
	csprm->llchk    = (cs_LLCHK_CAST)CSsys34L;
	csprm->xychk    = (cs_XYCHK_CAST)CSsys34X;

	return;
}

/**********************************************************************
**	rtn_val = CSsys34F (sys34,xy,ll);
**
**	struct cs_Trmer_ *sys34;	structure containing all parameters necessary
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
**	The ll and the xy arrays may share the same memory with no
**	adverse affects.  Yes, much of this code duplicates the
**	standard transverse mercator code, but we do this to avoid
**  a ton of spaghetti.
**
**********************************************************************/
int EXP_LVL9 CSsys34F (Const struct cs_Sys34_ *sys34,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Pi_o_2;			/* Pi over two */
	extern double cs_Mpi_o_2;			/* minus Pi over two */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_AnglTest;			/* .001 seconds of arc in
										   radians. */
	extern double cs_AnglTest1;			/* 1.0 minus .001 seconds of
										   arc radians. */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the south pole,
										   in radians. */
	extern double cs_EETest;			/* .001 seconds of arc
										   short of 90 degrees,
										   in radians. */
	extern double cs_WETest;			/* .001 seconds of arc
										   short of -90 degrees,
										   in radians. */
	extern double cs_Huge;				/* An approximation of
										   infinity, 1.0E+37,
										   but not so large as
										   we can't use it in
										   calculations. */
	extern double cs_Mhuge;				/* Negative infinity. */

	int rtn_val;
	int status;

	double lng, lat;
	double xx, yy;
	double del_lng;

	double sin_lat;
	double cos_lat;
	double tan_lat;
	double tan_lat_sq;

	double nu;
	double eta_sq;

	double A1;
	double A2;
	double A3;
	double A4;
	double A5;
	double A6;
	double A7;
	double B;
	double L;
	double L_sq;
	double M;

	double tmp1;

	rtn_val = cs_CNVRT_NRML;
	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	/* Adjust the input values, if necessary, to prevent a domain error. */
	if (fabs (lat) > cs_NPTest)
	{
		/* Latitude is on, or beyond, a pole. */
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
		if (lat >= cs_NPTest)
		{
			/* North Pole */
			sin_lat = cs_One;
			cos_lat = cs_Zero;
			tan_lat = cs_Huge;
			lat = cs_Pi_o_2;
		}
		else if (lat <= cs_SPTest)
		{
			/* South Pole */
			sin_lat = cs_Mone;
			cos_lat = cs_Zero;
			tan_lat = cs_Mhuge;
			lat = cs_Mpi_o_2;
		}
		else
		{
			sin_lat = sin (lat);
			cos_lat = cos (lat);
			tan_lat = sin_lat / cos_lat;
		}
	}
	else
	{
		/* Otherwise.  Since we have handled the poles,
		   cos (lat) should never be zero here. */
		sin_lat = sin (lat);
		cos_lat = cos (lat);
		tan_lat = sin_lat / cos_lat;
	}

	/* We used cs_Huge above as it isn't so huge as to make the following fail. */
	tan_lat_sq = tan_lat * tan_lat;

	/* That's the latitude, deal with the longitude now. */
	del_lng = lng - sys34->cent_lng;

	/* The following statement deals with the issue of the -180 crack.
	   For example, if the central meridian is -179, and the provided
	   longitude is +179, we want a del_lng of -2. */
	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	/* We can't allow a del_lng value to get too close to PI over 2
	   (i.e. 90 degrees) if the cosine of the latitude is one
	   (i.e. the equator).  Otherwise, all longitude values can be
	   handled.  Note, that we allow delta longitude values greater
	   than 90, it's the specific value of 90 that causes trouble.
	   When delta longitude exceeds 90, the projection just wraps
	   back on itself, thus providing the "rational" results which
	   we must return. */
	if (del_lng > cs_EETest || del_lng < cs_WETest)
	{
		rtn_val = cs_CNVRT_RNG;
		if (fabs (fabs (del_lng) - cs_Pi_o_2) < cs_AnglTest &&
		    fabs (cos_lat) > cs_AnglTest1)
		{
			/* Opps!!! We're in never never land. */
			if (del_lng < 0.0)
			{
				del_lng = cs_Mpi_o_2 + cs_AnglTest;
			}
			else
			{
				del_lng = cs_Pi_o_2 - cs_AnglTest;
			}
		}
	}

	/* OK, we can do our thing pretty safely now. */
	if (sys34->ecent == 0.0)
	{
		/* Here for a sphere.  If the above was written
		   correctly, B will never be precisely +- 1. */
		B = cos_lat * sin (del_lng);
		tmp1 = (cs_One + B) / (cs_One - B);
		xy [XX] = sys34->Rk_ovr_2 * log (tmp1);

		tmp1 = tan_lat / cos (del_lng);
		xy [YY] = sys34->Rk * (atan (tmp1) - sys34->org_lat);
	}
	else
	{
		/* Here for the ellipsoid, using Meade's nested solution.
		   A bit more precise, and significantly faster than
		   Snyder's adaptation of Redfearn's equation which was
		   used previously. */
		eta_sq = sys34->eprim_sq * cos_lat * cos_lat;
		M = CSmmFcal (&sys34->mmcofF,lat,sin_lat,cos_lat);

		tmp1 = sys34->e_sq * sin_lat * sin_lat;
		nu = sys34->e_rad / sqrt (cs_One - tmp1);
		A1 = sys34->k * nu;
		A2 = cs_Half * sys34->k * nu * tan_lat;
		A3 = (1.0 / 6.0) * (cs_One - tan_lat_sq + eta_sq);
		A4 = (1.0 / 12.0) * (cs_Five - tan_lat_sq + eta_sq * (cs_Nine + cs_Four * eta_sq));
		tmp1  = cs_Five - tan_lat_sq * (18.0 - tan_lat_sq);
		tmp1 += eta_sq * (14.0 - 58.0 * tan_lat_sq);
		A5 = (1.0 / 120.0) * tmp1;
		tmp1 = 61.0 - tan_lat_sq * (58.0 - tan_lat_sq);
		tmp1 += eta_sq * (270.0 - 330.0 * tan_lat_sq);
		A6 = (1.0 / 360.0) * tmp1;
		A7 = (1.0 / 5040.0) * (61.0 - tan_lat_sq * (479.0 -
													179.0 * tan_lat_sq +
													tan_lat_sq * tan_lat_sq));
		L = del_lng * cos_lat;
		L_sq = L * L;

		xy [XX] = ((((A7 * L_sq) + A5) * L_sq + A3) * L_sq + cs_One) * A1 * L;
		xy [YY] = ((((A6 * L_sq) + A4) * L_sq + cs_One) * A2 * L_sq) + sys34->k * M;
	}
	xy [XX] += 500000.0;

	/* OK, we have UTM (northern zone) coordinates now.  We apply the appropriate
	   Danish polynomial expansion to produce Danish Cartesian coordinates. */
	switch (sys34->region) {
	default:
		rtn_val = cs_CNVRT_RNG;
		/*lint -fallthrough */
	case 1:
		status = TransformUTM32_S34J (xy [XX],xy [YY],&xx,&yy,sys34->vintage);
		break;
	case 2:	 
		status = TransformUTM32_S34S (xy [XX],xy [YY],&xx,&yy,sys34->vintage);
		break; 
	case 3:
		status = TransformUTM33_S45B (xy [XX],xy [YY],&xx,&yy,sys34->vintage);
		break;
	}
	xy [XX] = xx;
	xy [YY] = yy;
	if (status != 0) rtn_val = cs_CNVRT_RNG;

	CS_quadF (xy,xy [XX],xy [YY],cs_Zero,cs_Zero,sys34->quad);
	return rtn_val;
}
/**********************************************************************
**	rtn_val = CSsys34I (sys34,ll,xy);
**
**	struct cs_Trmer_ *sys34;	structure detailing the Transverse Mercator
**								coordinate system in use.
**	double ll [2];				converted results returned here, longitude ([0])
**								and latitude ([1]), in degrees.
**	double xy [2];				coordinates to be converted are obtained from
**								here, x ([0]) and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The xy and ll arrays may share the same memory.
**********************************************************************/
int EXP_LVL9 CSsys34I (Const struct cs_Sys34_ *sys34,double ll [2],Const double xy [2])
{
	extern double cs_Pi;				/* Pi = 3.14159... */
	extern double cs_Pi_o_2;			/* PI / 2.0 */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Six;				/* 6.0 */
	extern double cs_Eight;				/* 8.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	int rtn_val;
	int status;

	double xx;
	double yy;

	double lat;
	double del_lng;

	double phi1;
	double sin_phi1;
	double cos_phi1;
	double tan_phi1;
	double tan_phi1_sq;

	double eta1_sq;
	double nu1;
	double B2;
	double B3;
	double B4;
	double B5;
	double B6;
	double B7;
	double D;
	double L;
	double M;
	double Q;
	double Q_sq;

	double tmp1;
	double tmp2;

	double myXY [3];

	rtn_val = cs_CNVRT_NRML;

	/* Undo whatever quad stuff may be in place. */
	CS_quadI (&xx,&yy,xy,cs_Zero,cs_Zero,sys34->quad);
	myXY [XX] = xx;
	myXY [YY] = yy;
	myXY [ZZ] = cs_Zero;

	/* Apply the Danish polynomial to give us UTM's. */
	switch (sys34->region) {
	default:
		rtn_val = cs_CNVRT_RNG;
		/*lint -fallthrough */
	case 1:
		status = TransformS34J_UTM32 (myXY [XX],myXY [YY],&xx,&yy,sys34->vintage);
		break;
	case 2: 
		status = TransformS34S_UTM32 (myXY [XX],myXY [YY],&xx,&yy,sys34->vintage);
		break; 
	case 3:
		status = TransformS45B_UTM33 (myXY [XX],myXY [YY],&xx,&yy,sys34->vintage);
		break;
	}
	if (status != 0) rtn_val = cs_CNVRT_RNG;
	xx -= 500000.0;

	/* Convert the UTM's to geographic coordinates.  Your right!!!
	   lots of duplicated code here.  We prefer duplicated code to
	   spaghetti code.

	   Deal with the limiting xx case.  Xx_max is the value which
	   the forward function returns for a latitude of zero and
	   a delta longitude of +-90.0 degrees.  This is the limit
	   of the map.  In any case, it is about 10 times the
	   scaled radius of the earth (Rk).

	   Since latitude is determined primarily by the sin (yy/R),
	   it wraps around automatically.  Therefore, there is no
	   mathematical limit on Y. */
	if (fabs (xx) > sys34->xx_max)
	{
		rtn_val = cs_CNVRT_RNG;
		xx = (xx >= 0.0) ? sys34->xx_max : -sys34->xx_max;
	}

	if (sys34->ecent == 0.0)
	{
		/* Here for a sphere.  From Snyder. */
		D = yy / sys34->Rk + sys34->org_lat;
		tmp1 = xx / sys34->Rk;

		tmp2 = sin (D) / cosh (tmp1);	/* cosh is never < 1 */
		lat = asin (tmp2);

		/* Careful, cos (D) is zero at the poles. */
		tmp2 = cos (D);
		if (fabs (tmp2) > cs_AnglTest)
		{
			del_lng = atan2 (sinh (tmp1),tmp2);
		}
		else
		{
			/* Longitude is undefined at the poles, so
			   we return a cs_CNVRT_INDF. */
			del_lng = cs_Zero;
			rtn_val = cs_CNVRT_INDF;
		}
	}
	else
	{
		/* Here for an ellipsoid.  Phi1 is the footprint
		   latitide, which may (unlikely but possible) be
		   a pole.  We use Synder, page 63 for the footprint
		   latitude. */
		M = yy / sys34->k;
		phi1 = CSmmIcal (&sys34->mmcofI,M);
		cos_phi1 = cos (phi1);
		if (fabs (cos_phi1) < cs_AnglTest)
		{
			/* Here for a pole.  The longitude is not
			   defined at wither pole, so we return
			   cs_CNVRT_INDF to indicate this. */
			del_lng = cs_Zero;
			lat = (phi1 < 0.0) ? -cs_Pi_o_2 : cs_Pi_o_2;
			rtn_val = cs_CNVRT_INDF;
		}
		else
		{
			sin_phi1 = sin (phi1);
			tan_phi1 = sin_phi1 / cos_phi1;
			tan_phi1_sq = tan_phi1 * tan_phi1;

			/* Using Meade's nested solution.  From
			   Maling, page 447. */
			tmp1 = cs_One - sys34->e_sq * sin_phi1 * sin_phi1;
			nu1 = sys34->e_rad / sqrt (tmp1);

			eta1_sq = sys34->eprim_sq * cos_phi1 * cos_phi1;

			Q = xx / (sys34->k * nu1);
			Q_sq = Q * Q;

			B2 = (-1.0 / 2.0) * tan_phi1 * (cs_One + eta1_sq);

			B3 = (-1.0 / 6.0) * (cs_One + cs_Two * tan_phi1_sq + eta1_sq);

			tmp1 = cs_One - cs_Nine * tan_phi1_sq - cs_Four * eta1_sq;
			tmp1 = cs_Five + cs_Three * tan_phi1_sq + eta1_sq * tmp1;
			B4   = (-1.0 / 12.0) * tmp1;

			tmp1  = eta1_sq * (cs_Six + cs_Eight * tan_phi1_sq);
			tmp1 += tan_phi1_sq * (28.0 + 24.0 * tan_phi1_sq);
			B5 = (1.0 / 120.0) * (cs_Five + tmp1);

			/* Some programs use 60 in place of 90 in the following.
			   I believe the 90 is correct.  The difference this
			   makes is very small.  Perhaps why the bug in those
			   programs has never been found.  I've never had bugs
			   like that, of course :-). */
			tmp1 = 46.0 - 252.0 * tan_phi1_sq - 90.0 * (tan_phi1_sq * tan_phi1_sq);
			tmp1 = tan_phi1_sq * (90.0 + 45.0 * tan_phi1_sq) + eta1_sq * tmp1;
			B6   = (61.0 + tmp1) * (1.0 / 360.0);

			tmp1 = 662.0 + 1320.0 * tan_phi1_sq + 720.0 * (tan_phi1_sq * tan_phi1_sq);
			B7   = (-1.0 / 5040.0) * (61.0 + tan_phi1_sq * tmp1);

			L = Q * (cs_One + Q_sq * (B3 + Q_sq * (B5 + B7 * Q_sq)));
			del_lng = L / cos_phi1;

			lat = phi1 + B2 * Q_sq * (cs_One + Q_sq * (B4 + B6 * Q_sq));
		}
	}

	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	ll [LNG] = (del_lng + sys34->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}
/**********************************************************************
**	alpha = CSsys34C (sys34,ll);
**
**	struct m_Trmer_ *sys34;		structure containing all parameters
**									necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double alpha;				the convergence angle, in degrees;
**								i.e. the deviation btewteen true
**								north and grid north.
**
**	We calculate this empirically, no known analytical formula.
**********************************************************************/
double EXP_LVL9 CSsys34C (Const struct cs_Sys34_ *sys34,Const double ll [2])
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
	status = CSsys34F (sys34,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSsys34F (sys34,xy2,my_ll);
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
	if ((fabs (del_xx) + fabs (del_yy)) >= sys34->one_mm)
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
**	k = CSsys34K (sys34,ll);
**
**	struct m_Trmer_ *sys34;		structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double k;					the true map scale factor at the
**								specified point.  Should be
**								compared with the sys34->k.
**********************************************************************/
double EXP_LVL9 CSsys34K (Const struct cs_Sys34_ *sys34,Const double ll [2])
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
	CSsys34F (sys34,xy1,ll1);

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + cs_HlfSecDeg;
	CSsys34F (sys34,xy2,ll2);

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	CS_llazdd (sys34->Rk,sys34->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point.  I don't think
	   ll_dd can be zero since we know that ll1 id not the
	   same as ll0.  However, just to be safe: */

	if (fabs (ll_dd) > sys34->one_mm)
	{
		kk = xy_dd / ll_dd;
	}
	else
	{
		kk = cs_SclInf;
	}
	return (kk);
}
/**********************************************************************
**	status = CSsys34L (sys34,cnt,pnts);
**
**	struct cs_Trmer_ *sys34;	the coordinate system against which the check is
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
int EXP_LVL9 CSsys34L (Const struct cs_Sys34_ *sys34,int cnt,Const double pnts [][3])
{
	int status;

	/* Check all the points. */

	status = cs_CNVRT_OK;
	return (status);
}
/**********************************************************************
**	status = CSsys34X (sys34,cnt,pnts);
**
**	struct cs_Trmer_ *sys34;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSsys34L to check lat/long
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
int EXP_LVL9 CSsys34X (Const struct cs_Sys34_ *sys34,int cnt,Const double pnts [][3])
{
	int rtn_val;

	rtn_val = cs_CNVRT_OK;

	/* All Y values are within the domain of the function.  However,
	   in the cartesian frame of reference, an absolute value of
	   X greater than xx_max is outside of the domain. */
	return (rtn_val);
}

/*******************************************************************************
********************************************************************************
** CS-MAP has now added the "vintage" parameter to all this.   This parameter **
** selects between the two (currently) different sets of polynomials used     **
** for the calculation.  A value of 1999 specifies the set of polynomials     **
** devised in November 1999.  Any value less than 1999 indicates the          **
** original set of polynomials (which we also refer to as pre-1999).          **
**                                                                            **  
********************************************************************************
*******************************************************************************/

/* Inverse for three zones, three functions. */
int TransformS34J_UTM32 (double x1, double y1,double* x2, double* y2,short vintage)
{
	extern double cs_Zero;

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	fwdPtr = (vintage < 1999) ? C_jtt98 : C_jtt;
	invPtr = (vintage < 1999) ? C_ttj98 : C_ttj;

	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (-x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			/* kmsPolynomial returns 1 if the input coordinates result in
			   rather absurd numbers for the polynomial calculation.
			   Redundant, but rather inexpensive. */
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			/* The official KMS way of determining if the input coordinate
			   is in range is to see of the inverse produces the original
			   coordinate within 4 centimeters.  SO, that's what we do, if
			   we can. */
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs ((-x1) - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else
		{
			/* If the inverse polynomials are not present, we use the following
			   to determine if the coordinate is in range.  Unfortunately, the
			   values (as provided by KMS) are rather liberal and will allow
			   a fwd/inv discrepancies of as much as 7 meters. */
			if (*x2 < T32JEMIN || *x2 > T32JEMAX || *y2 < T32JNMIN || *y2 > T32JNMAX)
			{
				rtnValue = 1;
			}
		}
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}
/* Pretty much the same as the above, without the comments. */
int TransformS34S_UTM32 (double x1, double y1,double* x2, double* y2,short vintage)
{
	extern double cs_Zero;

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	if ((-x1) < S34SXMIN || (-x1) > S34SXMAX || y1 < S34SYMIN || y1 > S34SYMAX)
	{
		rtnValue = 1;
	}

	fwdPtr = (vintage < 1999) ? C_stt98 : C_stt;
	invPtr = (vintage < 1999) ? C_tts98 : C_tts;
	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (-x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs ((-x1) - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else
		{
			if (*x2 < T32SEMIN || *x2 > T32SEMAX || *y2 < T32SNMIN || *y2 > T32SNMAX)
			{
				rtnValue = 1;
			}
		}
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}
int TransformS45B_UTM33 (double x1,double y1,double* x2,double* y2,short vintage)
{
	extern double cs_Zero;

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	if ((-x1) < S45BXMIN || (-x1) > S45BXMAX || y1 < S45BYMIN || y1 > S45BYMAX)
	{
		rtnValue = 1;
	}

	if (vintage < 1999)
	{
		fwdPtr = C_btt98;
		invPtr = C_ttb98;
	}
	else if (vintage == 1999)
	{
		fwdPtr = C_btt99;
		invPtr = C_ttb99;
	}
	else
	{
		fwdPtr = C_btt;
		invPtr = C_ttb;
	}

	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (-x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs ((-x1) - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else if (vintage < 1999)
		{
			if (*x2 < T33BEMIN || *x2 > T33BEMAX || *y2 < T33BNMIN || *y2 > T33BNMAX)
			{
				rtnValue = 1;
			}
		}
		else
		{
			if (*x2 < T32BEMIN || *x2 > T32BEMAX || *y2 < T32BNMIN || *y2 > T32BNMAX)
			{
				rtnValue = 1;
			}
		}
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}

/* Forward for three zones, three functions. */
int TransformUTM32_S34J (double x1, double y1,double* x2,double* y2,short vintage)
{
	extern double cs_Zero;
	extern double C_ttj [];			/* UTM50-32 -> System 34-Jylland, circa 1991 */
	extern double C_ttj98 [];		/* UTM50-32 -> System 34-Jylland, pre 1991 */

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	if (x1 < T32JEMIN || x1 > T32JEMAX || y1 < T32JNMIN || y1 > T32JNMAX)
	{
		rtnValue = 1;
	}

	fwdPtr = (vintage < 1999) ? C_ttj98 : C_ttj;
	invPtr = (vintage < 1999) ? C_jtt98 : C_jtt;
	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs (x1 - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else
		{
			if (*x2 < S34JXMIN || *x2 > S34JXMAX || *y2 < S34JYMIN || *y2 > S34JYMAX)
			{
				rtnValue = 1;
			}
		}
		*x2 = -*x2;
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}
int TransformUTM32_S34S (double x1,double y1,double *x2, double *y2,short vintage)
{
	extern double cs_Zero;

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	if (x1 < T32SEMIN || x1 > T32SEMAX || y1 < T32SNMIN || y1 > T32SNMAX)
	{
		rtnValue = 1;
	}

	fwdPtr = (vintage < 1999) ? C_tts98 : C_tts;
	invPtr = (vintage < 1999) ? C_stt98 : C_stt;
	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs (x1 - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else
		{
			if (*x2 < S34SXMIN || *x2 > S34SXMAX || *y2 < S34SYMIN || *y2 > S34SYMAX)
			{
				rtnValue = 1;
			}
		}
		*x2 = -*x2;
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}
int TransformUTM33_S45B (double x1,double y1,double *x2,double *y2,short vintage)
{
	extern double cs_Zero;

	int st;
	int rtnValue;
	double *fwdPtr;
	double *invPtr;
	double x3, y3;

	rtnValue = 0;
	if (vintage < 1999)
	{
		if (x1 < T33BEMIN || x1 > T33BEMAX || y1 < T33BNMIN || y1 > T33BNMAX)
		{
			rtnValue = 1;
		}
	}
	else
	{
		if (x1 < T32BEMIN || x1 > T32BEMAX || y1 < T32BNMIN || y1 > T32BNMAX)
		{
			rtnValue = 1;
		}
	}

	if (vintage < 1999)
	{
		fwdPtr = C_ttb98;
		invPtr = C_btt98;
	}
	else if (vintage == 1999)
	{
		fwdPtr = C_ttb99;
		invPtr = C_btt99;
	}
	else
	{
		fwdPtr = C_ttb;
		invPtr = C_btt;
	}

	if (*fwdPtr != 0.0)
	{
		st = kmsPolynomial (x1,y1,x2,y2,fwdPtr);
		if (st != 0)
		{
			rtnValue = 1;
		}
		else if (*invPtr != 0.0)
		{
			st = kmsPolynomial (*x2,*y2,&x3,&y3,invPtr);
			if (st == 0)
			{
				rtnValue = (fabs (x1 - x3) > cs_KmsTolerance) || (fabs (y1 - y3) > cs_KmsTolerance);
			}
			else
			{
				rtnValue = 1;
			}
		}
		else
		{
			if (*x2 < S45BXMIN || *x2 > S45BXMAX || *y2 < S45BYMIN || *y2 > S45BYMAX)
			{
				rtnValue = 1;
			}
		}
		*x2 = -*x2;
	}
	else
	{
		CS_erpt (cs_SYS34_NOSRC);
		rtnValue = -1;
		*x2 = cs_Zero;
		*y2 = cs_Zero;
	}
	return rtnValue;
}
int kmsPolynomial (double Xin,double Yin,double *xout,double *yout,Const double coefPtr [])
{
	extern double cs_Zero;
	extern double cs_One;

	int rtnValue;
	unsigned order;
	unsigned coefCnt;
	unsigned xIdx, yIdx;

	double xm, ym;
	double xmPwr, ymPwr;
	double X0, Y0;
	double P, Q;
	double *xCoefPtr, *yCoefPtr;

	/* Until we know different. */
	rtnValue = 0;

	/* Prepare for polynomial calculation. */
	order = (unsigned)*coefPtr++;
	coefCnt  = (order + 1) * (order + 2) / 2;
	yCoefPtr = (double*)coefPtr;
	xCoefPtr = (double*)coefPtr + coefCnt + 1;
	Y0 = *(yCoefPtr + coefCnt);
	X0 = *(xCoefPtr + coefCnt);

	/* Adjust for the origina of the transformation. */
	xm = Xin - X0;
	ym = Yin - Y0;

	/* In the following, the literal constants are arbitraily choosen.  The
	   sole purpose of this code is to prevent floating point exceptions in
	   the polynomial expansion coded below. */
	if (fabs (xm) < 1.0E-12) xm = cs_Zero;
	if (xm >  1.0E+12)
	{
		xm =  1.0E+12;
		rtnValue = 1;
	}
	if (xm < -1.0E+12)
	{
		xm = -1.0E+12;
		rtnValue = 1;
	}
	if (fabs (ym) < 1.0E-12) ym = cs_Zero;
	if (ym >  1.0E+12)
	{
		ym =  1.0E+12;
		rtnValue = 1;
	}
	if (ym < -1.0E+12)
	{
		ym = -1.0E+12;
		rtnValue = 1;
	}

	/* Calculate the polynomial. */
	P = cs_Zero;
	xmPwr = cs_One;
	for (yIdx = 0;yIdx <= order;yIdx++)
	{
		ymPwr = cs_One;
		for (xIdx = yIdx;xIdx <= order;xIdx++)
		{
			P += ymPwr * xmPwr * *yCoefPtr++;
			ymPwr *= ym;
		}
		xmPwr *= xm;
	}

	Q = cs_Zero;
	ymPwr = cs_One;
	for (xIdx = 0;xIdx <= order;xIdx++)
	{
		xmPwr = cs_One;
		for (yIdx = xIdx;yIdx <= order;yIdx++)
		{
			Q += ymPwr * xmPwr * *xCoefPtr++;
			xmPwr *= xm;
		}
		ymPwr *= ym;
	}

	*yout = P;
	*xout = Q;

	return rtnValue;
}

