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

/*lint -e801	Use of goto is deprecated */

#include "cs_map.h"

/*                           meters       unitless      degrees      degrees */
struct cs_Gauss_* EXP_LVL3 CSgaussS (double e_rad,double e_sq,double orgLng,double stdLat)
{
	extern double cs_Zero;
	extern double cs_Half;
	extern double cs_One;
	extern double cs_Pi_o_4;		/* pi over 4 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc in radians. */

	extern double cs_Degree;
	extern double cs_Mhuge;

	struct cs_Gauss_* gauss;

	double sinStdLat;
	double cosStdLat;
	double lat0;
	double sinLat0;

	double tmp1, tmp2, tmp3, tmp4;

	gauss = (struct cs_Gauss_ *)CS_malc (sizeof (struct cs_Gauss_));
	if (gauss == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;				
	}

	gauss->e_rad = e_rad;
	gauss->e_sq = e_sq;
	gauss->ecent = sqrt (gauss->e_sq);
	gauss->e_ovr_2 = gauss->ecent * cs_Half;
	gauss->one_esq = cs_One - gauss->e_sq;
	gauss->orgLng = orgLng * cs_Degree;
	gauss->stdLat = stdLat * cs_Degree;
	gauss->resolve = cs_AnglTest;

	if (gauss->ecent < 1.0E-12)
	{
		/* We assume a sphere.  Don't know why anyone would want
		   to do this, but we handle it anyway. */
		gauss->N0 = gauss->e_rad;
		gauss->alpha = cs_One;
		gauss->alpha_eo2 = cs_One;
		gauss->K = cs_Zero;
		gauss->logK = cs_Mhuge;
		gauss->alpha_eo2 = cs_Zero;
	}
	else
	{
		sinStdLat = sin (gauss->stdLat);
		cosStdLat = cos (gauss->stdLat);

		/* N0 is the radius of the Gaussian sphere.  The formulation hides this,
		   but it equivalent to the geometric mean of the meridional and first
		   vertical section radii of curvature at the specified origin latitude. */
		tmp1 = cs_One - gauss->e_sq * sinStdLat * sinStdLat;
		gauss->N0 = gauss->e_rad * sqrt (gauss->one_esq) / tmp1;

		tmp1 = cosStdLat * cosStdLat;
		tmp1 = tmp1 * tmp1;
		tmp1 = (gauss->e_sq * tmp1) / gauss->one_esq;
		gauss->alpha = sqrt (cs_One + tmp1);
		gauss->alpha_eo2 = gauss->alpha * gauss->ecent * cs_Half;

		sinLat0 = sinStdLat / gauss->alpha;
		lat0 = asin (sinLat0);

		tmp1 = gauss->ecent * sinStdLat;
		tmp2 = tan (cs_Pi_o_4 + lat0 * cs_Half);
		tmp3 = tan (cs_Pi_o_4 + gauss->stdLat  * cs_Half);
		tmp4 = (cs_One + tmp1) / (cs_One - tmp1);
		gauss->logK = log (tmp2) - gauss->alpha * log (tmp3) + gauss->alpha_eo2 * log (tmp4);
		gauss->K = exp (gauss->logK);
	}
	return gauss;
error:
	if (gauss != NULL)			/*lint !e774 */
	{
		CS_free (gauss);
	}
	return NULL;
}
int EXP_LVL3 CSgaussF (struct cs_Gauss_* gauss,double gaussian [2],double ellipsoidal [2])
{

	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;
	extern double cs_Pi_o_4;		/* pi over 4 */
	extern double cs_Pi_o_2;		/* pi over 2 */
	extern double cs_Degree;
	extern double cs_Radian;

	int rtnValue = cs_CNVRT_NRML;
	
	double lngE, latE;
	double lngS, latS;

	double tmp1, tmp2, tmp3;

	lngE = (ellipsoidal [LNG] * cs_Degree) - gauss->orgLng;
	latE = ellipsoidal [LAT] * cs_Degree;

	if (gauss->ecent < 1.0E-12)
	{
		lngS = lngE;
		latS = latE;
	}
	else
	{
		lngS = (lngE * gauss->alpha) * cs_Radian;

		tmp1 = gauss->ecent * sin (latE);
		tmp1 = (cs_One + tmp1) / (cs_One - tmp1);
		tmp2 = tan (cs_Pi_o_4 + latE * cs_Half);
		tmp3 = gauss->alpha * log (tmp2) - gauss->alpha_eo2 * log (tmp1) + gauss->logK;
		latS = cs_Two * atan (exp (tmp3)) - cs_Pi_o_2;
	}
	gaussian [LNG] = lngS * cs_Radian;
	gaussian [LAT] = latS * cs_Radian;
	
	return rtnValue;
}
int EXP_LVL3 CSgaussI (struct cs_Gauss_* gauss,double ellipsoidal [2],double gaussian [2])
{
	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;
	extern double cs_Pi_o_4;		/* pi over 4 */
	extern double cs_Pi_o_2;		/* pi over 2 */
	extern double cs_Degree;
	extern double cs_Radian;

	int itrMax;
	int rtnValue;

	double lngS, latS;
	double lngE, latE;
	double lastLatE;
	double Q;
	double tmp1;
	double tmp2;

	rtnValue = cs_CNVRT_NRML;

	lngS = gaussian [LNG] * cs_Degree;
	latS = gaussian [LAT] * cs_Degree;

	if (gauss->ecent < 1.0E-12)
	{
		lngE = lngS;
		latE = latS;
	}
	else
	{
		lngE = lngS / gauss->alpha;

		tmp1 = tan (cs_Pi_o_4 + latS * cs_Half);
		tmp1 = (log (tmp1) - gauss->logK) / gauss->alpha;
		latE = gauss->stdLat;
		itrMax = 10;			/* usually 4 or 5 does it */
		do
		{
			lastLatE = latE;
			tmp2 = gauss->ecent * sin (lastLatE);
			tmp2 = (cs_One + tmp2) / (cs_One - tmp2);
			Q = tmp1 + gauss->e_ovr_2 * log (tmp2);
			latE = cs_Two * atan (exp (Q)) - cs_Pi_o_2;
		} while (--itrMax >= 0 && fabs (latE - lastLatE) > gauss->resolve);
		if (itrMax < 0)
		{
			rtnValue = cs_CNVRT_DOMN;
		}
	}
	ellipsoidal [LNG] = (lngE + gauss->orgLng) * cs_Radian;
	ellipsoidal [LAT] = latE * cs_Radian;
	
	return rtnValue;
}
