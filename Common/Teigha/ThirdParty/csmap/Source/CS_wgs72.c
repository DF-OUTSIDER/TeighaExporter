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

static const double wgs72Erad = 6378135.000;
static const double wgs72deltaF = 0.3121057E-07;
int EXP_LVL9 CSwgs72Q (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,int err_list [],int list_sz)
{
	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to cswgs72_
	   Unlike other methods, this is a method explicitly
	   for WGS72 to WGS84.  Thus, we verify that the target
	   datum is indeed WGS84.  It is perhaps a bit klutzy
	   to do this strictly by name; but WGS72 is rearely used. */
	if (CS_stricmp (gxDef->srcDatum,"WGS72"))
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_WGS72SRC;
	}
	if (CS_stricmp (gxDef->trgDatum,"WGS84"))
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_WGS72TRG;
	}
	return (err_cnt + 1);
}
int EXP_LVL9 CSwgs72S (struct cs_GxXform_* gxXfrm)
{
	struct csWgs72_ *wgs72;

	wgs72 = &gxXfrm->xforms.wgs72;

	wgs72->errorValue    = gxXfrm->errorValue;
	wgs72->cnvrgValue    = gxXfrm->cnvrgValue;
	wgs72->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSwgs72F2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSwgs72F3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSwgs72I2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSwgs72I3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSwgs72L;
	gxXfrm->release = (cs_RELEASE_CAST)CSwgs72R;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSwgs72D;

	return 0;
}
int EXP_LVL9 CSwgs72F3 (struct csWgs72_ *wgs72,double ll_84 [3],Const double ll_72 [3])
{
	extern double cs_Two;				/* 2.0 */
	extern double cs_Degree;			/* 1.0 / 57.2... */
	extern double cs_Sin1Sec;			/* sine of 1 second of arc. */
	extern double cs_Sec2Deg;			/* 1/3600 */

	double lat;
	double del_lat;
	double del_hgt;
	double sin_lat;
	double cos_lat;
	double sin_2_lat;

	lat = ll_72 [LAT] * cs_Degree;
	sin_lat = sin (lat);
	cos_lat = cos (lat);
	sin_2_lat = sin (cs_Two * lat);

	del_lat = ((4.5 * cos_lat) / (wgs72Erad * cs_Sin1Sec)) +
			  ((wgs72deltaF * sin_2_lat) / cs_Sin1Sec);
	del_hgt = (4.5 * sin_lat) +
			  (wgs72Erad * wgs72deltaF * sin_lat * sin_lat) +
			  1.4 - 2.0;

	ll_84 [LNG] = ll_72 [LNG] + (0.554 / 3600.00);
	ll_84 [LAT] = ll_72 [LAT] + (del_lat * cs_Sec2Deg);
	ll_84 [HGT] = ll_72 [HGT] + del_hgt;

	return 0;
}
int EXP_LVL9 CSwgs72F2 (struct csWgs72_ *wgs72,double* ll_84,Const double* ll_72)
{
	extern double cs_Two;				/* 2.0 */
	extern double cs_Degree;			/* 1.0 / 57.2... */
	extern double cs_Sin1Sec;			/* sine of 1 second of arc. */
	extern double cs_Sec2Deg;			/* 1/3600 */

	double lat;
	double del_lat;
	double cos_lat;
	double sin_2_lat;

	lat = ll_72 [LAT] * cs_Degree;
	cos_lat = cos (lat);
	sin_2_lat = sin (cs_Two * lat);

	del_lat = ((4.5 * cos_lat) / (wgs72Erad * cs_Sin1Sec)) +
			  ((wgs72deltaF * sin_2_lat) / cs_Sin1Sec);

	ll_84 [LNG] = ll_72 [LNG] + (0.554 / 3600.00);
	ll_84 [LAT] = ll_72 [LAT] + (del_lat * cs_Sec2Deg);
	ll_84 [HGT] = ll_72 [HGT];

	return 0;
}
int EXP_LVL9 CSwgs72I3 (struct csWgs72_ *wgs72,double* ll_72,Const double* ll_84)
{
	extern double cs_Two;
	extern double cs_Degree;
	extern double cs_Sin1Sec;
	extern double cs_Sec2Deg;

	double lat;
	double del_lat;
	double del_hgt;
	double sin_lat;
	double cos_lat;
	double sin_2_lat;

	lat = ll_84 [LAT] * cs_Degree;

	sin_lat = sin (lat);
	cos_lat = cos (lat);
	sin_2_lat = sin (cs_Two * lat);

	/* Probably should do a real inverse on the latitude, but the
	   following works rather precisely (good to 0.00000005 seconds). */
	del_lat = ((4.5 * cos_lat) / (wgs72Erad * cs_Sin1Sec)) +
					((wgs72deltaF * sin_2_lat) / cs_Sin1Sec);
	del_hgt = (4.5 * sin_lat) +
				(wgs72Erad * wgs72deltaF * sin_lat * sin_lat) +
				1.4 - 2.0;

	ll_72 [LNG] = ll_84 [LNG] - (0.554 / 3600.00);
	ll_72 [LAT] = ll_84 [LAT] - (del_lat * cs_Sec2Deg);
	ll_72 [HGT] = ll_84 [HGT] - del_hgt;

	return 0;
}

/******************************************************************************
	No need for the iterative solution here as the longitude and latitude
	conversions are completely independent of the height conversion.
*/
int EXP_LVL9 CSwgs72I2 (struct csWgs72_ *wgs72,double* ll_72,Const double* ll_84)
{
	extern double cs_Two;
	extern double cs_Degree;
	extern double cs_Sin1Sec;
	extern double cs_Sec2Deg;

	double lat;
	double del_lat;
	double cos_lat;
	double sin_2_lat;

	lat = ll_84 [LAT] * cs_Degree;

	cos_lat = cos (lat);
	sin_2_lat = sin (cs_Two * lat);

	/* Probably should do a real inverse on the latitude, but the
	   following works rather precisely (good to 0.00000005 seconds). */
	del_lat = ((4.5 * cos_lat) / (wgs72Erad * cs_Sin1Sec)) +
					((wgs72deltaF * sin_2_lat) / cs_Sin1Sec);

	ll_72 [LNG] = ll_84 [LNG] - (0.554 / 3600.00);
	ll_72 [LAT] = ll_84 [LAT] - (del_lat * cs_Sec2Deg);
	ll_72 [HGT] = ll_84 [HGT];

	return 0;
}
int EXP_LVL9 CSwgs72L (struct csWgs72_ *wgs72,int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSwgs72N  (struct csWgs72_ *wgs72)
{
	return FALSE;
}
int EXP_LVL9 CSwgs72R (struct csWgs72_ *wgs72)
{
	return 0;
}
int EXP_LVL9 CSwgs72D (struct csWgs72_ *wgs72)
{
	return 0;
}
