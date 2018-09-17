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

/*lint -esym(613,err_list) possible use of a null pointer */

int EXP_LVL9 CSbursaQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	extern double cs_DelMax;
	extern double cs_RotMax;
	extern double cs_SclMax;

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
	if (fabs (gxDef->parameters.geocentricParameters.rotateX) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEX;
	}
	if (fabs (gxDef->parameters.geocentricParameters.rotateY) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEY;
	}
	if (fabs (gxDef->parameters.geocentricParameters.rotateZ) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEZ;
	}
	if (fabs (gxDef->parameters.geocentricParameters.scale) > cs_SclMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_BWSCALE;
	}

	/* That's it for Bursa WOlf (Position Vector Variety). */
	return (err_cnt + 1);
}
/******************************************************************************
	What we call the Bursa/Wolfe tranformation is an approximation of the
	Seven Parameter transformation.  The following approximations apply:

	1> The cosine of all rotation angles is one.
	2> The sine of all rotation angles is equal to the angle itself.
	3> The product of two sine terms is zero.
*/
int EXP_LVL9 CSbursaS (struct cs_GxXform_* gxXfrm)
{
	extern double cs_One;				/* 1.0 */
	extern double cs_Sec2Rad;			/* 4.848...E-06 */

	double trgScale;
	struct csBursa_ *bursa;

	bursa = NULL;
	bursa = &gxXfrm->xforms.bursa;
	
	bursa->srcERad = gxXfrm->srcDatum.e_rad;
	bursa->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	bursa->trgERad = gxXfrm->trgDatum.e_rad;
	bursa->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	bursa->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	bursa->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	bursa->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;
	bursa->rotX   = gxXfrm->gxDef.parameters.geocentricParameters.rotateX * cs_Sec2Rad;
	bursa->rotY   = gxXfrm->gxDef.parameters.geocentricParameters.rotateY * cs_Sec2Rad;
	bursa->rotZ   = gxXfrm->gxDef.parameters.geocentricParameters.rotateZ * cs_Sec2Rad;

	trgScale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	if (fabs (trgScale) < 1.0E-06)
	{
		// An actual scale factor of zero would produce a floating point
		// exception.  Can't allow that.
		CS_erpt (cs_ISER);
	}
	else
	{
		bursa->scale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	}

	bursa->errorValue    = gxXfrm->errorValue;
	bursa->cnvrgValue    = gxXfrm->cnvrgValue;
	bursa->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSbursaF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSbursaF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSbursaI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSbursaI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSbursaL;
	gxXfrm->release = (cs_RELEASE_CAST)CSbursaR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSbursaD;

	return 0;
}
int EXP_LVL9 CSbursaF3 (struct csBursa_ *bursa,double trgLl [3],Const double srcLl [3])
{
	int status;

	double xx, yy, zz;
	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,bursa->srcERad,bursa->srcESqr);

	/* Adjust these cartesian coordinates via the Bursa/Wolf transformation.
	   First, we apply the rotation matrix in a computationally effieicent
	   form.  That is, several elements of the matrix are assumed to have
	   fixed values. */
	xx =  xyz [XX] - bursa->rotZ * xyz [YY] + bursa->rotY * xyz [ZZ];
	yy =  bursa->rotZ * xyz [XX] + xyz [YY] - bursa->rotX * xyz [ZZ];
	zz = -bursa->rotY * xyz [XX] + bursa->rotX * xyz [YY] + xyz [ZZ];

	/* Apply the scale and translation. */
	xyz [XX] = bursa->scale * xx + bursa->deltaX;
	xyz [YY] = bursa->scale * yy + bursa->deltaY;
	xyz [ZZ] = bursa->scale * zz + bursa->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */
	status = CS_xyzToLlh (trgLl,xyz,bursa->trgERad,bursa->trgESqr);

	/* That's that. */
	return status;
}
int EXP_LVL9 CSbursaF2 (struct csBursa_ *bursa,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSbursaF3 (bursa,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSbursaI3 (struct csBursa_ *bursa,double* trgLl,Const double* srcLl)
{
	int status;

	double xx, yy, zz;
	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,bursa->trgERad,bursa->trgESqr);

	/* Invert the scaling and translation. */
	xx = (xyz [XX] - bursa->deltaX) / bursa->scale;
	yy = (xyz [YY] - bursa->deltaY) / bursa->scale;
	zz = (xyz [ZZ] - bursa->deltaZ) / bursa->scale;

	/* Apply the inverse of the rotation matrix.  Essentially, this code
	   is equivalent to the multiplication by the transpose of the original
	   rotation matrix.
	   
	   Note that this inverse is only an approximation as is the
	   forward.  A pure rotation matrix is orthogonal and its
	   transpose is equal to its inverse.  However, due to the
	   approximation used in the Bursa Wolf transformation, the
	   matrix ix not perfectly orthogonal, and thus the transpose
	   is not the perfect inverse. */
	xyz [XX] =  xx + bursa->rotZ * yy - bursa->rotY * zz;
	xyz [YY] = -bursa->rotZ * xx + yy + bursa->rotX * zz;
	xyz [ZZ] =  bursa->rotY * xx - bursa->rotX * yy + zz;

	/* Convert the new X, Y, and Z back to latitude and longitude. */
	status = CS_xyzToLlh (trgLl,xyz,bursa->srcERad,bursa->srcESqr);

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
int EXP_LVL9 CSbursaI2 (struct csBursa_ *bursa,double* trgLl,Const double* srcLl)
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
	epsilon [0] = epsilon [1] = bursa->cnvrgValue;		/* keep gcc compiler happy */

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
	for (ii = 0;ii < bursa->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSbursaF2 (bursa,newLl,guess);
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
		if (fabs (epsilon [LNG]) > bursa->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > bursa->cnvrgValue)
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
	if (ii >= bursa->maxIterations )
	{
		CS_erpt (cs_BW_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > bursa->errorValue ||
		    fabs (epsilon [LAT]) > bursa->errorValue)
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
int EXP_LVL9 CSbursaL (struct csBursa_ *bursa,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSbursaN (struct csBursa_ *bursa)
{
	int isNull;
	
	isNull = (fabs (bursa->deltaX) < 1.0E-03) &&
			 (fabs (bursa->deltaY) < 1.0E-03) &&
			 (fabs (bursa->deltaZ) < 1.0E-03) &&
			 (fabs (bursa->rotX)   < 1.0E-06) &&
			 (fabs (bursa->rotY)   < 1.0E-06) &&
			 (fabs (bursa->rotZ)   < 1.0E-06) &&
			 (fabs (bursa->scale)  < 1.0E-08);
	return isNull;
}
int EXP_LVL9 CSbursaR (struct csBursa_ *bursa)
{
	return 0;
}
int EXP_LVL9 CSbursaD (struct csBursa_ *bursa)
{
	return 0;
}
