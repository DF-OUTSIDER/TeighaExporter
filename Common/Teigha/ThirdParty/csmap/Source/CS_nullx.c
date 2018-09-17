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

/*lint -esym(550,list_sz)   variable not accessed */
int EXP_LVL9 CSnullxQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csNullx_
	   There is nothing to check here, but we leave a sample test
	   commented out to give the future developer a clue as to how
	   this function is supposed to work.
	if (fabs (gxDef->parameters.geocentricParameters.deltaX) > 1.0E-06)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAX;
	}  */
	return (err_cnt + 1);
}
/*lint +esym(550,list_sz) */
int EXP_LVL9 CSnullxS (struct cs_GxXform_* gxXfrm)
{
	struct csNullx_* nullx;

	nullx = &gxXfrm->xforms.nullx;
	
	nullx->errorValue    = gxXfrm->errorValue;
	nullx->cnvrgValue    = gxXfrm->cnvrgValue;
	nullx->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSnullxF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSnullxF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSnullxI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSnullxI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSnullxL;
	gxXfrm->isNull  = (cs_ISNULL_CAST) CSnullxN;
	gxXfrm->release = (cs_RELEASE_CAST)CSnullxR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSnullxD;
	
	return 0;
}
int EXP_LVL9 CSnullxF3 (struct csNullx_ *nullx,double trgLl [3],Const double srcLl [3])
{
	if (trgLl != srcLl)
	{
		trgLl [LNG] = srcLl [LNG];
		trgLl [LAT] = srcLl [LAT];
		trgLl [HGT] = srcLl [HGT];
	}
	return 0;
}
int EXP_LVL9 CSnullxF2 (struct csNullx_ *nullx,double* trgLl,Const double* srcLl)
{
	if (trgLl != srcLl)
	{
		trgLl [LNG] = srcLl [LNG];
		trgLl [LAT] = srcLl [LAT];
		trgLl [HGT] = srcLl [HGT];
	}
	return 0;
}
int EXP_LVL9 CSnullxI3 (struct csNullx_ *nullx,double* trgLl,Const double* srcLl)
{
	if (trgLl != srcLl)
	{
		trgLl [LNG] = srcLl [LNG];
		trgLl [LAT] = srcLl [LAT];
		trgLl [HGT] = srcLl [HGT];
	}
	return 0;
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
int EXP_LVL9 CSnullxI2 (struct csNullx_ *nullx,double* trgLl,Const double* srcLl)
{
	if (trgLl != srcLl)
	{
		trgLl [LNG] = srcLl [LNG];
		trgLl [LAT] = srcLl [LAT];
		trgLl [HGT] = srcLl [HGT];
	}
	return 0;
}
int EXP_LVL9 CSnullxL (struct csNullx_ *nullx,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSnullxN (struct csNullx_ *nullx)
{
	return TRUE;
}
int EXP_LVL9 CSnullxR (struct csNullx_ *nullx)
{
	return 0;
}
int EXP_LVL9 CSnullxD (struct csNullx_ *nullx)
{
	return 0;
}
