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
**	rtn_val = CS_cs2ll (csprm,ll,xy);
**
**	struct cs_Csprm_ *csprm;	coordinate system structure as obtained
**								from CS_csloc.
**	double ll [3];				the longitude ([0]), latitude ([1]), and
**								height ([2]) are returned here.
**	double xy [3];				coordinate system values to be converted.
**	int rtn_val;				returns cs_CNVRT_NRML if the conversion was
**								within the domain of the coordinate system,
**								otherwise cs_CNVRT_RNG is returned.
**
**	The ll and xy arrays may be the same array, with no adverse
**	affects.
**
**	The definition of the coordinate system is setup in the
**	cs_Csprm_ structure by CS_csloc.  All values required for
**	any specific conversion are contained in this structure.
**	In this manner, any module which needs to convert units
**	in any direction can do so without knowing anything
**	about the coordinate system, the projection, or the
**	datum being used.
**
**	The values returned are guaranteed to meet CS-MAP internal
**	standards for lat/longs in degrees as defined by CS_adjll.
**********************************************************************/

int EXP_LVL3 CS_cs2ll (Const struct cs_Csprm_ *csprm,double ll [3],Const double xy [3])
{
	int rtn_val;

	/* Convert the cartesian coordinates to latitude and
	   longitude.  A pointer to the appropriate function is
	   in the csprm structure.  We don't have to know anything
	   at all about exactly what is going on. */

	rtn_val = (*csprm->cs2ll)(&csprm->proj_prms,ll,xy);
	
	/* Add some real stuff here to do the 'Z' */
	ll [2] = xy [2];

	return (rtn_val);
}

