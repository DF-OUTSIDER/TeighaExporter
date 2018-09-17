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

/*		       * * * * R E M A R K S * * * *
*/

#include "cs_map.h"

/*lint -esym(613,err_list)  possible use of null pointer; but not really */

static short csMaxIterations = 8;
static double csCnvrgValue = 1.0E-09;
static double csErrorValue = 1.0E-06;

int EXP_LVL9 CSmolodQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	extern double cs_DelMax;

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csMolod_ */
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

	/* That's it for Molodensky. */
	return (err_cnt + 1);
}

int EXP_LVL9 CSmolodS (struct cs_GxXform_* gxXfrm)
{
	struct csMolod_ *molod;

	molod = &gxXfrm->xforms.molod;
	
	molod->srcERad = gxXfrm->srcDatum.e_rad;
	molod->srcPRad = gxXfrm->srcDatum.p_rad;
	molod->srcFlat = gxXfrm->srcDatum.flat;
	molod->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	molod->trgERad = gxXfrm->trgDatum.e_rad;
	molod->trgPRad = gxXfrm->trgDatum.p_rad;
	molod->trgFlat = gxXfrm->trgDatum.flat;
	molod->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	molod->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	molod->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	molod->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;

	molod->errorValue    = gxXfrm->errorValue;
	molod->cnvrgValue    = gxXfrm->cnvrgValue;
	molod->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSmolodF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSmolodF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSmolodI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSmolodI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSmolodL;
	gxXfrm->release = (cs_RELEASE_CAST)CSmolodR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSmolodD;

	return 0;
}
int EXP_LVL9 CSmolodSf (struct csMolod_* molod,Const struct cs_Datum_ *srcDatum,
												Const struct cs_Datum_* trgDatum)
{
	molod->srcERad = srcDatum->e_rad;
	molod->srcPRad = srcDatum->p_rad;
	molod->srcESqr = srcDatum->ecent * srcDatum->ecent;
	molod->srcFlat = srcDatum->flat;

	molod->trgERad = trgDatum->e_rad;
	molod->trgPRad = trgDatum->p_rad;
	molod->trgESqr = trgDatum->ecent * trgDatum->ecent;
	molod->trgFlat = trgDatum->flat;

	molod->deltaX = srcDatum->delta_X - trgDatum->delta_X;
	molod->deltaY = srcDatum->delta_Y - trgDatum->delta_Y;
	molod->deltaZ = srcDatum->delta_Z - trgDatum->delta_Z;

	molod->errorValue    = csErrorValue;
	molod->cnvrgValue    = csCnvrgValue;
	molod->maxIterations = csMaxIterations;

	return 0;
}
/**********************************************************************
*/
int EXP_LVL9 CSmolodF3 (struct csMolod_ *molod,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Degree;			/* 0.017... */
	extern double cs_Sin1Sec;			/* sine of 1 second of arc. */
	extern double cs_Sec2Deg;			/* 3600.00 */
	extern double cs_NPTest;			/* 89.99999 in radians. */

	double lng, lat;
	double sinLng, cosLng;
	double sinLat, cosLat;
	double sinCosLat;
	double sinSqLat;

	double Rn;
	double Rm;
	double temp1;
	double sqrtTemp1;
	double term1, term2, term3;
	double deltaLngSec, deltaLatSec, deltaHgt;

	lng = srcLl [LNG] * cs_Degree;
	lat = srcLl [LAT] * cs_Degree;

	sinLng = sin (lng);
	sinLat = sin (lat);
	cosLng = cos (lng);
	cosLat = cos (lat);
	sinSqLat = sinLat * sinLat;
	sinCosLat = sinLat * cosLat;

	temp1 = cs_One - (molod->srcESqr * sinSqLat);
	sqrtTemp1 = sqrt (temp1);

	/* Compute ellipsoid radii of curvature of the ellipsoid upon which
	   the source datum is based. */
	Rn = molod->srcERad / sqrtTemp1;
	Rm = molod->srcERad * (cs_One - molod->srcESqr) / (sqrtTemp1 * temp1);

	/* Compute delta latitude in seconds of arc. */
	term1 = molod->deltaZ * cosLat -
			molod->deltaX * sinLat * cosLng -
			molod->deltaY * sinLat * sinLng;
	term2 = (molod->trgERad - molod->srcERad) * molod->srcESqr * Rn * sinCosLat / molod->srcERad;
	temp1 = (Rm * molod->srcERad / molod->srcPRad) + Rn * molod->srcPRad / molod->srcERad;
	term3 = (molod->trgFlat - molod->srcFlat) * temp1 * sinCosLat;
	deltaLatSec = (term1 + term2 + term3) / ((Rm + srcLl [HGT]) * cs_Sin1Sec);

	/* Compute delta longitude in seconds of arc.  At the poles,
	   there is no delta longitude.  Must deal with this to prevent
	   division by zero. */
	if (fabs (lat) > cs_NPTest)
	{
		deltaLngSec = cs_Zero;
	}
	else
	{
		temp1 = molod->deltaY * cosLng - molod->deltaX * sinLng;
		deltaLngSec = temp1 / ((Rn + srcLl [HGT]) * cosLat * cs_Sin1Sec);
	}

	/* Compute delta height. */
	term1 = molod->deltaX * cosLat * cosLng +
			molod->deltaY * cosLat * sinLng +
			molod->deltaZ * sinLat;
	term2 = (molod->trgERad - molod->srcERad) * molod->srcERad / Rn;
	term3 = (molod->trgFlat - molod->srcFlat) * (molod->srcPRad / molod->srcERad) * Rn * sinSqLat;
	deltaHgt = term1 - term2 + term3;

	/* Compute the final results. */
	trgLl [LNG] = srcLl [LNG] + (deltaLngSec * cs_Sec2Deg);
	trgLl [LAT] = srcLl [LAT] + (deltaLatSec * cs_Sec2Deg);
	trgLl [HGT] = srcLl [HGT] + deltaHgt;

	/* This is always successul. */
	return 0;
}

