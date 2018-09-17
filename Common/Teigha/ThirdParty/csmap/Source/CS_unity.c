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
**	err_cnt = CSunityQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSunityQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_K360;			/* +360.0 */
	extern double cs_K540;			/* +40.0 */

	int err_cnt;

	double unit_k;
	double usr_rng;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Unity specific stuff. */
	
	if (cs_def->org_lng < cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* The user desired range for longitude is specified in user
	   units, so we need to be able to convert back to degrees. */

	unit_k = CS_unitlu (cs_UTYP_ANG,cs_def->unit);

	/* In normal situations, the unit is already checked, and the
	   following is not necessary. But, we don't want to ever divide
	   by zero. SO!!! */

	if (unit_k == 0.0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_UNIT;
	}
	else if (cs_def->prj_prm1 != 0.0 || cs_def->prj_prm2 != 0.0)
	{
		/* If a range is specified, the range must be equal to or
		   greater than 360, but less than 540. */

		usr_rng = (cs_def->prj_prm2 - cs_def->prj_prm1) * unit_k;
		if (usr_rng < 0.0)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_RNGORD;
		}
		else if (usr_rng < cs_K360 || usr_rng > cs_K540)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_LLRNG;
		}
	}
	return (err_cnt + 1);
}

/**********************************************************************
**	CSunityS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation and carries
**								the results of the setup.
**
**	This function, and its compatriots, exists simply so that an
**	executable which is capable of converting from one coordinate
**	system to another can also convert from (or to) latitudes and
**	longitudes.  Units translation is al that is done by these
**	functions.
**
**	For these coordinate systems, ll_min and ll_max apply to the
**	internal representation of the lat/longs.  xy_min and xy_max
**	apply to the external, i.e. user, representation of
**	lat/longs.  Internally, we always deal in degrees relative
**	to Greenwhich, and we do not allow anything outside the
**	range of -270 and +270 EXCLUSIVE.  The user representation
**	can be any of several units, and can have any range.
**	Specifically, a range of 0.0 to 360 is supported. User
**	ranges are INCLUSIVE.
**********************************************************************/

void EXP_LVL9 CSunityS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Half;				/*    0.5 */
	extern double cs_One;				/*    1.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_K360;				/*  360.0 */

	struct cs_Unity_ *unity;

	double my_range;
	
	unity = &csprm->proj_prms.unity;

	unity->gwo_lng = csprm->csdef.org_lng;
	unity->unit_s = csprm->csdef.scale;
	unity->k = cs_One;
	unity->usr_2pi = cs_K360 * unity->unit_s;
	unity->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Usr_min, usr_max, and usr_rng are in user units. */
	
	if (csprm->csdef.prj_prm1 != 0.0 ||
	    csprm->csdef.prj_prm2 != 0.0)
	{
		unity->usr_min = (csprm->csdef.prj_prm1 - unity->gwo_lng) * unity->unit_s;
		unity->usr_max = (csprm->csdef.prj_prm2 - unity->gwo_lng) * unity->unit_s;
	}
	else
	{
		unity->usr_min = cs_Km180 * unity->unit_s;
		unity->usr_max = cs_K180  * unity->unit_s;
	}
	unity->usr_rng = unity->usr_max - unity->usr_min;
	my_range = (unity->usr_rng * cs_Half) / unity->unit_s;

	/* Set up the useful limits.  In this case, the ll_min/max
	   are defined in internal units (i.e. Greenwhich, degrees)
	   where the XY units are defined in user units (e.g.
	   Meridian of Paris, grads, etc.).  There really is no
	   useful range limit, but we will set values which will
	   catch most operator screw ups.  If these limits are to
	   severe, the user can always define their own limits. */

	csprm->cent_mer = unity->gwo_lng;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* The definition does not contain any useful limits,
		   we'll provide some.  In general, we allow some
		   wrap around in longitude, but not around the
		   poles. */

		csprm->min_ll [LNG] = -my_range;
		csprm->max_ll [LNG] =  my_range;
		csprm->min_ll [LAT] = cs_Km90;
		csprm->max_ll [LAT] = cs_K90;
	}
	else
	{
		/* The definition includes a useful range specification.
		   We use these values without checking.  We expect the
		   user to give us absolute values, and we convert
		   to values relative to the central meridian. */

		csprm->min_ll [LNG] = CS_adj270 (csprm->csdef.ll_min [LNG]);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj270 (csprm->csdef.ll_max [LNG]);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* In this case, we are dealing with lat/long's, but in the
	   external reference system.  I.e. the central meridian may
	   be other than Greenwhich, the units other than degrees. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		/* No specification in the coordinate system definition.
		   In this rather special case, we can simply convert
		   the min/max of internal to be the min/max of
		   external. */

		csprm->min_xy [XX] = unity->usr_min;
		csprm->min_xy [YY] = cs_Km90 * unity->unit_s;

		csprm->max_xy [XX] = unity->usr_max;
		csprm->max_xy [YY] = cs_K90 * unity->unit_s;
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSunityF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSunityI;
	csprm->cs_scale = (cs_SCALE_CAST)CSunityK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSunityK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSunityK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSunityC;
	csprm->llchk    = (cs_LLCHK_CAST)CSunityL;
	csprm->xychk    = (cs_XYCHK_CAST)CSunityX;

	return;
}