int EXP_LVL3 CS_cs3ll (Const struct cs_Csprm_ *csprm,double ll [3],Const double xy [3])
{
	int rtn_val;

	/* Convert the cartesian coordinates to latitude and
	   longitude.  A pointer to the appropriate function is
	   in the csprm structure.  We don't have to know anything
	   at all about exactly what is going on. */

	rtn_val = (*csprm->cs2ll)(&csprm->proj_prms,ll,xy);

	/* Convert the Z value to meters.  Until we have a means of
	   defining the unit of the Z coordinate, we skip this for geographic
	   coordinate systems. */
	if ((csprm->prj_flags & cs_PRJFLG_GEOGR) == 0)
	{
		ll [HGT] = xy [ZZ] * csprm->csdef.unit_scl;
	}
	else
	{
		ll [HGT] = xy [ZZ];
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CS_ll2cs (csprm,xy,ll);
**
**	struct cs_Csprm_ *csprm;	coordinate system structure as obtained from
**								CS_csloc.
**	double xy [3];				coordinate system values returned here, x ([0]),
**								y ([1]), and z([2]).
**	double ll [3];				the longitude ([0]), latitude ([1]), and
**								height ([2]) to be converted (in degrees).
**	int rtn_val;				returns cs_CNVRT_NRML if the conversion was
**								within the domain of the coordinate system;
**								otherwise cs_CNVRT_RNG is returned.
**
**	The ll and xy arrays may be the same, with no adverse
**	affects.
**
**	The definition of the coordinate system is setup in the
**	cs_Csprm_ structure by CS_csloc.  All values required for
**	any specific conversion are contained in this structure.
**	In this manner, any module which needs to convert units
**	in any direction can do so without knowing anything
**	about the coordinate system, the projection, or the
**	datum being used.
**
**	This function will force lat/long values to meet CS-MAP
**	internal standards by calling CS_adjll prior to passing
**	the lat/long pair of the actual projection code.
**********************************************************************/

int EXP_LVL3 CS_ll2cs (Const struct cs_Csprm_ *csprm,double xy [3],Const double ll [3])

{
	extern double cs_Zero;			/* 0.0 */

	int rtn_val;

	/* Convert the latitude and longitude to cartesian coordinates.
	   The csprm structure has a pointer to a function which will
	   do this for me.  I don't have to know anything about exactly
	   what's going on. */

	rtn_val = (*csprm->ll2cs)(&csprm->proj_prms,xy,ll);

	/* Add some stuff here to do the real Z. */
	xy [2] = ll [2];

	/* Conversions can produce incredibly small numbers which are
	   undesirable (i.e. 1.234E-09).  Usually the user wants a
	   hard zero in this case. */
	if (fabs (xy [XX]) < csprm->csdef.zero [XX]) xy [XX] = cs_Zero;
	if (fabs (xy [YY]) < csprm->csdef.zero [YY]) xy [YY] = cs_Zero;

	return (rtn_val);
}

int EXP_LVL3 CS_ll3cs (Const struct cs_Csprm_ *csprm,double xy [3],Const double ll [3])
{
	extern double cs_Zero;			/* 0.0 */

	int rtn_val;

	rtn_val = (*csprm->ll2cs)(&csprm->proj_prms,xy,ll);

	/* Convert Z from meters to the real unit.  Until we have a means of
	   defining the unit of the Z coordinate, we skip this for geographic
	   coordinate systems. */
	if ((csprm->prj_flags & cs_PRJFLG_GEOGR) == 0)
	{
		xy [ZZ] = ll [HGT] / csprm->csdef.unit_scl;
	}
	else
	{
		xy [ZZ] = ll [HGT];
	}

	/* Conversions can produce incredibly small numbers which are
	   undesirable (i.e. 1.234E-09).  Usually the user wants a
	   hard zero in this case. */
	if (fabs (xy [XX]) < csprm->csdef.zero [XX]) xy [XX] = cs_Zero;
	if (fabs (xy [YY]) < csprm->csdef.zero [YY]) xy [YY] = cs_Zero;

	return (rtn_val);
}

/**********************************************************************
**	alpha = CS_cscnv (csprm,ll);
**
**	struct cs_Csprm_ *csprm;	coordinate system definition structure as
**								returned by CS_csloc.
**	double ll [3];				the longitude ([0]), latitude ([1]), and the
**								height ([2]) of the point at which the
**								convergence is to be computed.
**	double alpha;				the computed convergence angle at the specified
**								point in degrees east of north.
**
**	One of the distortions introduced by a projection is that
**	azimuthal distortion.  The measure of this distortion at
**	any given point on a map is called the convergence angle.
**	this is the angle from grid north (usually the Y axis)
**	and tru north.  This angle is returned, in degrees east
**	of north.  Typically, a point on the eastern side of
**	the central meridian will produce a negative convergence
**	angle.  This, of course, depends on the projection and
**	coordinate system in use.
**
**	Notice, if the cs_cnvrg pointer in the coordinate system
**	definition structure is NULL, there is no analytical function
**	for computing the convergence and a genric computation
**	is performed.
**********************************************************************/

double EXP_LVL3 CS_cscnv (Const struct cs_Csprm_ *csprm,Const double ll [3])

{
	double alpha;

	alpha = (*csprm->cs_cnvrg)(&csprm->proj_prms,ll);

	return (alpha);
}

/**********************************************************************
**	k = CS_csscl (csprm,ll);
**
**	struct cs_Csprm_ *csprm;	coordinate system definition structure as
**								returned by CS_csloc.
**	double ll [3];				the longitude ([0]), latitude ([1]), and
**								height ([2]) of the point at which the
**								scale is to be computed.
**	double k;					the computed true scale at the specified point.
**
**	Since every map has a certain amount of distortion, the
**	true scale of the map will not be the desired scale
**	at every spot on the map.  This function will compute
**	the difference between true scale and grid scale at the
**	indicated point returning the difference as yet another
**	scale factor, i.e. a number close to one.  Comparing this
**	value to 1.0 will provide a reasonable means of determining
**	one aspect of projection distortion at any given point
**	on the map.
**
**	For non-conformal projections, there are two scale factors,
**	h and k.  The k scale factor indicates the scale distorion
**	of the projection as measured along parallels, the h scale
**	factor measures the scale distortion of the projection
**	along meridians.  In a conformal projection, h and k are
**	essentially equal.  In equal area projections, h = 1/k.
**	These facts staem from the nature of the projection.
**	
**	In azimuthal projections, the h scale factor is a measure
**	of scale distortion along the radial lines from the origin
**	of the projection and the k scale factor is a measure of
**	distortion along lines normal to these radial lines.  In
**	the equi-distant azimuthal, h = 1.0.
**
**	This function will always return the most meaningful of the
**	two values depending upon the projections.  Usually, the
**	k value is returned.  However, for the American Polyconic,
**	the k value is always one.  Therefore, in the case of the
**	American Polyconic projection, for example, this function
**	will return the h scale factor.  See CS_cssck (3) and
**	CS_cssch (3) for functions which will always return
**	the k and h factors respectively.
**********************************************************************/

double EXP_LVL3 CS_csscl (Const struct cs_Csprm_ *csprm,Const double ll [3])
{
	double kk;

	kk = (*csprm->cs_scale)(&csprm->proj_prms,ll);

	return (kk);
}

/**********************************************************************
**    Calling Sequence:	k = CS_cssck (csprm,ll);
**
**	struct cs_Csprm_ *csprm;	coordinate system definition structure as
**								returned by CS_csloc.
**	double ll [3];				the longitude ([0]), the latitude ([1]),
**								and the height ([2]) of the point at which
**								the true scale is to be computed.
**	double k;					the computed true scale at the specified point.
**
**	Since every map has a certain amount of distortion, the
**	true scale of the map will not be the desired scale
**	at every spot on the map.  This function will compute
**	the difference between true scale and grid scale at the
**	indicated point returning the difference as yet another
**	scale factor, i.e. a number close to one.  Comparing this
**	value to 1.0 will provide a reasonable means of determining
**	one aspect of projection distortion at any given point
**	on the map.
**
**	For non-conformal projections, there are two scale factors,
**	h and k.  The k scale factor indicates the scale distorion
**	of the projection as measured along parallels, the h scale
**	factor measures the scale distortion of the projection
**	along meridians.  In a conformal projection, h and k are
**	essentially equal.  In equal area projections, h = 1/k.
**	These facts staem from the nature of the projection.
**	
**	In azimuthal projections, the h scale factor is a measure
**	of scale distortion along the radial lines from the origin
**	of the projection and the k scale factor is a measure of
**	distortion along lines normal to these radial lines.  In
**	the equi-distant azimuthal, h = 1.0.
**
**	The function always returns the k scale factor of the
**	projection.  For use in surveying, since conformal
**	projections are used for all state plane zones, the
**	returned value is also known as the grid scale factor.
**********************************************************************/

double EXP_LVL3 CS_cssck (Const struct cs_Csprm_ *csprm,Const double ll [3])

{
	double k;

	k = (*csprm->cs_sclk)(&csprm->proj_prms,ll);

	return (k);
}
/**********************************************************************
**	h = CS_cssch (csprm,ll);
**
**	struct cs_Csprm_ *csprm;	coordinate system definition structure as
**								returned by CS_csloc.
**	double ll [3];				the longitude ([0]), latitude ([1]), and
**								height ([2]) of the point at which the
**								true scale is to be computed.
**	double h;					the computed true scale at the specified point.
**
**	Since every map has a certain amount of distortion, the
**	true scale of the map will not be the desired scale
**	at every spot on the map.  This function will compute
**	the difference between true scale and grid scale at the
**	indicated point returning the difference as yet another
**	scale factor, i.e. a number close to one.  Comparing this
**	value to 1.0 will provide a reasonable means of determining
**	one aspect of projection distortion at any given point
**	on the map.
**
**	For non-conformal projections, there are two scale factors,
**	h and k.  The k scale factor indicates the scale distorion
**	of the projection as measured along parallels, the h scale
**	factor measures the scale distortion of the projection
**	along meridians.  In a conformal projection, h and k are
**	essentially equal.  In equal area projections, h = 1/k.
**	These facts staem from the nature of the projection.
**	
**	In azimuthal projections, the h scale factor is a measure
**	of scale distortion along the radial lines from the origin
**	of the projection and the k scale factor is a measure of
**	distortion along lines normal to these radial lines.  In
**	the equi-distant azimuthal, h = 1.0.
**
**	For conformal projections, this function returns the k
**	scale factor at the point supplied.  For non-conformal
**	projections, this function returns the h scale factor.
**********************************************************************/

double EXP_LVL3 CS_cssch (Const struct cs_Csprm_ *csprm,Const double ll [3])
{
	double h;

	h = (*csprm->cs_sclh)(&csprm->proj_prms,ll);

	return (h);
}

/**********************************************************************
**	st = CS_cschk (cs_def,chk_flg,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system definition being checked.
**	unsigned short chk_flg;		a bit map of checking options, see remarks below.
**	int err_list [];			pointer to an array of integers in which the
**								individual error codes are returned.  May be
**								the NULL pointer.
**	int list_sz;				sizeof the integer array pointed to by the
**								err_list argument; may be zero.
**
**	Constants which specify a specific bit in the chk_flg option
**	bit map are defined in cs_map.h and are:
**
**	cs_CSCHK_DATUM	indcates that a datum key name reference is
**			to be checked against the current datum
**			dictionary.
**	cs_CSCHK_ELLIPS	indictaes that an ellipsoid key name reference
**			is to be checked against the current ellipsoid
**			dictionary.
**	cs_CSCHK_REORT	indicates that all errors are to be reported
**			through the use of CS_erpt.
**
**	The datum and ellipsoid checks are optional as the Coordinate
**	system compiler must check these against the dictionaries it
**	is producing which may not be what is considered the currently
**	active dictionaries.
**
**	The list of errors is returned to enable a list box of errors
**	or some such thing to be produced by the calling module.
**
**	If err_list is passed as the NULL pointer, list_sz is
**	automatically forced to zero.  You may also set list_sz to
**	zero.  In either case, the list return feature is disabled
**	and all you get back is the number of errors detected.
**********************************************************************/

int EXP_LVL3 CS_cschk (	Const struct cs_Csdef_ *cs_def,unsigned short chk_flg,
						int err_list [],
						int list_sz)
{
	extern unsigned short cs_ErrSup;	/* Error report suppression
										   bit map */
	extern struct cs_Prjtab_ cs_Prjtab [];	/* Projection Table */
	extern char csErrnam [];		/* Dimensioned at MAXPATH */
	extern short cs_QuadMin;		/* Minimum acceptable value
									   for quad. */
	extern short cs_QuadMax;		/* Maximum acceptable value
									   for quad. */
	extern double cs_One;			/* 1.0 */

	unsigned short err_sup_save;
	
	int ii;
	int ll_flg;
	int err_cnt;

	struct cs_Prjtab_ *pp;
	struct cs_Dtdef_ *dt_def;
	struct cs_Eldef_ *el_def;
	
	double unit_k;

	/* Make sure we don't memory fault. */

	if (err_list == NULL) list_sz = 0;

	/* We will return err_cnt + 1. */

	err_cnt = -1;

	/* Look this projection up in the projection table, get a
	   pointer to the check function, and then check the
	   projection specific values. */

	for (pp = cs_Prjtab;pp->key_nm [0] != '\0';pp += 1)
	{
		if (!CS_stricmp (cs_def->prj_knm,pp->key_nm))
		{
			break;
		}
	}
	if (pp->check == NULL)
	{
		/* We have an invalid projection specification. */

		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_INVPRJ;
	}
	else
	{
		/* Call the projection checker for this projection.  Note,
		   this works only if it is the first possible change to
		   err_cnt.  If something gets added above this which
		   could possibly change err_cnt before getting here,
		   we have a problem. */

		err_cnt += (*pp->check)(cs_def,pp->code,err_list,list_sz);
	}

	/* Check the stuff common to all coordinate systems, regardless
	   of the projection. */

	if (cs_def->dat_knm [0] != '\0')
	{
		/* Check for a valid datum key name here. We use the
		   cs_ErrSup feature to turn off error reporting about
		   an undefined datum, but leave active all the other
		   stuff such as datum dictionary not found. */
		
		if ((chk_flg & cs_CSCHK_DATUM) != 0)
		{
			err_sup_save = cs_ErrSup;
			cs_ErrSup = cs_ERSUP_OPR;
			dt_def = CS_dtdef (cs_def->dat_knm);
			cs_ErrSup = err_sup_save;
			if (dt_def == NULL)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_INVDTM;
			}
			else
			{
				CS_free (dt_def);
				dt_def = NULL;
			}
		}
	}
	else if (cs_def->elp_knm [0] != '\0')
	{
		/* Check for a valid ellipsoid key name here. */

		if ((chk_flg & cs_CSCHK_ELLIPS) != 0)
		{
			err_sup_save = cs_ErrSup;
			cs_ErrSup = cs_ERSUP_OPR;
			el_def = CS_eldef (cs_def->elp_knm);
			cs_ErrSup = err_sup_save;
			if (el_def == NULL)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_INVELP;
			}
			else
			{
				CS_free (el_def);
				el_def = NULL;
			}
		}
	}
	else
	{
		/* Must have one or the other (except for NERTH). */
        if (pp->code != cs_PRJCOD_NERTH && pp->code != cs_PRJCOD_NRTHSRT)
        {
    		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_NOREF;
        }
	}

	/* Check the units; an LL coordinate system must have a valid
	   angular unit. */

	ll_flg = (pp->code == cs_PRJCOD_UNITY);
	if (ll_flg)
	{
		unit_k = CS_unitlu (cs_UTYP_ANG,cs_def->unit);
	}
	else
	{
		unit_k = CS_unitlu (cs_UTYP_LEN,cs_def->unit);
	}
	if (unit_k == 0.0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_UNIT;
	}

	/* Check the map scale factor.  This is rather subjective,
	   but a value less than one might indicate confusion with
	   the scale reduction fcator which is used on some
	   projections. */

	if (!ll_flg && cs_def->map_scl < cs_One)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_MAPSCL;
	}

	/* Check the quad specification. */

	if (cs_def->quad < cs_QuadMin || cs_def->quad > cs_QuadMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_INVQUAD;
	}

	/* Check the geographic useful range. */

	if (cs_def->ll_min [LNG] != 0.0 || cs_def->ll_max [LNG] != 0.0)
	{
		/* Not disabled. */

		if (cs_def->ll_min [LNG] >= cs_def->ll_max [LNG] ||
			cs_def->ll_min [LAT] >= cs_def->ll_max [LAT])
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_GEOMM;
		}
	}

	/* Check the cartesian useful range. */

	if (cs_def->xy_min [XX] != 0.0 || cs_def->xy_max [XX] != 0.0)
	{
		/* Not disabled. */

		if (cs_def->xy_min [XX] >= cs_def->xy_max [XX] ||
			cs_def->xy_min [YY] >= cs_def->xy_max [YY])
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_CRTMM;
		}
	}

	/* If so instructed, report all of the errors detected so far. */

	if ((chk_flg & cs_CSCHK_REPORT) != 0)
	{
		CS_stncp (csErrnam,cs_def->key_nm,MAXPATH);
		for (ii = 0;ii <= err_cnt && ii < list_sz;ii++)
		{
			CS_erpt (err_list [ii]);
		}
	}

	/* OK, return err_cnt to the calling module. */
	return (err_cnt + 1);
}