int EXP_LVL7 CSmolodF2 (struct csMolod_* molod,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSmolodF3 (molod,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}

/**********************************************************************
*/
int EXP_LVL7 CSmolodI3 (struct csMolod_ *molod,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Degree;			/* 0.017... */
	extern double cs_Sin1Sec;			/* sine of 1 second of arc. */
	extern double cs_Sec2Deg;			/* 3600.00 */
	extern double cs_NPTest;			/* 89.99999 in radians. */

	double lng, lat;
	double sinLng, cosLng;
	double sinLat, cosLat;
	double sinCosLat;
	double sinSqLat;

	double Rn;
	double Rm;
	double temp1;
	double sqrtTemp1;
	double term1, term2, term3;
	double deltaLngSec, deltaLatSec, deltaHgt;

	lng = srcLl [LNG] * cs_Degree;
	lat = srcLl [LAT] * cs_Degree;

	sinLng = sin (lng);
	sinLat = sin (lat);
	cosLng = cos (lng);
	cosLat = cos (lat);
	sinSqLat = sinLat * sinLat;
	sinCosLat = sinLat * cosLat;

	temp1 = cs_One - (molod->trgESqr * sinSqLat);
	sqrtTemp1 = sqrt (temp1);

	/* Compute ellipsoid radii of curvature of the ellipsoid upon which
	   the source datum is based. */
	Rn = molod->trgERad / sqrtTemp1;
	Rm = molod->trgERad * (cs_One - molod->trgESqr) / (sqrtTemp1 * temp1);

	/* Compute delta latitude in seconds of arc. */
	term1 = molod->deltaX * sinLat * cosLng +
			molod->deltaY * sinLat * sinLng -
			molod->deltaZ * cosLat;
	term2 = (molod->srcERad - molod->trgERad) * molod->trgESqr * Rn * sinCosLat / molod->trgERad;
	temp1 = (Rm * molod->trgERad / molod->trgPRad) + Rn * molod->trgPRad / molod->trgERad;
	term3 = (molod->srcFlat - molod->trgFlat) * temp1 * sinCosLat;
	deltaLatSec = (term1 + term2 + term3) / ((Rm + srcLl [HGT]) * cs_Sin1Sec);

	/* Compute delta longitude in seconds of arc.  At the poles,
	   there is no delta longitude.  Must deal with this to prevent
	   division by zero. */
	if (fabs (lat) > cs_NPTest)
	{
		deltaLngSec = cs_Zero;
	}
	else
	{
		temp1 = molod->deltaX * sinLng - molod->deltaY * cosLng;
		deltaLngSec = temp1 / ((Rn + srcLl [HGT]) * cosLat * cs_Sin1Sec);
	}

	/* Compute delta height. */
	term1 = molod->deltaX * cosLat * cosLng +
			molod->deltaY * cosLat * sinLng +
			molod->deltaZ * sinLat;
	term2 = (molod->srcERad - molod->trgERad) * molod->trgERad / Rn;
	term3 = (molod->srcFlat - molod->trgFlat) * (molod->trgPRad / molod->trgERad) * Rn * sinSqLat;
	deltaHgt = term3 - term2 - term1;

	/* Compute the final results. */
	trgLl [LNG] = srcLl [LNG] + (deltaLngSec * cs_Sec2Deg);
	trgLl [LAT] = srcLl [LAT] + (deltaLatSec * cs_Sec2Deg);
	trgLl [HGT] = srcLl [HGT] + deltaHgt;

	/* This is always successul. */
	return 0;
}

/**********************************************************************
*/
int EXP_LVL7 CSmolodI2 (struct csMolod_* molod,double* trgLl,Const double* srcLl)
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
	epsilon [0] = epsilon [1] = molod->cnvrgValue;		/* keep gcc compiler happy */

	/* First, we copy the source lat/longs to the local array.
	   This is the default result which the user may want in
	   the event of an error.  Note, we assume such has been
	   done below, even if there has not been an error. */
	trgLl [LNG] = guess [LNG] = srcLl [LNG];
	trgLl [LAT] = guess [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	/* To improve performancce, we could use CS_mo3dInvrs here to
	   get our guess.  Doing so could reduce the number of iterations
	   required below substantially. */

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < molod->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSmolodF2 (molod,newLl,guess);
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
		if (fabs (epsilon [LNG]) > molod->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > molod->cnvrgValue)
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
	if (ii >= molod->maxIterations )
	{
		CS_erpt (cs_MO_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > molod->errorValue ||
		    fabs (epsilon [LAT]) > molod->errorValue)
		{
			/* This is not supposed to happen.  It will not happen if the
			   input coordinates are anywhere reasonable.  However, since the
			   contract is that once created, a transformation always returns
			   a rational result, with perhaps a warning status, we return a
			   +1 here.  If you prefer a fatal, change this value to -1. */
			rtnVal = 1;
		}
	}

	/* Adjust the ll_lcl value to the computed value, now that we
	   know that it should be correct. */
	if (rtnVal >= 0)
	{
		trgLl [LNG] = guess [LNG];
		trgLl [LAT] = guess [LAT];
	}
	return rtnVal;
}
int EXP_LVL9 CSmolodL (struct csMolod_ *molod,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSmolodN (struct csMolod_ *molod)
{
	int isNull;
	
	isNull = (fabs (molod->deltaX) < 1.0e-03) &&
			 (fabs (molod->deltaY) < 1.0e-03) &&
			 (fabs (molod->deltaZ) < 1.0e-03);
	return isNull;
}
int EXP_LVL9 CSmolodR (struct csMolod_ *molod)
{
	return 0;
}
int EXP_LVL9 CSmolodD (struct csMolod_ *molod)
{
	return 0;
}
