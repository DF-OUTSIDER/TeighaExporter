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

static short csMaxIterations = 20;
static double csCnvrgValue = 1.0E-09;
static double csErrorValue = 1.0E-06;

int EXP_LVL9 CSparm7Q (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	extern double cs_DelMax;
	extern double cs_RotMax;
	extern double cs_SclMax;

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csParm7_ */
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

	/* That's it for the Seven Parameter. */
	return (err_cnt + 1);
}
/******************************************************************************
*/
int EXP_LVL9 CSparm7S (struct cs_GxXform_* gxXfrm)
{
	extern double cs_One;
	extern double cs_Sec2Rad;			/* 4.848136E-06 */

	struct csParm7_ *parm7;

	double trgScale;
	double sinAlpha, cosAlpha;
	double sinBeta, cosBeta;
	double sinGamma, cosGamma;

	parm7 = &gxXfrm->xforms.parm7;

	parm7->srcERad = gxXfrm->srcDatum.e_rad;
	parm7->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	parm7->trgERad = gxXfrm->trgDatum.e_rad;
	parm7->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	parm7->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	parm7->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	parm7->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;

	sinAlpha = sin (gxXfrm->gxDef.parameters.geocentricParameters.rotateX * cs_Sec2Rad);
	cosAlpha = cos (gxXfrm->gxDef.parameters.geocentricParameters.rotateX * cs_Sec2Rad);
	sinBeta  = sin (gxXfrm->gxDef.parameters.geocentricParameters.rotateY * cs_Sec2Rad);
	cosBeta  = cos (gxXfrm->gxDef.parameters.geocentricParameters.rotateY * cs_Sec2Rad);
	sinGamma = sin (gxXfrm->gxDef.parameters.geocentricParameters.rotateZ * cs_Sec2Rad);
	cosGamma = cos (gxXfrm->gxDef.parameters.geocentricParameters.rotateZ * cs_Sec2Rad);

	/* View the following as the development of a 3x3 orthogonal
	   rotation matirx: rt12 = rotate matrix row 1 column 2 */
	parm7->rt11 =  cosBeta * cosGamma;
	parm7->rt21 = -cosBeta * sinGamma;
	parm7->rt31 =  sinBeta;
	parm7->rt12 =  cosAlpha * sinGamma + sinAlpha * sinBeta * cosGamma;
	parm7->rt22 =  cosAlpha * cosGamma - sinAlpha * sinBeta * sinGamma;
	parm7->rt32 = -sinAlpha * cosBeta;
	parm7->rt13 =  sinAlpha * sinGamma - cosAlpha * sinBeta * cosGamma;
	parm7->rt23 =  sinAlpha * cosGamma + cosAlpha * sinBeta * sinGamma;
	parm7->rt33 =  cosAlpha * cosBeta;

	trgScale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	if (fabs (trgScale) < 1.0E-06)
	{
		CS_erpt (cs_ISER);
	}
	else
	{
		parm7->scale = (cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06));
	}

	parm7->errorValue    = gxXfrm->errorValue;
	parm7->cnvrgValue    = gxXfrm->cnvrgValue;
	parm7->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSparm7F2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSparm7F3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSparm7I2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSparm7I3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSparm7L;
	gxXfrm->release = (cs_RELEASE_CAST)CSparm7R;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSparm7D;

	return 0;
}
int EXP_LVL9 CSparm7Sf (struct csParm7_* parm7,Const struct cs_Datum_ *srcDatum,
												Const struct cs_Datum_* trgDatum)
{
	extern double cs_One;
	extern double cs_Sec2Rad;			/* 4.848136E-06 */

	double trgScale;
	double sinAlpha, cosAlpha;
	double sinBeta, cosBeta;
	double sinGamma, cosGamma;

	parm7->srcERad = srcDatum->e_rad;
	parm7->srcESqr = srcDatum->ecent * srcDatum->ecent;

	parm7->trgERad = trgDatum->e_rad;
	parm7->trgESqr = trgDatum->ecent * trgDatum->ecent;

	parm7->deltaX = srcDatum->delta_X - trgDatum->delta_X;
	parm7->deltaY = srcDatum->delta_Y - trgDatum->delta_Y;
	parm7->deltaZ = srcDatum->delta_Z - trgDatum->delta_Z;

	sinAlpha = sin ((srcDatum->rot_X - trgDatum->rot_X) * cs_Sec2Rad);
	cosAlpha = cos ((srcDatum->rot_X - trgDatum->rot_X) * cs_Sec2Rad);
	sinBeta  = sin ((srcDatum->rot_Y - trgDatum->rot_Y) * cs_Sec2Rad);
	cosBeta  = cos ((srcDatum->rot_Y - trgDatum->rot_Y) * cs_Sec2Rad);
	sinGamma = sin ((srcDatum->rot_Z - trgDatum->rot_Z) * cs_Sec2Rad);
	cosGamma = cos ((srcDatum->rot_Z - trgDatum->rot_Z) * cs_Sec2Rad);

	/* view the following as the development of a 3x3 orthogonal
	   rotation matirx: rt12 = rotate matrix row 1 column 2 */
	parm7->rt11 =  cosBeta * cosGamma;
	parm7->rt21 = -cosBeta * sinGamma;
	parm7->rt31 =  sinBeta;
	parm7->rt12 =  cosAlpha * sinGamma + sinAlpha * sinBeta * cosGamma;
	parm7->rt22 =  cosAlpha * cosGamma - sinAlpha * sinBeta * sinGamma;
	parm7->rt32 = -sinAlpha * cosBeta;
	parm7->rt13 =  sinAlpha * sinGamma - cosAlpha * sinBeta * cosGamma;
	parm7->rt23 =  sinAlpha * cosGamma + cosAlpha * sinBeta * sinGamma;
	parm7->rt33 =  cosAlpha * cosBeta;

	trgScale = cs_One + (trgDatum->bwscale * 1.0E-06);
	parm7->scale = (cs_One + (srcDatum->bwscale * 1.0E-06)) / trgScale;

	parm7->errorValue    = csErrorValue;
	parm7->cnvrgValue    = csCnvrgValue;
	parm7->maxIterations = csMaxIterations;

	return 0;
}
int EXP_LVL9 CSparm7F3 (struct csParm7_ *parm7,double* trgLl,Const double* srcLl)
{
	int status;

	double xx, yy, zz;
	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,parm7->srcERad,parm7->srcESqr);

	/* Adjust these cartesian coordinates via the Bursa/Wolf transformation.
	   First we apply the rotation.  This is, essentially, a matrix multiplcation. */
	xx = parm7->rt11 * xyz [XX] + parm7->rt12 * xyz [YY] + parm7->rt13 * xyz [ZZ];
	yy = parm7->rt21 * xyz [XX] + parm7->rt22 * xyz [YY] + parm7->rt23 * xyz [ZZ];
	zz = parm7->rt31 * xyz [XX] + parm7->rt32 * xyz [YY] + parm7->rt33 * xyz [ZZ];

	/* Apply the scale and translation factors. */
	xyz [XX] = parm7->scale * xx + parm7->deltaX;
	xyz [YY] = parm7->scale * yy + parm7->deltaY;
	xyz [ZZ] = parm7->scale * zz + parm7->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */
	status = CS_xyzToLlh (trgLl,xyz,parm7->trgERad,parm7->trgESqr);

	/* That's that. */
	return status;
}
int EXP_LVL9 CSparm7F2 (struct csParm7_ *parm7,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSparm7F3 (parm7,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSparm7I3 (struct csParm7_ *parm7,double* trgLl,Const double* srcLl)
{
	int status;

	double xx, yy, zz;
	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz,srcLl,parm7->trgERad,parm7->trgESqr);

	/* Reverse the scaling and translation. */
	xx = (xyz [XX] - parm7->deltaX) / parm7->scale;
	yy = (xyz [YY] - parm7->deltaY) / parm7->scale;
	zz = (xyz [ZZ] - parm7->deltaZ) / parm7->scale;

	/* Apply the inverse of the rotation matrix by deftly multiply by the
	   transpose of the forward matrix.  This works here as the rotation
	   only matrix is orthogonal. */
	xyz [XX] = parm7->rt11 * xx + parm7->rt21 * yy + parm7->rt31 * zz;
	xyz [YY] = parm7->rt12 * xx + parm7->rt22 * yy + parm7->rt32 * zz;
	xyz [ZZ] = parm7->rt13 * xx + parm7->rt23 * yy + parm7->rt33 * zz;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */
	status = CS_xyzToLlh (trgLl,xyz,parm7->srcERad,parm7->srcESqr);

	/* That's that. */
	return status;
}

/******************************************************************************
*/
int EXP_LVL9 CSparm7I2 (struct csParm7_ *parm7,double* trgLl,Const double* srcLl)
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
	epsilon [0] = epsilon [1] = parm7->cnvrgValue;		/* keep gcc compiler happy */

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
	for (ii = 0;ii < parm7->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSparm7F2 (parm7,newLl,guess);
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
		if (fabs (epsilon [LNG]) > parm7->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > parm7->cnvrgValue)
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
	if (ii >= parm7->maxIterations )
	{
		CS_erpt (cs_7P_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > parm7->errorValue ||
		    fabs (epsilon [LAT]) > parm7->errorValue)
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
int EXP_LVL9 CSparm7L (struct csParm7_ *parm7,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSparm7N (struct csParm7_ *parm7)
{
	extern double cs_One;		/* 1.0 */
	int isNull;
	
	isNull = (fabs (parm7->deltaX) < 1.0E-03) &&
			 (fabs (parm7->deltaY) < 1.0E-03) &&
			 (fabs (parm7->deltaZ) < 1.0E-03) &&

			 (fabs (parm7->scale)  < 1.0E-08) &&

			 (fabs (parm7->rt11 - cs_One) < 1.0E-08) &&
			 (fabs (parm7->rt12)          < 1.0E-08) &&
			 (fabs (parm7->rt13)          < 1.0E-08) &&

			 (fabs (parm7->rt21)          < 1.0E-08) &&
			 (fabs (parm7->rt22 - cs_One) < 1.0E-08) &&
			 (fabs (parm7->rt23)          < 1.0E-08) &&

			 (fabs (parm7->rt31)          < 1.0E-08) &&
			 (fabs (parm7->rt32)          < 1.0E-08) &&
			 (fabs (parm7->rt33 - cs_One) < 1.0E-08);
	return isNull;
}
int EXP_LVL9 CSparm7R (struct csParm7_ *parm7)
{
	return 0;
}
int EXP_LVL9 CSparm7D (struct csParm7_ *parm7)
{
	return 0;
}