/**********************************************************************
**	status = CS_llchk (csprm,cnt,pnts);
**
**	struct cs_Csprm_ *csprm;	the coordinate system.
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			the points to be checked.
**	int status;					results of the test are returned.
**
**	This is a generic function, therefore there is no automatic
**	adjustment of latitudes and longitudes.  That is, if the
**	calling module passes -187 degrees longitude, that is what is
**	checked.  The individual projection routines may adjust the
**	longitude before checking, but we do not do this here.
**
**	The returned value is a bit map constructed from the following
**	constants defined in cs_map.h.  A zero value (cs_CNVRT_OK)
**	indicates that the point, or line segment, was completely
**	within the mathematical domain and useful range of the
**	coordinate system.
**
**	cs_CNVRT_OK	is returned if all of the points, the entirity
**			of each line segment, and the entirity of the
**			region, is within the mathematical domain of
**			the coordinate system, and within the useful
**			range of the coordinate system.
**	cs_CNVRT_USFL	this bit (originally 1, check CS_MAP.h to be sure)
**			is set if a point, or a portion of a line segment,
**			or a portion of the region, is outside of the
**			useful range of the coordinate system.
**	cs_CNVRT_DOMN	this bit (originally 2, check CS_MAP.h to be sure)
**			is set if a point, or a portion of a line segment,
**			or a portion of the region, is outside of the
**			mathematical domain of the coordinate system.
**
**	To check points, without the corresponding line and region
**	check, you must feed the points to this function one at a
**	time.  To check lines without the corresponding region
**	check, you must feed each segment to this function one at
**	a time.
**
**	When a region is to be checked, the region must be defined by
**	the list of points provided by the pnts argument.  The region
**	must be closed (i.e. last point equals first point) which further
**	implies that the cnt argument must be four or greater.  The
**	region boundary must be simply connected (i.e. no bow ties) and
**	must proceed in the counterclockwise direction.  See CS_rgn??
**	for functions which will help you meet these requirments.
**	The rigorous region requirements are not required by many
**	projections and, therefore, 1) they are not enforced by way of
**	error messages or run-time checking, and 2) what works for one
**	projection may not work for another if the region requirements
**	are not adhered to.
**
**	If the user specifies a set of limits in the coordinate system
**	definition, those values are assumed to be the useful range.
**	If the user does not specify any values, the setup function
**	computes a set of values which represent the useful range.
**	How this is accomplished is projection dependent.
**
**	While this is a 2D operation (currently), the pnts argument
**	must point to an array of 3D points; this is for compatibility
**	with planned future enhancements of CS_MAP.
**********************************************************************/

