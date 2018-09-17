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

	Provides comparison functions for checking the veracity of things like
	WKT export/import.
*/

#include "cs_map.h"
#include "cs_Legacy.h"

/* The following function compares only those portions of a Coordinate System
   definition which affect numeric results.  The number of differences found
   is returned.  Zero return indcates a match.  If the message argument is
   not NULL, and the messageSize argument is greater than one, a description
   of the first difference located is returned. */
int EXP_LVL3 CS_csDefCmp (Const struct cs_Csdef_ *original,Const struct cs_Csdef_ *revised,char* message,size_t messageSize)
{
	extern struct cs_Prjtab_ cs_Prjtab [];		/* Projection Table */
	extern double cs_Six;
	extern double cs_Zero;

	int errCnt;
	int errCntCnc;

	struct cs_Prjtab_ *pp;

	double unitsFactor;

	char errMsg [512];

	const struct cs_Csdef_ *lclOrgPtr;
	const struct cs_Csdef_ *lclRevPtr;
	struct cs_Csdef_ lclOriginal;
	struct cs_Csdef_ lclRevised;

	errCnt = 0;
	errCntCnc = 0;
	errMsg [0] = '\0';				/* Defensive programming, also keeps lint happy */
	if (message != NULL && messageSize > 1)
	{
		/* Initialize the users messge array. */
		*message = '\0';
	}

	/* Before we get onto this to heavy; we check the projection code of the
	   original and the revised.  If the original is UTM, and the revised is
	   TM, we convert the original from UTM form to TM form.  This eliminates
	   a big headache. */
	lclOrgPtr = original;
	if (!CS_stricmp (original->prj_knm,"UTM") && !CS_stricmp (revised->prj_knm,"TM"))
	{
		/* Convert original to the TM form. */
		memcpy (&lclOriginal,original,sizeof (lclOriginal));
		strcpy (lclOriginal.prj_knm,"TM");
		lclOriginal.prj_prm1 = cs_Six * lclOriginal.prj_prm1 - 183.0;
		lclOriginal.org_lat = cs_Zero;
		unitsFactor = CS_unitlu (cs_UTYP_LEN,lclOriginal.unit);
		lclOriginal.x_off = 500000.0 / unitsFactor;
		lclOriginal.y_off = (lclOrgPtr->prj_prm2 >= 0.0) ? cs_Zero : 10000000.0 / unitsFactor;
		lclOriginal.scl_red = 0.9996;
		lclOriginal.quad = 1;
		lclOrgPtr = &lclOriginal;
	}
	lclRevPtr = revised;
	if (!CS_stricmp (revised->prj_knm,"UTM") && !CS_stricmp (original->prj_knm,"TM"))
	{
		/* Convert revised to the TM form. */
		memcpy (&lclRevised,revised,sizeof (lclRevised));
		strcpy (lclRevised.prj_knm,"TM");
		lclRevised.prj_prm1 = cs_Six * lclRevised.prj_prm1 - 183.0;
		lclRevised.org_lat = cs_Zero;
		unitsFactor = CS_unitlu (cs_UTYP_LEN,lclRevised.unit);
		lclRevised.x_off = 500000.0 / unitsFactor;
		lclRevised.y_off = (lclRevPtr->prj_prm2 >= 0.0) ? cs_Zero : 10000000.0 / unitsFactor;
		lclRevised.scl_red = 0.9996;
		lclRevised.quad = 1;
		lclRevPtr = &lclRevised;
	}

	if (CS_stricmp (lclOrgPtr->prj_knm,lclRevPtr->prj_knm))
	{
		if (errCnt == 0)		/*lint !e774   boolean always evaluates to True (correct in this case) */
		{
			sprintf (errMsg,"Projection key name was %s, is now %s",lclOrgPtr->prj_knm,lclRevPtr->prj_knm);
		}
		errCnt += 1;
	}
	if (CS_stricmp (lclOrgPtr->unit,lclRevPtr->unit))
	{
		if (errCnt == 0)		/*lint !e774   boolean always evaluates to True (not always in this case) */
		{
			sprintf (errMsg,"Unit name was %s, is now %s",lclOrgPtr->unit,lclRevPtr->unit);
		}
		errCnt += 1;
	}

	/* If the projection and/or unit names don't match, we're in deep do-do.
	   So, we bag it now. */
	if (errCnt != 0)			/*lint !e774   boolean always evaluates to True (not always in this case) */
	{
		if (message != 0 && messageSize > 1)
		{
			CS_stncp (message,errMsg,(int)messageSize);
		}
		return errCnt;
	}

	/* Look this projection up in the projection table, we need to pass the
	   projection code to the parameter check function. */
	for (pp = cs_Prjtab;pp->key_nm [0] != '\0';pp += 1)
	{
		if (!CS_stricmp (lclOrgPtr->prj_knm,pp->key_nm))
		{
			break;
		}
	}
	if (pp->key_nm [0] == '\0')
	{
		/* The projection name in the revised definition is not in the
		   projection data table. */
		if (errCnt == 0)			/*lint !e774   boolean always evaluates to True (correct in this instance) */
		{
			sprintf (errMsg,"Projection key name is now %s which is invalid.",lclRevPtr->prj_knm);
		}
		errCnt += 1;
	}
	else
	{
		/* We have the required pointer to the approriate projection
		   table entry.  We can proceed on to the real work of this
		   function.

		   Check all of the parameters. */
		errMsg[0] = '\0';

		/* We skip checking the first two parameters for the Unity projection as WKT
		   does not support the longitude range feature.  If the projection is a 
		   conic, the first two parameters require special treatment. */
		if (pp->code != cs_PRJCOD_UNITY)
		{
			/* For conic projections which require two standard parallels, the order of the parallels
			   does not make any difference to the mathemagics of the projection.  In this case,
			   if (prm1 != prm1 && prm2 != prm2) we do an additional check of
			   if (prm1 == prm2 && prm2 == prm1).  It is traditional to supply the northern
			   parallel first, but there are variations of WKT out there to do not follow this
			   tradition (and it is only a tradition). */
			if (pp->code == cs_PRJCOD_LM2SP   ||
				pp->code == cs_PRJCOD_LMBLG   ||
				pp->code == cs_PRJCOD_WCCSL   ||
				pp->code == cs_PRJCOD_MNDOTL  ||
				pp->code == cs_PRJCOD_ALBER   ||
				pp->code == cs_PRJCOD_LMBRTAF)
			{
				errCntCnc = 0;
				errCntCnc += CS_defCmpPrjPrm (pp, 1,lclOrgPtr->prj_prm1 ,lclRevPtr->prj_prm1,errMsg,sizeof (errMsg));
				errCntCnc += CS_defCmpPrjPrm (pp, 2,lclOrgPtr->prj_prm2 ,lclRevPtr->prj_prm2,errMsg,sizeof (errMsg));
				if (errCntCnc == 2)
				{
					errCnt += CS_defCmpPrjPrm (pp, 1,lclOrgPtr->prj_prm1 ,lclRevPtr->prj_prm2,errMsg,sizeof (errMsg));
					errCnt += CS_defCmpPrjPrm (pp, 2,lclOrgPtr->prj_prm2 ,lclRevPtr->prj_prm1,errMsg,sizeof (errMsg));
				}
			}
			else
			{
				/* Not a conic nor geographic, so we just compare the two
				   parameter values. */
				errCnt += CS_defCmpPrjPrm (pp, 1,lclOrgPtr->prj_prm1 ,lclRevPtr->prj_prm1,errMsg,sizeof (errMsg));
				errCnt += CS_defCmpPrjPrm (pp, 2,lclOrgPtr->prj_prm2 ,lclRevPtr->prj_prm2,errMsg,sizeof (errMsg));
			}
		}
		
		/* The f1rst two parameters have been dealt with, dealing with the rest
		   is rather straight forward. */
		errCnt += CS_defCmpPrjPrm (pp, 3,lclOrgPtr->prj_prm3 ,lclRevPtr->prj_prm3,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 4,lclOrgPtr->prj_prm4 ,lclRevPtr->prj_prm4,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 5,lclOrgPtr->prj_prm5 ,lclRevPtr->prj_prm5,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 6,lclOrgPtr->prj_prm6 ,lclRevPtr->prj_prm6,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 7,lclOrgPtr->prj_prm7 ,lclRevPtr->prj_prm7,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 8,lclOrgPtr->prj_prm8 ,lclRevPtr->prj_prm8,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp, 9,lclOrgPtr->prj_prm9 ,lclRevPtr->prj_prm9,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,10,lclOrgPtr->prj_prm10,lclRevPtr->prj_prm10,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,11,lclOrgPtr->prj_prm11,lclRevPtr->prj_prm11,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,12,lclOrgPtr->prj_prm12,lclRevPtr->prj_prm12,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,13,lclOrgPtr->prj_prm13,lclRevPtr->prj_prm13,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,14,lclOrgPtr->prj_prm14,lclRevPtr->prj_prm14,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,15,lclOrgPtr->prj_prm15,lclRevPtr->prj_prm15,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,16,lclOrgPtr->prj_prm16,lclRevPtr->prj_prm16,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,17,lclOrgPtr->prj_prm17,lclRevPtr->prj_prm17,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,18,lclOrgPtr->prj_prm18,lclRevPtr->prj_prm18,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,19,lclOrgPtr->prj_prm19,lclRevPtr->prj_prm19,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,20,lclOrgPtr->prj_prm20,lclRevPtr->prj_prm20,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,21,lclOrgPtr->prj_prm21,lclRevPtr->prj_prm21,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,22,lclOrgPtr->prj_prm22,lclRevPtr->prj_prm22,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,23,lclOrgPtr->prj_prm23,lclRevPtr->prj_prm23,errMsg,sizeof (errMsg));
		errCnt += CS_defCmpPrjPrm (pp,24,lclOrgPtr->prj_prm24,lclRevPtr->prj_prm24,errMsg,sizeof (errMsg));
	}

	/* Deal with the rest of the projected coordinate system definition. */
	if ((pp->flags & cs_PRJFLG_ORGLAT) == 0)
	{
		if (CS_cmpDbls (lclOrgPtr->org_lat,lclRevPtr->org_lat) == 0)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Origin Latitude was %14.8f, is now %14.8f",lclOrgPtr->org_lat,lclRevPtr->org_lat);
			}
			errCnt += 1;
		}
	}
	if ((pp->flags & cs_PRJFLG_ORGLNG) == 0)
	{
		if (CS_cmpDbls (lclOrgPtr->org_lng,lclRevPtr->org_lng) == 0)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Origin Longitude was %14.8f, is now %14.8f",lclOrgPtr->org_lng,lclRevPtr->org_lng);
			}
			errCnt += 1;
		}
	}
	if ((pp->flags & cs_PRJFLG_ORGFLS) == 0)
	{
		if (fabs (lclOrgPtr->x_off - lclRevPtr->x_off) > 0.001)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"False easting was %14.3f, is now %14.3f",lclOrgPtr->x_off,lclRevPtr->x_off);
			}
			errCnt += 1;
		}
		if (fabs (lclOrgPtr->y_off - lclRevPtr->y_off) > 0.001)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"False northing was %14.3f, is now %14.3f",lclOrgPtr->y_off,lclRevPtr->y_off);
			}
			errCnt += 1;
		}
	}
	if ((pp->flags & cs_PRJFLG_SCLRED) != 0)
	{
		if (CS_cmpDbls (lclOrgPtr->scl_red,lclRevPtr->scl_red) == 0)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Scale reduction was %12.10f, is now %12.10f",lclOrgPtr->scl_red,lclRevPtr->scl_red);
			}
			errCnt += 1;
		}
	}
	if (lclOrgPtr->quad != lclRevPtr->quad)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"Quad was %d, is now %d",lclOrgPtr->quad,lclRevPtr->quad);
		}
		errCnt += 1;
	}

	if (errCnt != 0)
	{
		if (message != 0 && messageSize > 1)
		{
			CS_stncp (message,errMsg,(int)messageSize);
		}
	}
	return errCnt;
}
int EXP_LVL3 CS_defCmpPrjPrm (struct cs_Prjtab_* pp,int prmNbr,double orgValue,double revValue,char *message,size_t messageSize)
{
	extern struct cs_PrjprmMap_ cs_PrjprmMap [];
	extern struct cs_Prjprm_ csPrjprm [];

	int errCnt;
	unsigned char parmType;
	struct cs_PrjprmMap_ *mapPtr;
	struct cs_Prjprm_ *prmPtr;
	double tolerance;

	char errMsg [512];

	errCnt = 0;
	errMsg [0] = '\0';			/* Defensive programming, also keeps lint happy */

	/* Get the type of parameter. */
	for (mapPtr = cs_PrjprmMap;mapPtr->prj_code != cs_PRJCOD_END;mapPtr += 1)
	{
		if (mapPtr->prj_code == pp->code) break;
	}
	if (mapPtr->prj_code == cs_PRJCOD_END)
	{
		sprintf (errMsg,"Projection code did not map.");
		errCnt += 1;
	}
	else
	{
		/* Locate the parameter type. */
		parmType = mapPtr->prm_types [prmNbr - 1];

		/* See if this parameter is used. */
		if (parmType == 0)
		{
			/* It's not used, so we don't compare it. */
			return 0;
		}
		
		/* Get a pointer to the parameter table, extract the logical type of
		   the parameter, and then use the logical type to determine a
		   tolerance value for the follwing comparison. */
		prmPtr = &csPrjprm [parmType];
		switch (prmPtr->log_type) {
		case cs_PRMLTYP_LNG:
		case cs_PRMLTYP_LAT:
		case cs_PRMLTYP_AZM:
			tolerance = 1.0E-07;
			break;
		case cs_PRMLTYP_ANGD:
			tolerance = 1.0E-06;
			break;
		case cs_PRMLTYP_CMPLXC:
			tolerance = 1.0E-08;
			break;
		case cs_PRMLTYP_ZNBR:
			tolerance = 0.5;
			break;
		case cs_PRMLTYP_HSNS:
			tolerance = 0.01;
			break;
		case cs_PRMLTYP_GHGT:
		case cs_PRMLTYP_ELEV:
			tolerance = 1.0E-02;
			break;
		case cs_PRMLTYP_AFCOEF:
			tolerance = 1.0E-08;
			break;
		case cs_PRMLTYP_XYCRD:
			tolerance = 1.0E-02;
			break;
		case cs_PRMLTYP_SCALE:
			tolerance = 1.0E-06;
			break;
		case cs_PRMLTYP_NONE:
		default:
			tolerance = 1.0E-12;
			break;
		}
		
		/* Do a generic compare of the values and then report any problem. */
		if (fabs (orgValue - revValue) >= tolerance)
		{
			/* they didn't match. */
			sprintf (errMsg,"%s was %16.8f, in now %18.8f",prmPtr->label,orgValue,revValue);
			errCnt += 1;
		}
	}
	if (message != NULL && messageSize > 1 && *message == '\0' && errCnt > 0)
	{
		CS_stncp (message,errMsg,(int)messageSize);
	}
	return errCnt;
}
int EXP_LVL3 CS_dtDefCmp (Const struct cs_Dtdef_ *original,Const struct cs_Dtdef_ *revised,char* message,size_t messageSize)
{
	static const double deltaEpsilon = 6.0E-04;
	static const double rotEpsilon = 5.0E-04;
	static double scaleEpsilon = 5.0E-06;

	int orgIsNull;
	int revIsNull;
	int ellipsoidUse = 0;

	short paramUseCount = -1;
	short orgDeltaZeroCount;
	short revDeltaZeroCount;

	int errCnt = 0;

	char errMsg [512];

	errMsg [0] = '\0';
	if (message != NULL && messageSize > 0) *message = '\0';

	/* The revised will often get converted to a type cs_DTCTYP_GEOCTR with zero
	   deltas if it was one of those types that is generally considered to be
	   equivalent with WGS84. */
	orgDeltaZeroCount  = (original->delta_X == 0.0);
	orgDeltaZeroCount += (original->delta_Y == 0.0);
	orgDeltaZeroCount += (original->delta_Z == 0.0);
	orgIsNull = (original->to84_via == cs_DTCTYP_WGS84  ||
			  original->to84_via == cs_DTCTYP_NAD83  ||
			  original->to84_via == cs_DTCTYP_WGS84  ||
			  original->to84_via == cs_DTCTYP_GDA94  ||
			  original->to84_via == cs_DTCTYP_NZGD2K ||
			  original->to84_via == cs_DTCTYP_ETRF89 ||
			  original->to84_via == cs_DTCTYP_RGF93  ||
				 (orgDeltaZeroCount == 3 &&
					(original->to84_via == cs_DTCTYP_GEOCTR || original->to84_via == cs_DTCTYP_3PARM)
		   )
				);
					
	revDeltaZeroCount  = (revised->delta_X == 0.0);
	revDeltaZeroCount += (revised->delta_Y == 0.0);
	revDeltaZeroCount += (revised->delta_Z == 0.0);
	revIsNull = (revised->to84_via == cs_DTCTYP_WGS84  ||
			   revised->to84_via == cs_DTCTYP_NAD83  ||
			   revised->to84_via == cs_DTCTYP_WGS84  ||
			   revised->to84_via == cs_DTCTYP_GDA94  ||
			   revised->to84_via == cs_DTCTYP_NZGD2K ||
			   revised->to84_via == cs_DTCTYP_ETRF89 ||
				 revised->to84_via == cs_DTCTYP_RGF93  ||
				 (revDeltaZeroCount == 3 &&
					(revised->to84_via == cs_DTCTYP_GEOCTR || revised->to84_via == cs_DTCTYP_3PARM)
				 )
				);

	/* We can now compare the type giving proper consideration to the fact that
	   there are numerous ways of defining the null transformation.  That is, if
	   both original and revised are the null transformation, we  */
	if (orgIsNull && revIsNull)
		{
		/* Both are the null transformation, so they are essentially
		   equivalent. */
		return errCnt;
	}

	/* Compare the transformation technique. */
	if (original->to84_via != revised->to84_via)
	{
		if (errCnt == 0)			/*lint !e774   boolean always evaluates to True (correct in this instance) */
		{
			sprintf (errMsg,"Datum transformation method on datum named %s does not match.  Method was %d, is now %d",
							original->key_nm,
							original->to84_via,
							revised->to84_via);
		}
		
		/* We bump the error count by 8 to make this error appear to be more
		   severe than other types of errors. */
		errCnt += 8;
	}
	else
	{
		/* If the transformation technique is the same, it makes sense to compare
		   the parameters; if the transformation is of the type that uses
		   parameters, of course. */
		switch (original->to84_via) {
		case cs_DTCTYP_MOLO:    paramUseCount = 3; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_MREG:    paramUseCount = 7; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_BURS:    paramUseCount = 7; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_NAD27:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_NAD83:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_WGS84:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_WGS72:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_HPGN:    paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_7PARM:   paramUseCount = 7; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_AGD66:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_3PARM:   paramUseCount = 3; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_6PARM:   paramUseCount = 6; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_4PARM:   paramUseCount = 4; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_AGD84:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_NZGD49:  paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_ATS77:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_GDA94:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_NZGD2K:  paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_CSRS:    paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_TOKYO:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_RGF93:   paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_ED50:    paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_DHDN:    paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_ETRF89:  paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_GEOCTR:  paramUseCount = 3; ellipsoidUse =  TRUE; break;
		case cs_DTCTYP_CHENYX:  paramUseCount = 0; ellipsoidUse = FALSE; break;
		case cs_DTCTYP_NONE:
		default:
			paramUseCount = -1;
			ellipsoidUse = FALSE;
			break;
		}
	}

	/* Check the parameters as is appropriate. */

	if (paramUseCount > 0)
	{
		if (fabs (original->delta_X - revised->delta_X) > deltaEpsilon)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"%s: Delta X was %12.3f, is now %12.3f",original->key_nm,original->delta_X,revised->delta_X);
			}
			errCnt += 1;
		}
		if (fabs (original->delta_Y - revised->delta_Y) > deltaEpsilon)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"%s: Delta Y was %12.3f, is now %12.3f",original->key_nm,original->delta_Y,revised->delta_Y);
			}
			errCnt += 1;
		}
		if (fabs (original->delta_Z - revised->delta_Z) > deltaEpsilon)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"%s: Delta Z was %12.3f, is now %12.3f",original->key_nm,original->delta_Z,revised->delta_Z);
			}
			errCnt += 1;
		}
	}
	if (paramUseCount > 4)
		{
		if (fabs (original->rot_X - revised->rot_X)> rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: X Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_X,revised->rot_X);
				}
				errCnt += 1;
			}
		if (fabs (original->rot_Y - revised->rot_Y) > rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Y Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_Y,revised->rot_Y);
				}
				errCnt += 1;
			}
		if (fabs (original->rot_Z - revised->rot_Z) > rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Z Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_Z,revised->rot_Z);
				}
				errCnt += 1;
			}
	}
	if (paramUseCount > 3 && paramUseCount != 6)
	{
		if (fabs (original->bwscale - revised->bwscale) > scaleEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Scale was %12.8f, is now %12.8f",original->key_nm,original->bwscale,revised->bwscale);
				}
				errCnt += 1;
			}
		}

	/* Check the ellipsoid if it matters for the trsansformation technique. */

	if (ellipsoidUse)
	{
		if (!CS_stricmp (original->ell_knm,revised->ell_knm))
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"%s: Ellipsoid was %s, is now %s",original->key_nm,original->ell_knm,revised->ell_knm);
			}
			errCnt += 1;
		}
	}

	/* OK, we're done.  Return results of comparison to the calling module. */
	if (errCnt != 0)
	{
		if (message != 0 && messageSize > 1)
		{
			CS_stncp (message,errMsg,(int)messageSize);
		}
	}
	return errCnt;
}
int EXP_LVL3 CS_elDefCmp (Const struct cs_Eldef_ *original,Const struct cs_Eldef_ *revised,char* message,size_t messageSize)
{
	int errCnt = 0;

	char errMsg [512];

	errMsg [0] = '\0';				/* Defensive programming, also keeps lint happy */
	/* Pretty simple for an ellipsoid. */
	if (fabs (original->e_rad - revised->e_rad) > 6.0E-04)
	{
		if (errCnt == 0)			/*lint !e774   boolean always evaluates to True (correct in this instance) */
		{
			sprintf (errMsg,"%s: Equatorial radius was %14.4f, is now %14.4f",original->key_nm,original->e_rad,revised->e_rad);
		}
		errCnt += 1;
	}
	if (fabs (original->p_rad - revised->p_rad) > 6.0E-04)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"%s: Polar radius was %14.4f, is now %14.4f",original->key_nm,original->p_rad,revised->p_rad);
		}
		errCnt += 1;
	}
	if (fabs (original->flat - revised->flat) > 5.0E-07)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"%s: Flattening was %14.8f, is now %14.8f",original->key_nm,original->flat,revised->flat);
		}
		errCnt += 1;
	}
	if (fabs (original->ecent - revised->ecent) > 5.0E-08)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"%s: Eccentricity was %11.9f, is now %11.9f",original->key_nm,original->ecent,revised->ecent);
		}
		errCnt += 1;
	}
	if (errCnt != 0)
	{
		if (message != 0 && messageSize > 1)
		{
			CS_stncp (message,errMsg,(int)messageSize);
		}
	}
	return errCnt;
}

