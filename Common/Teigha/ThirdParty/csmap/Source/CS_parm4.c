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

/*lint -esym(613,err_list)  possible use of null pointer, but not really */

int EXP_LVL9 CSparm4Q (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	extern double cs_DelMax;
	extern double cs_SclMax;

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csParm4_ */
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
	if (fabs (gxDef->parameters.geocentricParameters.scale) > cs_SclMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_BWSCALE;
	}

	/* That's it for Four Paramater. */
	return (err_cnt + 1);
}
/******************************************************************************
*/
int EXP_LVL9 CSparm4S (struct cs_GxXform_* gxXfrm)
{
	extern double cs_One;				/* 1.0 */

	double trgScale;
	struct csParm4_ *parm4;

	parm4 = &gxXfrm->xforms.parm4;
	
	parm4->srcERad = gxXfrm->srcDatum.e_rad;
	parm4->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	parm4->trgERad = gxXfrm->trgDatum.e_rad;
	parm4->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	parm4->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	parm4->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	parm4->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;

	trgScale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	if (fabs (trgScale) < 1.0E-06)
	{
		// An actual scale factor of zero would produce a floating point
		// exception.  Can't allow that.
		CS_erpt (cs_ISER);
	}
	else
	{
		parm4->scale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	}

	parm4->errorValue    = gxXfrm->errorValue;
	parm4->cnvrgValue    = gxXfrm->cnvrgValue;
	parm4->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSparm4F2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSparm4F3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSparm4I2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSparm4I3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSparm4L;
	gxXfrm->release = (cs_RELEASE_CAST)CSparm4R;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSparm4D;

	return 0;
}
int EXP_LVL9 CSparm4F3 (struct csParm4_ *parm4,double trgLl [3],Const double srcLl [3])
{
	int status;

	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,parm4->srcERad,parm4->srcESqr);

	/* Adjust these cartesian coordinates via the Bursa/Wolf transformation. */
	xyz [XX] = xyz [XX] * parm4->scale + parm4->deltaX;
	xyz [YY] = xyz [YY] * parm4->scale + parm4->deltaY;
	xyz [ZZ] = xyz [ZZ] * parm4->scale + parm4->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */
	status = CS_xyzToLlh (trgLl,xyz,parm4->trgERad,parm4->trgESqr);

	/* That's that. */
	return status;
}
int EXP_LVL9 CSparm4F2 (struct csParm4_ *parm4,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSparm4F3 (parm4,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSparm4I3 (struct csParm4_ *parm4,double* trgLl,Const double* srcLl)
{
	int status;

	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,parm4->trgERad,parm4->trgESqr);

	/* Apply the translation parameters. */
	xyz [XX] = (xyz [XX] - parm4->deltaX) / parm4->scale;
	xyz [YY] = (xyz [YY] - parm4->deltaY) / parm4->scale;
	xyz [ZZ] = (xyz [ZZ] - parm4->deltaZ) / parm4->scale;

	/* Convert the new X, Y, and Z back to latitude and longitude. */
	status = CS_xyzToLlh (trgLl,xyz,parm4->srcERad,parm4->srcESqr);

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
int EXP_LVL9 CSparm4I2 (struct csParm4_ *parm4,double* trgLl,Const double* srcLl)
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
	epsilon [0] = epsilon [1] = parm4->cnvrgValue;		/* keep gcc compiler happy */

	/* First, we copy the WGS-84 lat/longs to the local array.  This is the
	   default result which the user may want in the event of a fatal error.
	   Note, we assume such has been done below, even if there has not been
	   an error. */
	trgLl [LNG] = guess [LNG] = srcLl [LNG];
	trgLl [LAT] = guess [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	/* To improve performancce, we could use CS_bw3dInvrs here to
	   get our guess.  That function will probably reduce the
	   number of iterations require below substantially. */

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < parm4->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSparm4F2 (parm4,newLl,guess);
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
		if (fabs (epsilon [LNG]) > parm4->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > parm4->cnvrgValue)
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
	if (ii >= parm4->maxIterations )
	{
		CS_erpt (cs_4P_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > parm4->errorValue ||
		    fabs (epsilon [LAT]) > parm4->errorValue)
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
int EXP_LVL9 CSparm4L (struct csParm4_ *parm4,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSparm4N (struct csParm4_ *parm4)
{
	int isNull;
	
	isNull = (fabs (parm4->deltaX) < 1.0E-03) &&
			 (fabs (parm4->deltaY) < 1.0E-03) &&
			 (fabs (parm4->deltaZ) < 1.0E-03) &&
			 (fabs (parm4->scale)  < 1.0E-08);
	return isNull;
}
int EXP_LVL9 CSparm4R (struct csParm4_ *parm4)
{
	return 0;
}
int EXP_LVL9 CSparm4D (struct csParm4_ *parm4)
{
	return 0;
}