int EXP_LVL3 CS_llchk (Const struct cs_Csprm_ *csprm,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double del_lng, lat;

	/* We have to use the projection function to determine
	   if the point or line segment is within the mathematical
	   range.  This check always takes precendence over all
	   other checks. */

	if (*csprm->llchk != NULL)
	{
		status = (*csprm->llchk)(&csprm->proj_prms.trmer,cnt,pnts);
		if (status != cs_CNVRT_OK) return (status);
	}

	/* We now check against the the useful range of the coordinate
	   system.  These may be the values computed by the setup
	   function or specified by the user.

	   As of the current time, we are assuming that the useful
	   limits, in term of latitude and longitude, of a projection
	   can be defined by a simple min/max rectanlge. Further, we
	   assume that if all of the provided points are inside
	   the min/max rectangle, that the entire region is inside
	   the rectangle. We may encounter a weird situation in the
	   future for which these assumtions prove to be invalid.

	   With an exception or two, every projection has a central
	   meridian which defines the actual range of latitudes and
	   longitudes.  To avoid problems with the -180 degree crack,
	   we need to check angular distances from the central meridian,
	   not the actual lat/long.  Where needed, an actual lat/long
	   check is acheived by setting cent_mer to zero. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat     = pnts [ii][LAT];
		del_lng = pnts [ii][LNG] - csprm->cent_mer;
		if (del_lng < csprm->min_ll [LNG] ||
			del_lng > csprm->max_ll [LNG] ||
			lat     < csprm->min_ll [LAT] ||
			lat     > csprm->max_ll [LAT])
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}

	/* Return the determined status. */

	return (status);
}

