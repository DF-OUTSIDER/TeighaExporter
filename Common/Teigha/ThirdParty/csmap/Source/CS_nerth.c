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

/*lint -esym(715,prj_code,ll,nerth) */  /* function argument not referenced */

#include "cs_map.h"

/*lint -esym(613,err_list)  possible use of null pointerl but not really */

/**********************************************************************
**	err_cnt = CSnerthQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSnerthQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Non-Earth specific stuff. */
	if (cs_def->dat_knm [0] != '\0')
	{
		/* Must be referenced to an ellipsoid, not a datum. */
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_NRDATUM;
	}
	/* That's it for Non-Earth system. */

	return (err_cnt + 1);
}

/**********************************************************************
**	CSnerthS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	This projection is supported for the sphere only.  We use
**	the equatorial radius of the related ellipsoid for the
**	radius of the sphere.
**
**********************************************************************/

void EXP_LVL9 CSnerthS (struct cs_Csprm_ *csprm)
{
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Degree;
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];

	struct cs_Nerth_ *nerth;

	nerth = &csprm->proj_prms.nerth;

	/* Transfer the necessary arguments to the "nerth" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	nerth->prjCode = csprm->prj_code;
	nerth->k = csprm->csdef.scale;
	nerth->x_off = csprm->csdef.x_off;
	nerth->y_off = csprm->csdef.y_off;
	nerth->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
	nerth->factor = 1.0E-50 / nerth->k;

	if (nerth->prjCode == cs_PRJCOD_NRTHSRT)
	{
		nerth->scale     = csprm->csdef.prj_prm1;
		nerth->rotation  = csprm->csdef.prj_prm2;
		nerth->rotOrgX   = csprm->csdef.prj_prm3;
		nerth->rotOrgY   = csprm->csdef.prj_prm4;
		nerth->sinRot    = sin (nerth->rotation * cs_Degree);
		nerth->cosRot    = cos (nerth->rotation * cs_Degree);
	}
	else
	{
		nerth->scale     = cs_One;
		nerth->rotation  = cs_Zero;
		nerth->rotOrgX   = cs_Zero;
		nerth->rotOrgY   = cs_Zero;
		nerth->sinRot    = cs_Zero;
		nerth->cosRot    = cs_One;
	}

	/* Set up the coordinate checking information.  We do not
	   allow user specified geographic limits since these things
	   do not produce legitimate geographic coordinates. */

	csprm->cent_mer = cs_Zero;
	csprm->min_ll [LNG] = -nerth->factor;
	csprm->max_ll [LNG] =  nerth->factor;
	csprm->min_ll [LAT] = -nerth->factor;
	csprm->max_ll [LAT] =  nerth->factor;

	/* We allow and use user defined cartesian coordinates. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		/* No specification in the coordinate system definition. */
		csprm->min_xy [XX] = -10000000.00;
		csprm->max_xy [XX] =  10000000.00;
		csprm->min_xy [YY] = -10000000.00;
		csprm->max_xy [YY] =  10000000.00;
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
	csprm->ll2cs    = (cs_LL2CS_CAST)CSnerthF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSnerthI;
	csprm->cs_scale = (cs_SCALE_CAST)CSnerthK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSnerthK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSnerthK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSnerthC;
	csprm->llchk    = (cs_LLCHK_CAST)CSnerthL;
	csprm->xychk    = (cs_XYCHK_CAST)CSnerthX;

	return;
}

/**********************************************************************
**	rtn_val = CSnerthF (nerth,xy,ll);
**
**	struct cs_Nerth_ *nerth;	structure which defines the parameters
**								in effect for the transformation.
**	double xy [2];				the results are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				intermediate coordinates, not really
**								geographic coordinates.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**********************************************************************/

