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

/* The following functions represent a rewrite of the functions originally
   appearing in CS_defCmp.c.  There is a one to one correpsondence between
   the functions in the original with the functions defined in this module,
   the distinction being:
   1> the functions defined here have an Ex appended to the name, and
   2> the function protypes are slightly different.
   
   The original functions are maintain spearately so as to preserve the
   existing beghavior of all applications which reference the existsing
   functions.  The new versions do a better job of making a mathematical
   comparison, and return a result of the comparison which indicates, in
   meters, what affect the difference may have on calculated results 
   produced by the definitions being compared.  (This quality value, i.e.
   qValue is really just a guess, but can be used by applications to get
   a comparison which is more tolerant than the standard comparison
   function.)
   
   Generally, the protype for each function now includes a pointer to
   a double as the first argument.  If not zero, the functions return
   in the double a value, in meters, which represents a best guess as
   to the difference in the calculated results produced by the two
   different definitions.
   
   If these functions are provided with the NULL pointer for the
   qValue argument, they much much like the orginal function
   counterparts, substantially improved in the cases of datums and
   coordinate systems.
   
   Note, that even in the case where the return value is zero, indicating
   a pretty good match, a qValue will be returned.
*/

/*lint -e774  Boolean within 'if' always evaluates to true (PC-Lint bug) */