/**********************************************************************
**	status = CS_xychk (csprm,cnt,pnts);
**
**	struct cs_Csprm_ *csprm;	the coordinate system.
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			the points to be checked.
**	int status;					results of the test are returned.
**
**	The returned value is a bit map constructed from the following
**	constants defined in cs_map.h.  A zero value (cs_CNVRT_OK)
**	indicates that the point, or line segment, was completely
**	within the mathematical domain and useful range of the
**	coordinate system.
**
**	cs_CNVRT_OK	is returned if all of the points, the entirity
**			of each line segment, and the entirity of the
**			region, is within the mathematical domain of
**			the coordinate system, and within the useful
**			range of the coordinate system.  Originally, the
**			value of this constant was zero, check CS_MAP.h
**			to be sure.
**	cs_CNVRT_USFL	this bit (originally 1, check CS_MAP.h to be sure)
**			is set if a point, or a portion of a line segment,
**			or a portion of the region, is outside of the
**			useful range of the coordinate system.
**	cs_CNVRT_DOMN	this bit (originally 2, check CS_MAP.h to be sure)
**			is set if a point, or a portion of a line segment,
**			or a portion of the region, is outside of the
**			mathematical domain of the coordinate system.
**
**	To check points, without the corresponding line and region
**	check, you must feed the points to this function one at a
**	time.  To check lines without the corresponding region
**	check, you must feed each segment to this function one at
**	a time.
**
**	When a region is to be checked, the region must be defined by
**	the list of points provided by the pnts argument.  The region
**	must be closed (i.e. last point equals first point) which further
**	implies that the cnt argument must be four or greater.  The
**	region boundary must be simply connected (i.e. no bow ties) and
**	must proceed in the counterclockwise direction.  See CS_rgn??
**	for functions which will help you meet these requirments.
**	The rigorous region requirements are not required by many
**	projections and, therefore, 1) they are not enforced by way of
**	error messages or run-time checking, and 2) what works for one
**	projection may not work for another if the region requirements
**	are not adhered to.
**
**	If the user specifies a set of limits in the coordinate system
**	definition, those values are assumed to be the useful range.
**	If the user does not specify any values, the setup function
**	computes a set of values which represent the useful range.
**	How this is accomplished is projection dependent.
**
**	This is a 2D operation. The coordinate arrays are dimensioned
**	at 3 for compatibility for planned enhancements to the CS_MAP
**	product.
**********************************************************************/

