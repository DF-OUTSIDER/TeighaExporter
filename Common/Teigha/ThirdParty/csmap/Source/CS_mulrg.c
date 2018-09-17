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
#include "cs_Legacy.h"

/*lint -e702   Shift right of signed quantity; applied to shorts of values < 1024 */
/*lint -esym(550,list_sz)   symbol not accessed */
int EXP_LVL9 CSmulrgQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csMulrg_
	if (fabs (gxDef->parameters.geocentricParameters.deltaX) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAX;
	}  */

	/* That's it for DMA Multiple Regression transformation. */
	return (err_cnt + 1);
}
/*lint +esym(550,list_sz) */
/******************************************************************************
*/
int EXP_LVL9 CSmulrgS (struct cs_GxXform_* gxXfrm)
{
	extern struct cs_Datum_ cs_Wgs84Def;

	short uu;
	short vv;
	short idx;
	short wrdIdx;
	short bitNbr;
	
	int flag;

	ulong32_t mask;

	struct csMulrg_ *mulrg;

	mulrg = &gxXfrm->xforms.mulrg;

	mulrg->uu_off = gxXfrm->gxDef.parameters.dmaMulRegParameters.phiOffset;
	mulrg->vv_off = gxXfrm->gxDef.parameters.dmaMulRegParameters.lambdaOffset;
	mulrg->normalizationScale = gxXfrm->gxDef.parameters.dmaMulRegParameters.normalizationScale;
	mulrg->validation = gxXfrm->gxDef.parameters.dmaMulRegParameters.validation;
	mulrg->testPhi = gxXfrm->gxDef.parameters.dmaMulRegParameters.testPhi;
	mulrg->testLambda = gxXfrm->gxDef.parameters.dmaMulRegParameters.testLambda;
	mulrg->deltaPhi = gxXfrm->gxDef.parameters.dmaMulRegParameters.deltaPhi;
	mulrg->deltaLambda = gxXfrm->gxDef.parameters.dmaMulRegParameters.deltaLambda;
	mulrg->deltaHeight = gxXfrm->gxDef.parameters.dmaMulRegParameters.deltaHeight;

	mulrg->max_uu = 0;
	mulrg->max_vv = 0;
	for (idx = 0;idx < 4;idx += 1)
	{
		mulrg->phiMap [idx]    = (ulong32_t)0;
		mulrg->lambdaMap [idx] = (ulong32_t)0;
		mulrg->heightMap [idx] = (ulong32_t)0;
	}

	for (uu = 0;uu < 10;uu += 1)
	{
		for (vv = 0;vv < 10;vv += 1)
		{
			idx = uu * 10 + vv;			/*lint !e734    result may be too many bits for a short */

			mulrg->phiCoefs [idx]    = gxXfrm->gxDef.parameters.dmaMulRegParameters.coeffPhi [idx];
			mulrg->lambdaCoefs [idx] = gxXfrm->gxDef.parameters.dmaMulRegParameters.coeffLambda [idx];
			mulrg->heightCoefs [idx] = gxXfrm->gxDef.parameters.dmaMulRegParameters.coeffHeight [idx];

			wrdIdx = idx >> 5;
			bitNbr = idx & 0x1F;
			mask = (ulong32_t)0x80000000L >> bitNbr;

			if (mulrg->phiCoefs [idx] != 0.0)
			{
				if (uu > mulrg->max_uu) mulrg->max_uu = uu;
				if (vv > mulrg->max_vv) mulrg->max_vv = vv;
				mulrg->phiMap [wrdIdx] |= mask;
			}
			if (mulrg->lambdaCoefs [idx] != 0.0)
			{
				if (uu > mulrg->max_uu) mulrg->max_uu = uu;
				if (vv > mulrg->max_vv) mulrg->max_vv = vv;
				mulrg->lambdaMap [wrdIdx] |= mask;
			}
			if (mulrg->heightCoefs [idx] != 0.0)
			{
				if (uu > mulrg->max_uu) mulrg->max_uu = uu;
				if (vv > mulrg->max_vv) mulrg->max_vv = vv;
				mulrg->heightMap [wrdIdx] |= mask;
			}
		}
	}


	/* Need to determine what the fallback transformation, if any, is.  Can
	   be either 7 parameter or Molodensky. */
	flag = (gxXfrm->srcDatum.delta_X != 0.0) ||
		   (gxXfrm->srcDatum.delta_Y != 0.0) ||
		   (gxXfrm->srcDatum.delta_Z != 0.0);
	if (!flag)
	{
		mulrg->fallback = cs_DTCMTH_NONE;
	}
	else
	{
		/* There is enough information to do a Molodensky anyway.  Probably
		   should be doing a geocentric translation, but we did Molodensky in
		   prior releases, so we'll use it now. */
		mulrg->fallback = cs_DTCMTH_MOLOD;
		flag = (gxXfrm->srcDatum.rot_X != 0.0) ||
			   (gxXfrm->srcDatum.rot_Y != 0.0) ||
			   (gxXfrm->srcDatum.rot_Z != 0.0);
		if (flag)
		{
			mulrg->fallback = cs_DTCMTH_6PARM;
			if (gxXfrm->srcDatum.bwscale != 0.0)
			{
				mulrg->fallback = cs_DTCMTH_7PARM;
			}
		}
	}

	/* Do what is required to support the selected fallback position.
	   Note, that the target datum of WGS84 is always implied on these
	   things. */
	switch (mulrg->fallback) {
	case cs_DTCMTH_MOLOD:
		CSmolodSf (&mulrg->fallbackXfrm.molod,&gxXfrm->srcDatum,&cs_Wgs84Def);		/*lint !e534   ignoring return value */
		break;
	case cs_DTCMTH_7PARM:
		CSparm7Sf (&mulrg->fallbackXfrm.parm7,&gxXfrm->srcDatum,&cs_Wgs84Def);		/*lint !e534   ignoring return value */
		break;
	case cs_DTCMTH_6PARM:
		// TODO: 
		//CSparm6Sf (&mulrg->fallbackXfrm.parm6,&gxXfrm->srcDatum,&cs_Wgs84Def);
		//break;
	case dtcTypNone:
	default:
		mulrg->fallback = dtcTypNone;
		break;
	}

	mulrg->errorValue    = gxXfrm->errorValue;
	mulrg->cnvrgValue    = gxXfrm->cnvrgValue;
	mulrg->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSmulrgF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSmulrgF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSmulrgI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSmulrgI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSmulrgL;
	gxXfrm->release = (cs_RELEASE_CAST)CSmulrgR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSmulrgD;

	return 0;
}
/* Note that this multiple regression function is called directly by the high
   level interface.  Contrary to the grid file interpolation techniques which
   are only called by the CS_gridi.c module.  Thus, the status values
   returned by this module needs to represent the status values which are
   to be returned to the high level user.

   Thus, to fix a defect (Trac 110), we necessarily use different status values
   here with regard to fall back modules than are used in the grid file
   interpolation modules. 

   To review:
    0 = a normal calculation
   -1 = a system error of some sort (physical I/O, heap corruption, etc.)
   +1 = the point to be converted is outside of the useful range of the
        transformation.  In this case, the point is returned unshifted.
   +2 = the point to be converted is outside of the useful range, but a
        fallback method was successfully used.
*/
int EXP_LVL9 CSmulrgF3 (struct csMulrg_ *mulrg,double* trgLl,Const double* srcLl)
{
	extern double cs_One;
	extern double cs_Sec2Deg;

	short ii;
	short jj;
	short idx;
	short wrdIdx;
	short bitNbr;

	int fbStatus;

	ulong32_t mask;

	double uu;
	double vv;
	double uu_pwr = 0.0;		// initialization to keep gcc compiler happy
	double vv_pwr = 0.0;		// initialization to keep gcc compiler happy

	double lngSum;
	double latSum;
	double hgtSum;

	double myLl [3];

	/* The default result is no conversion.  We also get a copy of the
	   source coordinates in an array which we can modify. */
	myLl [LNG] = trgLl [LNG] = srcLl [LNG];
	myLl [LAT] = trgLl [LAT] = srcLl [LAT];
	myLl [HGT] = trgLl [HGT] = srcLl [HGT];

	/* Compute the normalized input coordinates, uu and vv. */
	uu = (myLl [LAT] + mulrg->uu_off) * mulrg->normalizationScale;
	vv = (myLl [LNG] + mulrg->vv_off) * mulrg->normalizationScale;

	/* Make sure the values are within the range of the multiple rgression
	   formulas. */
	if (fabs (uu) > mulrg->validation || fabs (vv) > mulrg->validation)
	{
		/* If the input is out of range.  If there is a fallback position,
		   we use it now.  Otherwise, we simply return the original
		   coordinates.  In any case, we return a 1 to indicate that
		   something out of the ordinary happened.
		   
		   All fallbacks are forward transformations. */

		CS_erpt (cs_MREG_RANGE);			/* Register warning. */
		switch (mulrg->fallback) {
		case cs_DTCMTH_MOLOD:
			fbStatus = CSmolodF3 (&mulrg->fallbackXfrm.molod,trgLl,srcLl);
			break;
		case cs_DTCMTH_6PARM:
			fbStatus = CSparm6F3 (&mulrg->fallbackXfrm.parm6,trgLl,srcLl);
			break;
		case cs_DTCMTH_7PARM:
			fbStatus = CSparm7F3 (&mulrg->fallbackXfrm.parm7,trgLl,srcLl);
			break;
		case dtcTypNone:
		default:
			/* There is no fallback, return out of range indication. */
			CS_erpt (cs_MREG_RANGEF);
			fbStatus = 1;
			break;
		}
		/* +2 status says we used a fallback calculation. */
		return (fbStatus == 0) ? 2 : 1;
	}

	/* Initialize the variables in which we will accumulate
	   the delta values. */
	lngSum = latSum = hgtSum = 0.0;

	/* Start the loops necessary to perform the calculation. */
	for (ii = 0;ii <= mulrg->max_uu;ii++)
	{
		/* Compute the necessary uu power term for this
		   iteration of the loop. */
		if (ii == 0) uu_pwr  = cs_One;
		else	     uu_pwr *= uu;						/*lint !e644 */
		for (jj = 0;jj <= mulrg->max_vv;jj++)
		{
			/* Compute the necessary vv power term for this
			   iteration of the loop. */
			if (jj == 0) vv_pwr  = cs_One;
			else	     vv_pwr *= vv;					/*lint !e644 */

			/* Compute the bit map bit number for this
			   iteration for both loops. */
			idx = ii * 10 + jj;		/*lint !e734   too many bits for a short,  This works for MULREG! */
			bitNbr = (idx & 0x1F);
			wrdIdx = idx >> 5;
			mask = (ulong32_t)0x80000000L >> bitNbr;

			/* See if this term is in the longitude calculation. */
			if ((mulrg->lambdaMap [wrdIdx] & mask) != 0)
			{
				/* Yes it is, compute the value. */
				lngSum += mulrg->lambdaCoefs [idx] * uu_pwr * vv_pwr;
			}

			/* Same for the latitude. */
			if ((mulrg->phiMap [wrdIdx] & mask) != 0)
			{
				latSum += mulrg->phiCoefs [idx] * uu_pwr * vv_pwr;
			}

			/* Same for the height. */
			if ((mulrg->heightMap [wrdIdx] & mask) != 0)
			{
				hgtSum += mulrg->heightCoefs [idx] * uu_pwr * vv_pwr;
			}
		}
	}

	/* Compute the final results. */
	trgLl [LNG] = myLl [LNG] + lngSum * cs_Sec2Deg;
	trgLl [LAT] = myLl [LAT] + latSum * cs_Sec2Deg;
	trgLl [HGT] = myLl [HGT] + hgtSum;

	/* That's it. */
	return 0;
}
int EXP_LVL9 CSmulrgF2 (struct csMulrg_ *mulrg,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSmulrgF3 (mulrg,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSmulrgI3 (struct csMulrg_ *mulrg,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;					/* 0.0 */

	short lngOk;
	short latOk;

	int ii;
	int status;
	int rtnVal;

	double guess [3];
	double newLl [3];
	double epsilon [3];

	/* Assume everything goes OK until we know different. */
	status = rtnVal = 0;
	epsilon [0] = epsilon [1] = mulrg->cnvrgValue;		/* keep gcc compiler happy */

	/* First, we copy the WGS-84 lat/longs to the local array.
	   This is the default result which the user may want in
	   the event of an error.  Besides, we assume such has been
	   done below, even if there has not been an error. */
	guess [LNG] = trgLl [LNG] = srcLl [LNG];
	guess [LAT] = trgLl [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	/* We deal with height in a different manner. */
	guess [HGT] = cs_Zero;

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < mulrg->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		status = CSmulrgF3 (mulrg,newLl,guess);
		if (status < 0 || status == 1)
		{
			/* Something other than the normal situation happened.  Typically,
			   this means the current guess (perhaps the provided point) is
			   outside the useful range of the transformation and there was no
			   fallback.  In any case, we'll deal with this below as the end
			   of this 'for' loop code block. */
			break;
		}

		/* See how far we are off.  Note, we use the latitude and
		   longitude only.  Otherwise, we would never really converge. */
		epsilon [LNG] = srcLl [LNG] - newLl [LNG];
		epsilon [LAT] = srcLl [LAT] - newLl [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > mulrg->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > mulrg->cnvrgValue)
		{
			latOk = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newLl that is within
		   samllValue of srcLl, we are done. */
		if (lngOk && latOk) break;
	}

	/* We now need to deal with the situation where the forward function
	   used in the iterative loop above indicated that the last guess
	   (often the original point) was outside of the useful range of the
	   transformation.  Note, that encountering any guess that is outside
	   of the useful range, we deal with as indicated here. */
	if (status == 0 || status == 2)
	{
		/* If we didn't resolve in maxIteration tries, we issue a warning
		   message. */
		if (ii >= mulrg->maxIterations)
		{
			CS_erpt (cs_WGS_CNVRG);

			/* Issue a warning if we're close, a fatal if we are still way off.
			   In any case, we return the last computed value.  We could have
			   gotten very fancy with this stuff, but it would have had serious
			   affects on the performance.  So we just check epsilon here as
			   we know we have an error and this doesn't happen very often. */
			rtnVal = 1;
			if (fabs (epsilon [LNG]) > mulrg->errorValue ||
				fabs (epsilon [LAT]) > mulrg->errorValue)
			{
				/* In normal use, this never happens.  However, when processing
				   data on the edges of the coverage, this can happen.  We have
				   a choice.  Increase the values of "errorValue", or simply
				   consider a failure to converge in the specified number of
				   iterations as a coverage issue.  For now, we consider a
				   failure to converge as a coverage issue. */
				rtnVal = 1;
			}
		}

		/* Adjust the ll_lcl value to the computed value, now that we
		   know that it should be correct. */
		if (rtnVal == 0)
		{
			trgLl [LNG] = guess [LNG];
			trgLl [LAT] = guess [LAT];
			/* The iterative forward calculations above are all done with a height
			   value of zero.  Thus, the height returned is essentially a delta
			   height. */
			trgLl [HGT] = srcLl [HGT] - guess [HGT];
		}
	}
	if (status == 1)
	{
		/* We were provided a point which is outside of the useful range
		   of this transformation, or the iterative conversion technique
		   wandered outside the useful range of the transformation. */
		CS_erpt (cs_MREG_RANGE);			/* Register warning message. */
		trgLl [LNG] = srcLl [LNG];
		trgLl [LAT] = srcLl [LAT];
		trgLl [HGT] = srcLl [HGT];
		rtnVal = 1;
	}
	else if (status < 0)
	{
		/* We encountered some sort of system error; i.e. an error which is
		   not related to the point that has been provided. Heap corruption,
		   out of memory, physical I/O error are examples. */
		CS_erpt (cs_NO_MEM);
		rtnVal = -1;
	}
	return rtnVal;
}

/******************************************************************************
*/
int EXP_LVL9 CSmulrgI2 (struct csMulrg_ *mulrg,double* trgLl,Const double* srcLl)
{
	extern double cs_Zero;					/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CSmulrgI3 (mulrg,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSmulrgL (struct csMulrg_ *mulrg,int cnt,Const double pnts [][3])
{
	int status;
	int index;

	double uu;
	double vv;

	status = cs_CNVRT_OK;
	for (index = 0;index < cnt;index += 1)
	{
		/* Compute the normalized input coordinates, uu and vv. */
		uu = (pnts [index][LAT] + mulrg->uu_off) * mulrg->normalizationScale;
		vv = (pnts [index][LNG] + mulrg->vv_off) * mulrg->normalizationScale;

		if (fabs (uu) > mulrg->validation || fabs (vv) > mulrg->validation)
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}
	return status;
}
int EXP_LVL9 CSmulrgN  (struct csMulrg_ *mulrg)
{
	return FALSE;
}
int EXP_LVL9 CSmulrgR (struct csMulrg_ *mulrg)
{
	return 0;
}
int EXP_LVL9 CSmulrgD (struct csMulrg_ *mulrg)
{
	return 0;
}