int EXP_LVL3 CS_elDefCmpEx (double* qValuePtr,Const struct cs_Eldef_ *original,
											  Const struct cs_Eldef_ *revised,
											  char* message,
											  size_t msgSize)
{
	static const double lat60 = (60.0 * ONE_DEGREE);
	static const double sinLat60 = 0.86602540378443864676372317075294;
	static const double cosLat60 = 0.5;

	int errCnt;

	double e_sq;
	double ddOrg;
	double ddRev;

	struct cs_MmcofF_ mmF;
	char errMsg [256];

	errCnt = 0;
	memset (errMsg,'\0',sizeof (errMsg));
	if (fabs (original->e_rad - revised->e_rad) > 6.0E-04)
	{
		if (errCnt == 0)
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
	if (fabs (original->flat - revised->flat) > 5.0E-05)
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
	if (errCnt != 0 && message != 0 && msgSize > 1 && *errMsg != '\0')
	{
		CS_stncp (message,errMsg,(int)msgSize);
	}

	/* Calculate the meridional arc from the equator to 60 degrees of latitude
	   for each of the ellipsoids.  We return the difference as the quality
	   factor. */
	if (qValuePtr != 0)
	{
		e_sq = original->ecent * original->ecent;
		CSmmFsu (&mmF,original->e_rad,e_sq);
		ddOrg = CSmmFcal (&mmF,lat60,sinLat60,cosLat60);

		e_sq = revised->ecent * revised->ecent;
		CSmmFsu (&mmF,revised->e_rad,e_sq);
		ddRev = CSmmFcal (&mmF,lat60,sinLat60,cosLat60);

		*qValuePtr = fabs (ddOrg - ddRev);
	}	
	return errCnt;
}
/*lint -esym(550,ellipsoidUse)  symbol not referenced. */
/* ellipsoidUse is set to true in the event of a transformation method which
   is dependent upon the ellispoid, implying that the ellipsoid needs to be
   numerically the same for the datums to be numerically the same.  This
   verification is not done in this module for whatever reason (I can think
   of many).  Nevertheless, for now we ignore PC-Lint's reminding us of this
   issue. */
int EXP_LVL3 CS_dtDefCmpEx (double* qValuePtr,Const struct cs_Dtdef_ *original,
											  Const struct cs_Dtdef_ *revised,
											  char* message,
											  size_t msgSize)
{
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Six;
	extern double cs_K15;

	static const double deltaEpsilon = 2.0E-03;
	static const double rotEpsilon = 1.0E-03;
	static double scaleEpsilon = 5.0E-06;

	int orgIsNull;
	int revIsNull;
	int ellipsoidUse = 0;

	short paramUseCount = -1;
	short orgDeltaZeroCount;
	short revDeltaZeroCount;
	short orgVia;
	short revVia;

	int errCnt = 0;

	double delta;
	double qValue;

	char errMsg [256];

	qValue = cs_Zero;			// until we know otherwise
	ellipsoidUse = 0;			// until we know otherwise
	errMsg [0] = '\0';

	/* First we see if both of the datums are the null transformation.  If so
	   we consider them to be the same.  We do not attempt to compare the
	   ellipsoid specification.  Doing so could make this function unusable
	   for many applications as comparing the names of the ellipsoids in the
	   definitions may not be CS-MAP ellipsoid names.

	   The revised will often get converted to a type cs_DTCTYP_GEOCTR with zero
	   deltas if it was one of those types that is generally considered to be
	   equivalent with WGS84.
	   
	   See if the original is one of the variations of a null transform. */
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
					(original->to84_via == cs_DTCTYP_GEOCTR ||
					 original->to84_via == cs_DTCTYP_3PARM  ||
					 original->to84_via == cs_DTCTYP_MOLO)
				 )
			);

	/* Determine if the revised datums is one of the variations of a null transform. */					
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
					(revised->to84_via == cs_DTCTYP_GEOCTR ||
					 revised->to84_via == cs_DTCTYP_3PARM  ||
					 revised->to84_via == cs_DTCTYP_MOLO)
				 )
			);

	/* If both are the null transform, we assume they are the same, and we
	   skip most of the remaining code. */
	if (orgIsNull && revIsNull)
	{
		/* The datums both look like one of the variations used to implement
		   the null transformation.  For this to be valid, in some cases, the
		   ellipsoids need to be the same. */
		if ((original->to84_via == cs_DTCTYP_GEOCTR ||
			 original->to84_via == cs_DTCTYP_3PARM  ||
			 original->to84_via == cs_DTCTYP_MOLO) &&
			(revised->to84_via  == cs_DTCTYP_GEOCTR ||
			 revised->to84_via  == cs_DTCTYP_3PARM  ||
			 revised->to84_via  == cs_DTCTYP_MOLO)
		   )
		{	 
			ellipsoidUse = 1;
		}
	}
	else
	{
		/* Compare the transformation technique.  Several of these are essentially
		   equivalent. */
		orgVia = original->to84_via;
		revVia = revised->to84_via;
		if (orgVia == cs_DTCTYP_3PARM || orgVia == cs_DTCTYP_MOLO) orgVia = cs_DTCTYP_GEOCTR;
		if (revVia == cs_DTCTYP_3PARM || orgVia == cs_DTCTYP_MOLO) revVia = cs_DTCTYP_GEOCTR;
		if ((orgVia == cs_DTCTYP_7PARM && revVia == cs_DTCTYP_BURS) ||
		    (orgVia == cs_DTCTYP_BURS  && revVia == cs_DTCTYP_7PARM))
		{
			/* There is a difference in the technique, but the difference is
			   not that significant.  We boost the qValue a bit to account
			   for this, and we mark this as a single error as opposed to what
			   we do below. */
			revVia = orgVia;
			qValue += cs_One;
			if (errCnt == 0)
			{
				sprintf (errMsg,"Datum transformation method on datum named %s does not match.  Method was %d, is now %d",
								original->key_nm,
								original->to84_via,
								revised->to84_via);
			}
			errCnt += 1;
		}

		/* Convert a "MULREG" on either side to be the same as other side,
		   unless of course, they are both MULREGs.  If they are both MULREG's,
		   the code below will work.  It will compare the parameters associated
		   with the definition and report if they are the same or not.  There's
		   not much else we can do with MULREG's. */
		if (orgVia == cs_DTCTYP_MREG && (revVia == cs_DTCTYP_GEOCTR ||
										 revVia == cs_DTCTYP_3PARM  ||
										 revVia == cs_DTCTYP_MOLO))
		{
			orgVia = revVia;
			qValue += cs_One;
			if (errCnt == 0)
			{
				sprintf (errMsg,"Datum transformation method on datum named %s does not match.  Method was %d, is now %d",
								original->key_nm,
								original->to84_via,
								revised->to84_via);
			}
			errCnt += 1;
		}
		else if (revVia == cs_DTCTYP_MREG && (orgVia == cs_DTCTYP_GEOCTR ||
											  orgVia == cs_DTCTYP_3PARM  ||
											  orgVia == cs_DTCTYP_MOLO))
		{
			revVia = orgVia;
			qValue += cs_One;
			if (errCnt == 0)
			{
				sprintf (errMsg,"Datum transformation method on datum named %s does not match.  Method was %d, is now %d",
								original->key_nm,
								original->to84_via,
								revised->to84_via);
			}
			errCnt += 1;
		}
		else if (orgVia == cs_DTCTYP_MREG && revVia == cs_DTCTYP_MREG)
		{
			/* It could very well be that we don't want to even try in this
			   case.  We don't have access to the MULREG definition in this
			   function, so there is nothing to check other than what the
			   fallback will be.  So, you may want to uncomment the code
			   provided. */
			/*
			  *qValuePtr = cs_Zero;
			  return 0;
			*/
		}

		// OK, see if we have an equivalent method, or at least roughly
		// equivalent method.
		if (orgVia != revVia)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Datum transformation method on datum named %s does not match.  Method was %d, is now %d",
								original->key_nm,
								original->to84_via,
								revised->to84_via);
			}

			/* We bump the error count by 4 to make this error appear to be more
			   significant than other types of errors. */
			errCnt += 4;
			qValue = 50.0;		/* Your typical datum shift is 100 meters, so we
								   use half this value as an estimate. */
		}
		else
		{
			/* If the transformation technique is the same, it makes sense to compare
			   the parameters; if the transformation is of the type that uses
			   parameters, of course. */
			switch (original->to84_via) {
			case cs_DTCTYP_MOLO:    paramUseCount = 3; break;
			// The following case assumes unused parameters are set to zero and
			// therefore will match perfectly.
			case cs_DTCTYP_MREG:    paramUseCount = 7; break;
			case cs_DTCTYP_BURS:    paramUseCount = 7; break;
			case cs_DTCTYP_NAD27:   paramUseCount = 0; break;
			case cs_DTCTYP_NAD83:   paramUseCount = 0; break;
			case cs_DTCTYP_WGS84:   paramUseCount = 0; break;
			case cs_DTCTYP_WGS72:   paramUseCount = 0; break;
			case cs_DTCTYP_HPGN:    paramUseCount = 0; break;
			case cs_DTCTYP_7PARM:   paramUseCount = 7; break;
			case cs_DTCTYP_AGD66:   paramUseCount = 0; break;
			case cs_DTCTYP_3PARM:   paramUseCount = 3; break;
			case cs_DTCTYP_6PARM:   paramUseCount = 6; break;
			case cs_DTCTYP_4PARM:   paramUseCount = 4; break;
			case cs_DTCTYP_AGD84:   paramUseCount = 0; break;
			case cs_DTCTYP_NZGD49:  paramUseCount = 0; break;
			case cs_DTCTYP_ATS77:   paramUseCount = 0; break;
			case cs_DTCTYP_GDA94:   paramUseCount = 0; break;
			case cs_DTCTYP_NZGD2K:  paramUseCount = 0; break;
			case cs_DTCTYP_CSRS:    paramUseCount = 0; break;
			case cs_DTCTYP_TOKYO:   paramUseCount = 0; break;
			case cs_DTCTYP_RGF93:   paramUseCount = 0; break;
			case cs_DTCTYP_ED50:    paramUseCount = 0; break;
			case cs_DTCTYP_DHDN:    paramUseCount = 0; break;
			case cs_DTCTYP_ETRF89:  paramUseCount = 0; break;
			case cs_DTCTYP_GEOCTR:  paramUseCount = 3; break;
			case cs_DTCTYP_CHENYX:  paramUseCount = 0; break;
			case cs_DTCTYP_NONE:
			default:
				paramUseCount = -1;
				ellipsoidUse = FALSE;
				break;
			}
		}

		/* Check the aparameters as is appropriate. */
		if (paramUseCount > 0)
		{
			delta = fabs (original->delta_X - revised->delta_X);
			if (delta > deltaEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Delta X was %12.3f, is now %12.3f",original->key_nm,original->delta_X,revised->delta_X);
				}
				errCnt += 1;
				qValue += delta;
			}
			delta = fabs (original->delta_Y - revised->delta_Y);
			if (delta > deltaEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Delta Y was %12.3f, is now %12.3f",original->key_nm,original->delta_Y,revised->delta_Y);
				}
				errCnt += 1;
				qValue += delta;
			}
			delta = fabs (original->delta_Z - revised->delta_Z);
			if (delta > deltaEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Delta Z was %12.3f, is now %12.3f",original->key_nm,original->delta_Z,revised->delta_Z);
				}
				errCnt += 1;
				qValue += delta;
			}
		}
		if (paramUseCount > 4)
		{
			delta = fabs (original->rot_X - revised->rot_X);
			if (delta > rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: X Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_X,revised->rot_X);
				}
				errCnt += 1;
				qValue += delta * cs_K15;
			}
			delta = fabs (original->rot_Y - revised->rot_Y);
			if (delta > rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Y Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_Y,revised->rot_Y);
				}
				errCnt += 1;
				qValue += delta * cs_K15;
			}
			delta = fabs (original->rot_Z - revised->rot_Z);
			if (delta > rotEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Z Rotation was %12.3f, is now %12.3f",original->key_nm,original->rot_Z,revised->rot_Z);
				}
				errCnt += 1;
				qValue += delta * cs_K15;
			}
		}
		if (paramUseCount > 3 && paramUseCount != 6)
		{
			delta = fabs (original->bwscale - revised->bwscale);
			if (delta > scaleEpsilon)
			{
				if (errCnt == 0)
				{
					sprintf (errMsg,"%s: Scale was %12.8f, is now %12.8f",original->key_nm,original->bwscale,revised->bwscale);
				}
				errCnt += 1;
				qValue += delta * cs_Six;
			}
		}
	}

	/* OK, we're done.  Return results of comparison to the calling module. */
	if (errCnt != 0 && message != 0 && msgSize > 1 && *errMsg != '\0')
	{
		CS_stncp (message,errMsg,(int)msgSize);
	}
	if (qValuePtr != 0)
	{
		*qValuePtr = qValue;
	}
	return errCnt;
}
/*lint +esym(550,ellipsoidUse) */