/**********************************************************************
**	rtn_val = CSunityF (unity,xy,ll);
**
**	struct cs_Unity_ *unity;	structure which defines the parameters
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
**	This function essentially converts lat/longs from the
**	internal representation of degrees relative to Greenwich
**	to the user representation.  We assume that the numbers
**	provided have come from a CS-MAP function and, therefore,
**	should not need to be normalized.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	This function, and its compatriots, are made available so that
**	programs designed to convert from one coordinate system to
**	another can also convert from one coordniate system to lat/longs
**	and vice versa.  In this case, all we need do is units
**	translation.
**********************************************************************/

int EXP_LVL9 CSunityF (Const struct cs_Unity_ *unity,double xy [2],Const double ll [2])
{
	extern double cs_Zero;			/*    0.0 */
	extern double cs_K90;			/*   90.0 */
	extern double cs_Km90;			/*  -90.0 */
	extern double cs_K270;			/*  270.0 */
	extern double cs_Km270;			/* -270.0 */

	int rtn_val;

	double lng;

	rtn_val = cs_CNVRT_NRML;

	/* If the lat/long we have been provided are not within internal
	   standards, we have a domain problem.  This should only happen
	   if we are called directly by an application program. */

	xy [XX] = ll [LNG];
	xy [YY] = ll [LAT];
	if (xy [XX] <= cs_Km270 || xy [XX] > cs_K270)
	{
		rtn_val = cs_CNVRT_DOMN;
		xy [XX] = CS_adj270 (xy [XX]);
	}
	if (xy [YY] < cs_Km90 || xy [YY] > cs_K90)
	{
		rtn_val = cs_CNVRT_DOMN;
		xy [YY] = CS_adj90 (xy [YY]);
	}

	/* Convert to user form. */

	xy [XX] = (xy [XX] - unity->gwo_lng) * unity->unit_s;
	xy [YY] =  xy [YY] * unity->unit_s;

	/* Normalize longitude, if necessary, to acheive the user's
	   desired range. */


	if (xy [XX] < unity->usr_min || xy [XX] > unity->usr_max)
	{
		lng = xy [XX] - unity->usr_min;
		lng = fmod (lng,unity->usr_2pi);
		if (lng < 0.0) lng += unity->usr_2pi;
		if (lng > unity->usr_rng) lng -= unity->usr_2pi;
		xy [XX] = unity->usr_min + lng;
	}
	if (unity->quad != 0)
	{
		CS_quadF (xy,xy [XX],xy [YY],cs_Zero,cs_Zero,unity->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSunityI (unity,ll,xy);
**
**	struct cs_Unity_ *unity;	structure which defines the parameters
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
**	This function converts lat/long's in user form into the
**	internal form which we use: degrees relative to Greenwich.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	This function, and it compatriots, exist so that a executable
**	designed to convert from one coordinate system to another
**	can also convert to and from latitude and longitudes.
**	In this case, all we need do is units translation.
**********************************************************************/

int EXP_LVL9 CSunityI (Const struct cs_Unity_ *unity,double ll [2],Const double xy [2])
{
	extern double cs_Zero;				/*    0.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */

	int rtn_val;

	double lcl_xy [2];

	rtn_val = cs_CNVRT_NRML;

	/* Apply the quadrant processing. This is usually used to make
	   west longitude positive. */

	if (unity->quad == 0)
	{
		lcl_xy [XX] = xy [XX];
		lcl_xy [YY] = xy [YY];
	}
	else
	{
		CS_quadI (&lcl_xy [XX],&lcl_xy [YY],xy,cs_Zero,cs_Zero,unity->quad);
	}

	/* See if the user supplied values are with the user's
	   specified range.  If not, we have a domain error,
	   and we normalize to the user's specified range
	   before proceeding. */

	if (lcl_xy [XX] < unity->usr_min || lcl_xy [XX] > unity->usr_max)
	{
		rtn_val = cs_CNVRT_DOMN;
		lcl_xy [XX] -= unity->usr_min;
		lcl_xy [XX] = fmod (lcl_xy [XX],unity->usr_2pi);
		lcl_xy [XX] += unity->usr_min;
	}

	/* Convert the supplied values to internal units and
	   referencing. */

	ll [LNG] = unity->gwo_lng + (lcl_xy [XX] / unity->unit_s);
	ll [LAT] = lcl_xy [YY] / unity->unit_s;

	/* Normalize the longitude to internal standards. */

	ll [LNG] = CS_adj270 (ll [LNG]);

	/* If the resulting latitude is out of range, we do have a
	   problem. */


	if (ll [LAT] < cs_Km90 || ll [LAT] > cs_K90)
	{
		/* Not within the user's expected range. */

		rtn_val = cs_CNVRT_RNG;
		ll [LAT] = CS_adj90 (ll [LAT]);
	}
	return (rtn_val);
}

/**********************************************************************
**	cnvrg = CSunityC (unity,ll);
**
**	struct cs_Unity_ *unity;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**								degrees.
**	double cnvrg;				the convergence angle at the specified point.
**
**	This function, and its compatriots, exist so that an executable
**	which converts from one coordinate system to another can be used
**	to convert from latitudes and longitudes or vice versa.  Most
**	of the extraneous code simply reflects the structure in which
**	the function must operate.
**********************************************************************/

double EXP_LVL9 CSunityC (Const struct cs_Unity_ *unity,Const double ll [2])
{
	extern double cs_Zero;

	return (cs_Zero);
}										/*lint !e715 */

/**********************************************************************
**	k = CSunityK (unity,ll);
**
**	struct cs_Unity_ *unity;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**								degrees.
**	double k;	the true map scale factor at the specified point.
**
**	This function, and its compatriots, exist so that an executable
**	which converts from one coordinate system to another can be used
**	to convert from latitudes and longitudes or vice versa.  Most
**	of the extraneous code simply reflects the structure in which
**	the function must operate.
**********************************************************************/

double EXP_LVL9 CSunityK (Const struct cs_Unity_ *unity,Const double ll [2])
{
	return (unity->k);
}											/*lint !e715 */

/**********************************************************************
**	status = CSunityL (unity,cnt,pnts);
**
**	struct cs_Unity_ *unity;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][2];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line, or region
**								is completely within the domain of the
**								coordinate system.  Otherwise, cs_CNVRT_DOMN
**								is returned.
**
**	In this function, we consider the lat/longs to be internal
**	lat/longs: in degrees relative to Greenwich.  Thus, any
**	lat/long which meets our internal standards is considered
**	legitimate.  Therefore, we don't reallyl need the unity
**	argument.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**********************************************************************/

int EXP_LVL9 CSunityL (Const struct cs_Unity_ *unity,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/*  1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/*  PI over 2 */
	extern double cs_3Pi_o_2;		/* 3PI over 2 */

	int ii;
	int status;

	double lat;
	double lng;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		lng = pnts [ii][LNG] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2 ||
		    fabs (lng) >= cs_3Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}										/*lint !e715 */

/**********************************************************************
**	status = CSunityX (unity,cnt,pnts);
**
**	struct cs_Unity_ *unity;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSunityL to check lat/long
**	values.
**
**	This is a rather special case.  We we should be presented
**	with is lat/longs in the user's form.  Thus, we are checking
**	to see if the user's lat/long's, when converted to internal
**	standards, meet internal standards.
**********************************************************************/

int EXP_LVL9 CSunityX (Const struct cs_Unity_ *unity,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Simply check that all X's and Y's are within the basic
	   range. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSunityI (unity,test_ll,pnts [ii]);
		if (status != cs_CNVRT_OK)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