int EXP_LVL3 CS_xychk (Const struct cs_Csprm_ *csprm,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double xx, yy;

	/* We have to use the projection function to determine
	   if the point or line segment is within the mathematical
	   range.  This check always takes precendence over all
	   other checks. */

	if (csprm->xychk != NULL)
	{
		status = (*csprm->xychk)(&csprm->proj_prms,cnt,pnts);
		if (status != cs_CNVRT_OK) return (status);
	}

	/* If we are still here, we check against the the useful
	   range of the coordinate system.  This may be the values
	   computed by the setup function or specified by the
	   user.

	   Since the useful range is a simple min/max rectangle by
	   definition, we can test each point for being inside
	   the useful region. If all are inside, then the whole
	   region is inside; and vice versa. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		xx = pnts [ii][XX];
		yy = pnts [ii][YY];

		if (xx < csprm->min_xy [XX] ||
			yy < csprm->min_xy [YY] ||
			xx > csprm->max_xy [XX] ||
			yy > csprm->max_xy [YY])
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}

	/* Return the determined status. */

	return (status);
}

/* Returns TRUE (i.e. non-zero) if the conversion represented by the provided
   cs_Csprm_ structure is reentrant. */
int	EXP_LVL1 CS_isCsPrmReentrant (Const struct cs_Csprm_ *prjConversion)
{
	extern char csErrnam [];				/* Dimensioned at MAXPATH */

	int isReentrant = FALSE;

	if (prjConversion != NULL)
	{
		isReentrant = ((prjConversion->prj_flags & cs_PRJFLG_RNTRNT) != 0);
	}
	else
	{
		CS_stncp (csErrnam,"CS_hpApi:1",MAXPATH);
		CS_erpt (cs_ISER);
	}
	return isReentrant;
}
int	EXP_LVL1 CS_isCsReentrant (Const char *csys)
{
	extern char csErrnam [];				/* Dimensioned at MAXPATH */
	extern struct cs_Prjtab_ cs_Prjtab [];	/* Projection Table */

	int isReentrant;
	struct cs_Prjtab_ *pp;
	struct cs_Csdef_ *csDefPtr;
	
	isReentrant = -1;		/* -1 says error, probably csys is not the name,
							   could also be a bogus projection key name in
							   the definition. */

	if (csys != NULL)
	{
		csDefPtr = CS_csdef (csys);
		if (csDefPtr != NULL)
		{
			for (pp = cs_Prjtab;pp->code != cs_PRJCOD_END;pp += 1)
			{
				if (!CS_stricmp (csDefPtr->prj_knm,pp->key_nm))
				{
					break;
				}
			}
			if (pp->code != cs_PRJCOD_END)
			{
				isReentrant = ((pp->flags & cs_PRJFLG_RNTRNT) != 0);
			}
			else
			{
				CS_erpt (cs_UNKWN_PROJ);
			}
			CS_free (csDefPtr);
		}
		else
		{
			CS_erpt (cs_CS_NOT_FND);
		}
	}
	else
	{
		CS_stncp (csErrnam,"CS_hpApi:2",MAXPATH);
		CS_erpt (cs_ISER);
	}
	return isReentrant;
}