int EXP_LVL3 CS_csDefCmpEx (double *qValuePtr,Const struct cs_Csdef_ *original,Const struct cs_Csdef_ *revised,char* message,size_t msgSize)
{
	extern struct cs_Prjtab_ cs_Prjtab [];		/* Projection Table */
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Six;
	extern double cs_Huge;			/* The quality value we return in the event of a software error. */

	short prmNbr;
	short unitType;

	int errCnt = 0;
	int prmErrCnt;

	struct cs_Prjtab_ *ppOrg;
	struct cs_Prjtab_ *ppRev;
	struct cs_Csdef_* csDefPtr;

	double delta;
	double swpDbl;
	double qValue;
	double qFactor;
	double unitsFactor;
	double unitsFactor1;
	double tolerance;
	double prmQValue;

	char errMsg [512];
	char prmErrMsg [256];

	struct cs_Csdef_ lclOriginal;
	struct cs_Csdef_ lclRevised;

	errMsg [0] = '\0';
	qValue = cs_Zero;

	/* Before we get onto this to heavy; we get copies of the two definitions
	   which we can modify.  In several cases, there are variations of
	   projections and parameters which are identical.  What shall do here
	   is modify our copies in those special cases as is appropriate so that
	   the actual comparison code is rational and straight forward.  */
	memcpy (&lclOriginal,original,sizeof (lclOriginal));
	memcpy (&lclRevised,revised,sizeof (lclRevised));

	/* Again, to preserve our sanity, we obtain the projection codes of the
	   the definitions.  We'll need at least one of them, so this is not all
	   wasted effort. */
	for (ppOrg = cs_Prjtab;ppOrg->key_nm [0] != '\0';ppOrg += 1)
	{
		if (!CS_stricmp (lclOriginal.prj_knm,ppOrg->key_nm))
		{
			break;
		}
	}
	for (ppRev = cs_Prjtab;ppRev->key_nm [0] != '\0';ppRev += 1)
	{
		if (!CS_stricmp (lclRevised.prj_knm,ppRev->key_nm))
		{
			break;
		}
	}
	if (ppOrg->code == cs_PRJCOD_END)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"Projection key name was %s which is invalid.",revised->prj_knm);
		}
		qValue = cs_Huge;
		errCnt += 1;
	}
	if (ppRev->code == cs_PRJCOD_END)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"Projection key name is now %s which is invalid.",revised->prj_knm);
		}
		qValue = cs_Huge;
		errCnt += 1;
	}
	if (errCnt != 0)
	{
		/* The calling module has not supplied use with useful definitions.
		   Bag it now. */
		goto error;
	}

	/* We now apply legitimate changes to the definitions such that
	   "mathematically" identical definitions will produce a
	   successful match in the code below. */
	csDefPtr = 0;
	if (ppOrg->code == cs_PRJCOD_UTM && ppRev->code == cs_PRJCOD_TRMER)
	{
		csDefPtr = &lclOriginal;
	}
	else if (ppOrg->code == cs_PRJCOD_TRMER && ppRev->code == cs_PRJCOD_UTM)
	{
		csDefPtr = &lclRevised;
	}
	if (csDefPtr != 0)
	{
		/* csDefPtr points to a UTM definition which is now converted to
		   a standard Transverse Mercator form. */
		strcpy (csDefPtr->prj_knm,"TM");
		csDefPtr->prj_prm1 = (double)(((int)csDefPtr->prj_prm1 * 6) - 183);	/*lint !e790 */
		csDefPtr->org_lng = csDefPtr->prj_prm1;
		csDefPtr->org_lat = cs_Zero;
		unitsFactor = CS_unitlu (cs_UTYP_LEN,csDefPtr->unit);
		csDefPtr->x_off = 500000.0 / unitsFactor;
		csDefPtr->y_off = (csDefPtr->prj_prm2 >= 0.0) ? cs_Zero : 10000000.0 / unitsFactor;
		csDefPtr->scl_red = 0.9996;
		csDefPtr->quad = 1;
	}

	/* If either definition is a conic with two standard parallels, make the
	   northern most parallel orj_prm1 in either or both cases. */
	if (ppOrg->code == cs_PRJCOD_LM2SP   ||
		ppOrg->code == cs_PRJCOD_LMBLG   ||
		ppOrg->code == cs_PRJCOD_WCCSL   ||
		ppOrg->code == cs_PRJCOD_MNDOTL  ||
		ppOrg->code == cs_PRJCOD_ALBER   ||
		ppOrg->code == cs_PRJCOD_LMBRTAF ||
		ppOrg->code == cs_PRJCOD_LMMICH)
	{
		if (lclOriginal.prj_prm1 < lclOriginal.prj_prm2)
		{
			swpDbl = lclOriginal.prj_prm2;
			lclOriginal.prj_prm2 = lclOriginal.prj_prm1;
			lclOriginal.prj_prm1 = swpDbl;
		}
	}
	if (ppRev->code == cs_PRJCOD_LM2SP   ||
		ppRev->code == cs_PRJCOD_LMBLG   ||
		ppRev->code == cs_PRJCOD_WCCSL   ||
		ppRev->code == cs_PRJCOD_MNDOTL  ||
		ppRev->code == cs_PRJCOD_ALBER   ||
		ppRev->code == cs_PRJCOD_LMBRTAF ||
		ppRev->code == cs_PRJCOD_LMMICH)
	{
		if (lclRevised.prj_prm1 < lclRevised.prj_prm2)
		{
			swpDbl = lclRevised.prj_prm2;
			lclRevised.prj_prm2 = lclRevised.prj_prm1;
			lclRevised.prj_prm1 = swpDbl;
		}
	}

	/* We consider the two different formulations of the Gauss Kruger to
	   be the same. */
	if (ppOrg->code == cs_PRJCOD_TRMER && ppRev->code == cs_PRJCOD_TRMRKRG)
	{
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
	}
	if (ppOrg->code == cs_PRJCOD_TRMRKRG && ppRev->code == cs_PRJCOD_TRMER)
	{
		CS_stncp (lclOriginal.prj_knm,lclRevised.prj_knm,sizeof (lclRevised.prj_knm));
	}

	/* Gauss Kruger is essentially a Transverse Mercator with a scale
	   reduction factor of 1.0. */
	if (ppOrg->code == cs_PRJCOD_TRMER && CS_cmpDbls (lclOriginal.scl_red,cs_One) && ppRev->code == cs_PRJCOD_GAUSSK)
	{
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
		lclRevised.scl_red = cs_One;
	}
	if (ppRev->code == cs_PRJCOD_TRMER && CS_cmpDbls(lclRevised.scl_red,cs_One) && ppOrg->code == cs_PRJCOD_GAUSSK)
	{
		CS_stncp (lclOriginal.prj_knm,lclRevised.prj_knm,sizeof (lclOriginal.prj_knm));
		lclOriginal.scl_red = cs_One;
	}

	/* If one of the definitions is LMSP1 and the other is LMTAN, make them
	   the same. */
	if ((ppOrg->code == cs_PRJCOD_LMTAN && ppRev->code == cs_PRJCOD_LM1SP) ||
		(ppOrg->code == cs_PRJCOD_LM1SP && ppRev->code == cs_PRJCOD_LMTAN))
	{
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
	}
	
	/* Check for standard Transverse Mercator with the appropriate quad value
	   and a South Oriented Transverse Mercator. */
	if ((ppOrg->code == cs_PRJCOD_TRMER && lclOriginal.quad == 3 && ppRev->code == cs_PRJCOD_SOTRM) ||
		(ppRev->code == cs_PRJCOD_TRMER && lclRevised.quad  == 3 && ppOrg->code == cs_PRJCOD_SOTRM))
	{
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
		lclRevised.quad = lclOriginal.quad;
	}
	
	/* The MRCAT and MRCATK are often simply different parameterizations of the
	   same thing. */
	if ((ppOrg->code == cs_PRJCOD_MRCAT && ppRev->code == cs_PRJCOD_MRCATK) ||
	    (ppRev->code == cs_PRJCOD_MRCAT && ppOrg->code == cs_PRJCOD_MRCATK))
	{
		/* TODO: We should use the CSmrcatPhiFromK function to compare the
		   standard latitude of the MRCAT variation with the scale factor
		   of the MRCATK variation.
		   
		   For now, we assume them to be the same. */
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
		lclRevised.scl_red = lclOriginal.scl_red;
		lclRevised.prj_prm1 = lclOriginal.prj_prm1;
	}
	
	/* The Swiss Cylindircal and the Oblique Cylindrical are often
	   approximated by the Oblique Mercator.  We deal with this here. */
	if ((ppOrg->code == cs_PRJCOD_HOM1XY && fabs (lclOriginal.prj_prm3) > 89.9) &&
		(ppRev->code == cs_PRJCOD_SWISS || ppRev->code == cs_PRJCOD_OBQCYL))
	{
		/* Convert the original to the same type as the revised, keeping all the
		   parameters of the original for comparison purposes. */
		CS_stncp (lclOriginal.prj_knm,lclRevised.prj_knm,sizeof (lclOriginal.prj_knm));
		lclOriginal.org_lng = lclOriginal.prj_prm1;
		lclOriginal.org_lat = lclOriginal.prj_prm2;
		if (ppRev->code == cs_PRJCOD_SWISS)
		{
			lclOriginal.prj_prm1 = cs_Zero;
		}
		else if (ppRev->code == cs_PRJCOD_OBQCYL)
		{
			lclOriginal.prj_prm1 = lclRevised.prj_prm1;
		}
		lclOriginal.prj_prm2 = cs_Zero;
		lclOriginal.prj_prm3 = cs_Zero;
	}
	if ((ppRev->code == cs_PRJCOD_HOM1XY && fabs (lclRevised.prj_prm3) > 89.9) &&
		(ppOrg->code == cs_PRJCOD_SWISS || ppOrg->code == cs_PRJCOD_OBQCYL))
	{
		/* Convert the original to the same type as the revised, keeping all the
		   parameters of the original for comparison purposes. */
		CS_stncp (lclRevised.prj_knm,lclOriginal.prj_knm,sizeof (lclRevised.prj_knm));
		lclRevised.org_lng = lclRevised.prj_prm1;
		lclRevised.org_lat = lclRevised.prj_prm2;
		if (ppOrg->code == cs_PRJCOD_SWISS)
		{
			lclRevised.prj_prm1 = cs_Zero;
		}
		else if (ppOrg->code == cs_PRJCOD_OBQCYL)
		{
			lclRevised.prj_prm1 = lclOriginal.prj_prm1;		// This is an approximation!!!!!
		}
		lclRevised.prj_prm2 = cs_Zero;
		lclRevised.prj_prm3 = cs_Zero;
	}

	/* Adjust for any changes to the projection code we may have made above. */
	for (ppOrg = cs_Prjtab;ppOrg->key_nm [0] != '\0';ppOrg += 1)
	{
		if (!CS_stricmp (lclOriginal.prj_knm,ppOrg->key_nm))
		{
			break;
		}
	}
	for (ppRev = cs_Prjtab;ppRev->key_nm [0] != '\0';ppRev += 1)
	{
		if (!CS_stricmp (lclRevised.prj_knm,ppRev->key_nm))
		{
			break;
		}
	}

	/* OK.  We've handled all of the variations which are identical; at least
	   the ones we know about as of now.  Now we do the comparison in a
	   rational manner which can be understood. */
	if (ppOrg->code != ppRev->code)
	{
		/* The projection codes do not match, so there is a significant
		   difference. For certain differences, we use a lower qValue than the
		   general case. */
		if ((ppOrg->code == cs_PRJCOD_TRMER && ppRev->code == cs_PRJCOD_TRMRS) ||
			(ppOrg->code == cs_PRJCOD_TRMRS && ppRev->code == cs_PRJCOD_TRMER)
		   )
		{
			qValue = 0.05;
		}
		if ((ppOrg->code == cs_PRJCOD_OSTRO && ppRev->code == cs_PRJCOD_SSTRO) ||
			(ppOrg->code == cs_PRJCOD_SSTRO && ppRev->code == cs_PRJCOD_OSTRO)
		   )
		{
			qValue = cs_Six;
		}
		else
		{
			qValue = cs_Huge;
		}
		if (errCnt == 0)
		{
			sprintf (errMsg,"Projection key name was %s, is now %s",lclOriginal.prj_knm,lclRevised.prj_knm);
		}
		errCnt += 1;
	}

	if (ppOrg->code == cs_PRJCOD_UNITY)
	{
		unitType = cs_UTYP_ANG;
		tolerance = 0.0001;
		qFactor = 111000.0;
	}
	else
	{
		unitType = cs_UTYP_LEN;
		tolerance = 0.0000001;
		qFactor = 100000.0;
	}
	unitsFactor  = CS_unitlu (unitType,lclOriginal.unit);
	unitsFactor1 = CS_unitlu (unitType,lclRevised.unit);
	delta = fabs (unitsFactor - unitsFactor1);
	if (delta > tolerance)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"Unit name was %s, is now %s",lclOriginal.unit,lclRevised.unit);
		}
		qValue += delta * qFactor;
		errCnt += 1;
	}

	/* If the projection codes or units don't match, we're in deep do-do.  So,
	   we bag it now if we haven't got a match so far. */
	if (errCnt != 0)
	{
		goto error;
	}

	/* Check all of the parameters. */
	if (errCnt == 0) errMsg[0] = '\0';

	/* Compare all of the projection parameters. */
	for (prmNbr = 1;prmNbr <= 24;prmNbr += 1)
	{
		if (ppOrg->code == cs_PRJCOD_UNITY && prmNbr <= 2)
		{
			/* We don't compare the first twoparameters of a geographic
			   definition as CS-MAP is the only vendor which uses these
			   values. */
			continue;
		}


		prmQValue = cs_Zero;
		prmErrMsg [0] = '\0';
		prmErrCnt = CSprjPrmCmp (&prmQValue,prmNbr,&lclOriginal,&lclRevised,prmErrMsg,sizeof (prmErrMsg));
		if (prmErrCnt != 0)
		{
			if (errCnt == 0)
			{
				CS_stncp (errMsg,prmErrMsg,sizeof (errMsg));
			}
			errCnt += prmErrCnt;
			qValue += prmQValue;
		}
	}

	if ((ppOrg->flags & cs_PRJFLG_ORGLAT) == 0)
	{
		delta = fabs (lclOriginal.org_lat - lclRevised.org_lat);
		if (delta > 1.0E-07)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Origin Latitude was %14.8f, is now %14.8f",lclOriginal.org_lat,lclRevised.org_lat);
			}
			errCnt += 1;
			qValue += delta * 111000.000;
		}
	}
	if ((ppOrg->flags & cs_PRJFLG_ORGLNG) == 0)
	{
		delta = fabs (lclOriginal.org_lng - lclRevised.org_lng);
		if (delta > 1.0E-07)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Origin Longitude was %14.8f, is now %14.8f",lclOriginal.org_lng,lclRevised.org_lng);
			}
			errCnt += 1;
			qValue += delta * 111000.000;
		}
	}
	if ((ppOrg->flags & cs_PRJFLG_ORGFLS) == 0)
	{
		delta = fabs (lclOriginal.x_off - lclRevised.x_off) * unitsFactor;
		if (delta > 0.008)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"False easting was %14.3f, is now %14.3f",lclOriginal.x_off,lclRevised.x_off);
			}
			errCnt += 1;
			qValue += delta;
		}
		delta = fabs (lclOriginal.y_off - lclRevised.y_off) * unitsFactor;
		if (delta > 0.008)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"False northing was %14.3f, is now %14.3f",lclOriginal.y_off,lclRevised.y_off);
			}
			errCnt += 1;
			qValue += delta;
		}
	}
	if ((ppOrg->flags & cs_PRJFLG_SCLRED) != 0)
	{
		delta = fabs (lclOriginal.scl_red - lclRevised.scl_red);
		if (delta > 1.0E-08)
		{
			if (errCnt == 0)
			{
				sprintf (errMsg,"Scale reduction was %12.10f, is now %12.10f",lclOriginal.scl_red,lclRevised.scl_red);
			}
			errCnt += 1;
			qValue = delta * 1000000.0;
		}
	}
	if (lclOriginal.quad != lclRevised.quad)
	{
		if (errCnt == 0)
		{
			sprintf (errMsg,"Quad was %d, is now %d",lclOriginal.quad,lclRevised.quad);
		}
		errCnt += 1;
		qValue = cs_Huge;
	}