int EXP_LVL9 CSnerthF (Const struct cs_Nerth_ *nerth,double xy [2],Const double ll [2])
{
	int rtn_val;
	double lclX, lclY;

	rtn_val = cs_CNVRT_NRML;
	xy [XX] = ll [0] / nerth->factor;
	xy [YY] = ll [1] / nerth->factor;
	if (nerth->quad == 0)
	{
		xy [XX] += nerth->x_off;
		xy [YY] += nerth->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],nerth->x_off,nerth->y_off,nerth->quad);
	}

	if (nerth->prjCode == cs_PRJCOD_NRTHSRT)
	{
		lclX = xy [XX] - nerth->rotOrgX;
		lclY = xy [YY] - nerth->rotOrgY;
		xy [XX] = nerth->cosRot * lclX - nerth->sinRot * lclY;
		xy [YY] = nerth->cosRot * lclY + nerth->sinRot * lclX;
		
		xy [XX] /= nerth->scale;
		xy [YY] /= nerth->scale;
		xy [XX] += nerth->rotOrgX;
		xy [YY] += nerth->rotOrgY;
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSnerthI (nerth,ll,xy);
**
**	struct cs_Nerth_ *nerth;	structure which defines the parameters
**								in effect for the transformation.
**	double ll [2];				intermediary coordinates.
**	double xy [2];				the coordinates to be converted, x ([0])
**								and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and xy arguments of this function may point to the same
**	array with no adverse affects.
**
**********************************************************************/

int EXP_LVL9 CSnerthI (Const struct cs_Nerth_ *nerth,double ll [2],Const double xy [2])
{
	int rtn_val;
	double x, y;
	double lclX, lclY;
	double lclXY [2];

	rtn_val = cs_CNVRT_NRML;

	lclXY [XX] = xy [XX];
	lclXY [YY] = xy [YY];
	if (nerth->prjCode == cs_PRJCOD_NRTHSRT)
	{
		lclX = lclXY [XX] - nerth->rotOrgX;
		lclY = lclXY [YY] - nerth->rotOrgY;
		lclXY [XX] = nerth->cosRot * lclX + nerth->sinRot * lclY;
		lclXY [YY] = nerth->cosRot * lclY - nerth->sinRot * lclX;

		lclXY [XX] *= nerth->scale;
		lclXY [YY] *= nerth->scale;
		lclXY [XX] += nerth->rotOrgX;
		lclXY [YY] += nerth->rotOrgY;
	}

	if (nerth->quad == 0)
	{
		x = lclXY [XX] - nerth->x_off;
		y = lclXY [YY] - nerth->y_off;
	}
	else
	{
		CS_quadI (&x,&y,lclXY,nerth->x_off,nerth->y_off,nerth->quad);
	}

	ll [LNG] = x * nerth->factor;
	ll [LAT] = y * nerth->factor;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSnerthC (nerth,ll);
**
**	struct cs_Nerth_ *nerth;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Equidistant Cylindrical
**								projection.
**	double ll [2];				ignored
**	double gamma;				returns 0.0
**********************************************************************/

double EXP_LVL9 CSnerthC (Const struct cs_Nerth_ *nerth,Const double ll [2])
{
	extern double cs_Zero;
	return cs_Zero;
}

/**********************************************************************
**	kk = CSnerthK (nerth,ll);
**
**	struct cs_Nerth_ *nerth;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal Equal
**								Area projection.
**	double ll [2];				ignored
**	double kk;					1.0
**********************************************************************/

double EXP_LVL9 CSnerthK (Const struct cs_Nerth_ *nerth,Const double ll [2])
{
	return nerth->scale;
}

/**********************************************************************
**	status = CSnerthL (nerth,cnt,pnts);
**
**	struct cs_Nerth_ *nerth;	the coordinate system against which the
**								check is to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][3];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line,
**								or region is completely within the domain
**								of the coordinate system.  Otherwise,
**								cs_CNVRT_DOMN is returned.
**********************************************************************/

int EXP_LVL9 CSnerthL (Const struct cs_Nerth_ *nerth,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	/* Check all the points.  */
	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		if (fabs (pnts [ii][LNG]) > 1.0E-40 ||
		    fabs (pnts [ii][LAT]) > 1.0E-40)
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}
	return (status);
}							/*lint !e715 */

/**********************************************************************
**	status = CSnerthX (nerth,cnt,pnts);
**
**	struct cs_Nerth_ *nerth;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**********************************************************************/

int EXP_LVL9 CSnerthX (Const struct cs_Nerth_ *nerth,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;
	double test_xy [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic
	   range.  No special checks are required for lines and/or
	   regions. */
	for (ii = 0;ii < cnt;ii++)
	{
		test_xy [XX] = fabs (pnts [ii][XX]);
		test_xy [YY] = fabs (pnts [ii][YY]);
		if (test_xy [XX] >= 1.0E+07 || test_xy [XX] < 1.0E-40 ||
			test_xy [YY] >= 1.0E+07 || test_xy [YY] < 1.0E-40)
		{
			rtn_val = cs_CNVRT_USFL;
			break;
		}
	}
	return (rtn_val);
}
