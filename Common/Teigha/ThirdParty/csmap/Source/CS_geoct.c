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

/*lint -esym(613,err_list)  possible use of null pointer; but not really */

int EXP_LVL9 CSgeoctQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	extern double cs_DelMax;

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csBursa_ */
	if (fabs (gxDef->parameters.geocentricParameters.deltaX) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAX;
	}
	if (fabs (gxDef->parameters.geocentricParameters.deltaY) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAY;
	}
	if (fabs (gxDef->parameters.geocentricParameters.deltaZ) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAZ;
	}

	/* That's it for Geocentric Translation. */
	return (err_cnt + 1);
}
/******************************************************************************
*/
int EXP_LVL9 CSgeoctS (struct cs_GxXform_* gxXfrm)
{
	struct csGeoct_ *geoct;

	geoct = NULL;
	geoct = &gxXfrm->xforms.geoct;
	
	geoct->srcERad = gxXfrm->srcDatum.e_rad;
	geoct->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	geoct->trgERad = gxXfrm->trgDatum.e_rad;
	geoct->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	geoct->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	geoct->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	geoct->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;

	geoct->errorValue    = gxXfrm->errorValue;
	geoct->cnvrgValue    = gxXfrm->cnvrgValue;
	geoct->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSgeoctF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSgeoctF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSgeoctI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSgeoctI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSgeoctL;
	gxXfrm->release = (cs_RELEASE_CAST)CSgeoctR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSgeoctD;

	return 0;
}
int EXP_LVL9 CSgeoctF3 (struct csGeoct_ *geoct,double trgLl [3],Const double srcLl [3])
{
	int status;

	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,geoct->srcERad,geoct->srcESqr);

	/* Apply the scale and translation. */
	xyz [XX] += geoct->deltaX;
	xyz [YY] += geoct->deltaY;
	xyz [ZZ] += geoct->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */
	status = CS_xyzToLlh (trgLl,xyz,geoct->trgERad,geoct->trgESqr);

	/* That's that. */
	return status;
}
int EXP_LVL9 CSgeoctF2 (struct csGeoct_ *geoct,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSgeoctF3 (geoct,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSgeoctI3 (struct csGeoct_ *geoct,double* trgLl,Const double* srcLl)
{
	int status;

	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,geoct->trgERad,geoct->trgESqr);

	/* Invert the translation. */
	xyz [XX] -= geoct->deltaX;
	xyz [YY] -= geoct->deltaY;
	xyz [ZZ] -= geoct->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude. */
	status = CS_xyzToLlh (trgLl,xyz,geoct->srcERad,geoct->srcESqr);

	return status;
}

/******************************************************************************
	This is an iterative exercise of CS_bw2dFowrd.  Thus, this calculation
	will produce a rather precise inverse of geographic coordinates.  Used in
	the two dimensional case as we don't know what the ellipsoid elevation is.
	Thus, we can't precisely compute the inverse of a coordinate we might
	have computed the forward for, and the using application threw away the
	Z.  The end result of all this can be positional creep unless we do
	as coded below.
*/
int EXP_LVL9 CSgeoctI2 (struct csGeoct_ *geoct,double* trgLl,Const double* srcLl)
{
	int ii;
	int lngOk;
	int latOk;
	int rtnVal;

	double guess [3];
	double newLl [3];
	double epsilon [3];

	/* Assume everything goes OK until we know different. */
	rtnVal = 0;
	epsilon [0] = epsilon [1] = geoct->cnvrgValue;		/* keep gcc compiler happy */

	/* First, we copy the WGS-84 lat/longs to the local array.  This is the
	   default result which the user may want in the event of a fatal error.
	   Note, we assume such has been done below, even if there has not been
	   an error. */
	trgLl [LNG] = guess [LNG] = srcLl [LNG];
	trgLl [LAT] = guess [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	/* To improve performancce, we could use CSgeoctI3 here to get our
	   first guess.  That function will probably reduce the number of
	   iterations require below substantially. */

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < geoct->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSgeoctF2 (geoct,newLl,guess);
		if (rtnVal != 0)
		{
			/* Oopps!! We must have been given some pretty strange
			   coordinate values. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = srcLl [LNG] - newLl [LNG];
		epsilon [LAT] = srcLl [LAT] - newLl [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > geoct->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > geoct->cnvrgValue)
		{
			latOk = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newLl that is within
		   samllValue of srcLl, we are done. */
		if (lngOk && latOk) break;
	}

	/* If we didn't resolve in maxIteration tries, we issue a warning
	   message.  Usually, three or four iterations does the trick. */
	if (ii >= geoct->maxIterations )
	{
		CS_erpt (cs_GEOCT_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > geoct->errorValue ||
		    fabs (epsilon [LAT]) > geoct->errorValue)
		{
			/* This is not supposed to happen.  It will not happen if the
			   input coordinates are anywhere reasonable.  However, since the
			   contract is that once created, a transformation always returns
			   a rational result, with perhaps a warning status, we return a
			   +1 here.  If you prefer a fatal, change this value to -1. */
			rtnVal = 1;
		}
	}

	/* If we are not returning fatal error status, we return the last
	   computed value even if we are returning a warning status.  It may be
	   of value, often is. */
	if (rtnVal >= 0)
	{
		trgLl [LNG] = guess [LNG];
		trgLl [LAT] = guess [LAT];
	}
	return rtnVal;
}
int EXP_LVL9 CSgeoctL (struct csGeoct_ *geoct,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSgeoctN (struct csGeoct_ *geoct)
{
	int isNull;
	
	isNull = (fabs (geoct->deltaX) < 1.0E-03) &&
			 (fabs (geoct->deltaY) < 1.0E-03) &&
			 (fabs (geoct->deltaZ) < 1.0E-03);
	return isNull;
}
int EXP_LVL9 CSgeoctR (struct csGeoct_ *geoct)
{
	return 0;
}
int EXP_LVL9 CSgeoctD (struct csGeoct_ *geoct)
{
	return 0;
}