error:
	if (errCnt != 0 && message != 0 && msgSize > 1 && *message != '\0' && errMsg [0] != '\0')
	{
		CS_stncp (message,errMsg,(int)msgSize);
	}
	if (qValuePtr != NULL)
	{
		*qValuePtr = qValue;
	}
	return errCnt;
}

/* The following compares one of the parameters in two different coordinate
   system definitions assumed to be of the same projection (and variation).
   It returns a TRUE value if the two parameters are within a tolerance which
   varied by the kind (i.e. logical type) of parameter.  Further, it returns
   a double which provides an indication (a rough guess) as to the degree
   coordinate calculations will differ because of the parameter difference.
   This value is provided in meters, and is calculated and returned regardless
   of the return value.
   
   If and only if the return value is FALSE, this function will place in
   message a textual (English only) description of the parameter and the
   difference between the two, suitable for reporting to an end user.  This
   function will +NOT+ under any circustances modify or overwrite an existing
   string in the provided messge array. */
int EXP_LVL3 CSprjPrmCmp (double* qValuePtr,short prmNbr,Const struct cs_Csdef_* original,
														 Const struct cs_Csdef_* revised,
														 char* message,
														 size_t msgSize)
{
	extern double cs_Zero;
	extern double cs_Huge;
	extern struct cs_Prjtab_ cs_Prjtab [];
	extern struct cs_Prjprm_ csPrjprm [];
	extern struct cs_PrjprmMap_ cs_PrjprmMap [];

	int errCnt;
	unsigned char parmType;
	short prjCodeOrg;
	short prjCodeRev;

	struct cs_Prjtab_ *ppPtr;
	struct cs_Prjprm_ *prmPtr;
	struct cs_PrjprmMap_ *mapPtr;

	double orgValue;
	double revValue;
	double delta;
	double qFactor;
	double qValue;
	double tolerance;

	char errMsg [512];

	/* Until we have verified all of the imput information, we set up to
	   return an error condition. */
	prjCodeOrg = cs_PRJCOD_END;
	prjCodeRev = cs_PRJCOD_END;
	parmType = cs_PRMCOD_INVLD;
	errCnt = 1;
	qValue = cs_Huge;
	CS_stncp (errMsg,"Internal software error.",sizeof (errMsg));

	/* Get the projection type(s). */
	for (ppPtr = cs_Prjtab;ppPtr->code != cs_PRJCOD_END;ppPtr += 1)
	{
		if (!CS_stricmp (original->prj_knm,ppPtr->key_nm))
		{
			prjCodeOrg = ppPtr->code;
		}
		if (!CS_stricmp (revised->prj_knm,ppPtr->key_nm))
		{
			prjCodeRev = ppPtr->code;
		}
	}

	if (prmNbr >= 1 && prmNbr <= 24 &&
	    prjCodeOrg != cs_PRJCOD_END &&
	    prjCodeOrg == prjCodeRev)
	{
		/* Get the type of parameter. */
		for (mapPtr = cs_PrjprmMap;mapPtr->prj_code != cs_PRJCOD_END;mapPtr += 1)
		{
			if (mapPtr->prj_code == prjCodeOrg)
			{
				parmType = mapPtr->prm_types [prmNbr - 1];
				break;
			}
		}
		if (mapPtr->prj_code == cs_PRJCOD_END)
		{
			CS_stncp (errMsg,"Projection code did not map.",sizeof (errMsg));
		}
	}

	if (parmType != cs_PRMCOD_INVLD)
	{
		/* All of the above is OK.  so we switch our default return values
		   to OK, from total failure. */
		qValue = cs_Zero;
		errCnt = 0;
		
		/* See if this parameter is used. */
		if (parmType != cs_PRMCOD_NOTUSED)
		{
			/* Get a pointer to the parameter table, extract the logical type
			   of the parameter, and then use the logical type to determine a
			   tolerance value for the following comparison.  qFactor is a
			   value which converts the difference between the two values into
			   a value resembling meters for calculation of the qValue element
			   of the results structure.
			   
			   Currently, we use thelogical type to determine a tolerance and
			   the qFactor value.  A better result cou;d possibly be obtained
			   by using the actual parmType itself, rather than the locigal
			   type from the parameter type table. */
			prmPtr = &csPrjprm [parmType];
			switch (prmPtr->log_type) {
			case cs_PRMLTYP_LNG:
			case cs_PRMLTYP_LAT:
				tolerance = 1.0E-07;		/* Value is degrees */
				qFactor = 111000.0;			/* One degree of latitude is worth about 111,000 meters */
				break;
			case cs_PRMLTYP_AZM:
				tolerance = 1.0E-04;		/* Value is degrees */
				qFactor = 1000.0;
				break;
			case cs_PRMLTYP_ANGD:
				tolerance = 1.0E-06;		/* Value is degrees */
				qFactor = 111000.0;			/* One degree is worth about 111,000 meters */
				break;
			case cs_PRMLTYP_CMPLXC:
				tolerance = 1.0E-07;		/* Value is unitless, but they are usually normalized */
				qFactor = 1000.0;			/* No way to know for sure, this value is rather arbitrary. */
				break;
			case cs_PRMLTYP_ZNBR:
				tolerance = 0.4;			/* UTM zone number */
				qFactor = 666000.0;			/* a zone is worth 6 degrees */
				break;
			case cs_PRMLTYP_HSNS:
				tolerance = 0.01;			/* UTM Hemmisphere, really oa sign switch boolean */
				qFactor = 10E+06;			/* A hemisphere is worth about 10 million meters */		
				break;
			case cs_PRMLTYP_GHGT:
				tolerance = 1.0E-02;		/* geoid heights are always in meters */
				qFactor = 1.0;
				break;
			case cs_PRMLTYP_ELEV:
				tolerance = 1.0E-02;		/* elevatations are always in system units */
				qFactor = original->unit_scl;
				break;
			case cs_PRMLTYP_AFCOEF:
				/* The first two affine coefficients usually translation values,
				   and therefore will be in system units.  The remaining
				   coefficients are terms which include, for example, a scale
				   factor and the sine/cosine of a rotation angle. */
				tolerance = 1.0E-08;
				if (parmType == cs_PRMCOD_AFA0 || parmType == cs_PRMCOD_AFB0)
				{
					qFactor = original->unit_scl;
				}
				else
				{
					qFactor = 10000.0;		/* I picked this value right out of
											   the air. */
				}
				break;
			case cs_PRMLTYP_XYCRD:
				tolerance = 1.0E-02;
				qFactor = original->unit_scl;
				break;
			case cs_PRMLTYP_SCALE:
				tolerance = 1.0E-06;
				qFactor = 10000.0;
				break;
			case cs_PRMLTYP_NONE:
			default:
				tolerance = 1.0E-12;
				qFactor = 10000.0;
				break;
			}

			/* Do a generic compare of the values and then report any problem. */
			orgValue = CS_prmValue (original,prmNbr);
			revValue = CS_prmValue (revised,prmNbr);
			delta = fabs (orgValue - revValue);
			if (delta >= tolerance)
			{
				/* they didn't match. */
				qValue += delta * qFactor; 
				sprintf (errMsg,"%s was %16.8f, is now %18.8f",prmPtr->label,orgValue,revValue);
				errCnt += 1;
			}
		}
	}
	
	/* Rertun a description of the cause of the difference, if there was a
	   significant discrepancy.  Note, we don't overwrite a message which
	   may already be in ther user's buffer. */
	if (message != NULL && msgSize > 1 && *message == '\0' && errCnt > 0)
	{
		CS_stncp (message,errMsg,(int)msgSize);
	}
	if (qValuePtr != NULL)
	{
		*qValuePtr = qValue;
	}
	return errCnt;
}
