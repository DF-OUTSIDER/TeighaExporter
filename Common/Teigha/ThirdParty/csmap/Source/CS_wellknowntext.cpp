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

// Now comes May 21, 2014 
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.
//lint -esym(766,..\Include\cs_wkt.h)			Disable PC-Lint's warning of unreferenced headers 
//lint -e571     suspicious cast

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

#include "cs_Legacy.h"

#	include <math.h>

extern "C" const double cs_Zero;
extern "C" const double cs_One;
extern "C" const double cs_K180;
extern "C" const double cs_K90;
extern "C" const double cs_Km180;
extern "C" const double cs_Km90;

extern "C" const double cs_Wgs84_aa;
extern "C" const double cs_Wgs84_ff;
extern "C" const double cs_Wgs84_esq;

extern "C" double cs_Degree;			/* 1.0 / 57.29577.... */
extern "C" double cs_Radian;			/* 57.29577..... */
extern "C" const unsigned long KcsNmMapNoNumber;
extern "C" const unsigned long KcsNmInvNumber;

extern "C" int cs_Errno;
extern "C" char csErrnam [MAXPATH];

extern "C" struct cs_Prjprm_ csPrjprm [];
extern "C" struct cs_Prjtab_ cs_Prjtab [];
extern "C" struct cs_PrjprmMap_ cs_PrjprmMap [];

extern TrcWktEleTypeMap KrcWktEleTypeMap [];
extern TrcWktAxisValueMap KrcWktAxisValueMap [];
extern "C" csWktPrjNameMap_ csWktPrjNameMap [];
extern TrcWktPrmNameMap KrcWktPrmNameMap [];

extern "C" const struct csKeyNmRed_ csAcronyms [];
extern "C" const struct csKeyNmRed_ csElReduce [];
extern "C" const struct csKeyNmRed_ csDtReduce [];
extern "C" const struct csKeyNmRed_ csCsReduce [];

extern "C" csFILE* csDiagnostic;

int CS_wktToCsEx (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const char *wellKnownText);
int CSwellKnownTextKrovak (struct cs_Csdef_* cs_def,struct cs_Eldef_ *el_def,const TrcWktElement* wktElement,
																			 double primeMeridian,
																			 double geogcsUnitsFactor,
																			 ErcWktFlavor flavor);
void CSwktConicAdj (struct cs_Csdef_ *csDef);

// The following function is used to compare a WKT ellipsoid definition as
// provided by the elDef argument, with an MSI dictionary definition indicated
// by the elKeyNamePtr argument.  Returns true if the two ellipsoids are
// numerically equivalent.
int CSelDictWktCompare (const char* elKeyNamePtr,const struct cs_Eldef_ *wktElDef)
{
	bool same = false;
	struct cs_Eldef_ *msiElDef;

	msiElDef = CS_eldef (elKeyNamePtr);
	if (msiElDef != 0)
	{
		same = (fabs (msiElDef->e_rad - wktElDef->e_rad) < 5.0E-03) &&
			(fabs (msiElDef->p_rad - wktElDef->p_rad) < 5.0E-03);

		CS_free(msiElDef);
		msiElDef=NULL;
	}
	return same ? 1 : 0;
}
void CS_wktUnitNameFix (char *unitName,size_t rsltSize,ErcWktFlavor flavor,const char *wktName)
{
	enum EcsMapSt csMapSt = csMapNoMatch;
	enum EcsNameFlavor nmFlavor;

    if (wktName != NULL && *wktName != '\0')
    {
		nmFlavor = csWktFlvrToCsMapFlvr (flavor);
		csMapSt = csMapNameToNameC (csMapUnitKeyName,unitName,rsltSize,csMapFlvrAutodesk,
																	   nmFlavor,
																	   wktName);
    }
	if (csMapSt != csMapOk)
	{
		CS_stncp (unitName,"<unknown>",static_cast<int>(rsltSize));
	}
}
//newPage//
// Function to reduce a keyname by appying the provided table.
short CS_wktReduceKeyNm (char *result,size_t rsltSize,const char *source,const struct csKeyNmRed_ *reduceTbl)
{
	short esriType = FALSE;
	short spCount = 0;
	short usCount = 0;
	size_t charsLeft;
	char *cp;
	char *cpRpl;
	const struct csKeyNmRed_ *tblPtr;
	char lclSource [128];
	char lclResult [256];

	// Get a copy of the source string in a place where we can dink with it.
	CS_stncp (lclSource,source,sizeof (lclSource));

	// Count the underscores and spaces in the source.
	for (cp = lclSource;*cp != '\0';cp += 1)
	{
		if (*cp == ' ') spCount += 1;
		if (*cp == '_') usCount += 1;
	}
	esriType = (spCount == 0) && (usCount > 0);

	// If it is an esri type, replace all the underscores with spaces.
	if (esriType)
	{
		for (cp = lclSource;*cp != '\0';cp += 1)
		{
			if (*cp == '_') *cp = ' ';
		}
	}

	// For each entry in the reduce table, we see if our lclSource contains an
	// equivalent sub-string.
	cpRpl = NULL;
	for (tblPtr = reduceTbl;*(tblPtr->original) != '\0';tblPtr += 1)
	{
		cpRpl = (char *)CS_stristr (lclSource,tblPtr->original);
		if (cpRpl != NULL) break;
	}
	if (cpRpl != NULL)
	{
		*cpRpl = '\0';
		cpRpl += strlen (tblPtr->original);

		cp = CS_stncp (lclResult,lclSource,sizeof (lclResult));
		charsLeft = sizeof (lclResult) - strlen (lclResult);
		cp = CS_stncp (cp,tblPtr->replace,static_cast<int>(charsLeft));
		charsLeft = sizeof (lclResult) - strlen (lclResult);
		CS_stncp (cp,cpRpl,static_cast<int>(charsLeft));
	}
	else
	{
		CS_stncp (lclResult,lclSource,sizeof (lclResult));
	}
	CS_stncp (result,lclResult,static_cast<int>(rsltSize));
	return (cpRpl != NULL);
}
// Function processes WKT ellipsoid key names before passing on to CS-MAP.
void CS_wktElNameFix (char *ellipsoidName,size_t rsltSize,const char *srcName)
{
	char reducedName [128];

	CS_wktReduceKeyNm (reducedName,sizeof (reducedName),srcName,csAcronyms);		//lint !e534   ignoring return value
	CS_wktReduceKeyNm (reducedName,sizeof (reducedName),reducedName,csElReduce);	//lint !e534   ignoring return value
	CS_stncp (ellipsoidName,reducedName,static_cast<int>(rsltSize));
}
// Function processes WKT datum key names before passing on to CS-MAP.
void CS_wktDtNameFix (char *datumName,size_t rsltSize,const char *srcName)
{
	char reducedName [128];

	if (*srcName == 'D' && *(srcName + 1) == '_') srcName += 2;
	CS_wktReduceKeyNm (reducedName,sizeof (reducedName),srcName,csAcronyms);		//lint !e534   ignoring return value
	CS_wktReduceKeyNm (reducedName,sizeof (reducedName),reducedName,csDtReduce);	//lint !e534   ignoring return value
	CS_stncp (datumName,reducedName,static_cast<int>(rsltSize));
}
// Function processes WKT datum key names before passing on to CS-MAP.
void CS_wktCsNameFix (char *csysName,size_t rsltSize,const char *srcName)
{
	char reducedName [128];

	CS_stncp (reducedName,srcName,sizeof (reducedName));
	while (CS_wktReduceKeyNm (reducedName,sizeof (reducedName),reducedName,csAcronyms));	//lint !e722   suspicious use of ';'
	while (CS_wktReduceKeyNm (reducedName,sizeof (reducedName),reducedName,csCsReduce));	//lint !e722   suspicious use of ';'
	CS_stncp (csysName,reducedName,static_cast<int>(rsltSize));
}
unsigned short CS_wktProjLookUp (ErcWktFlavor flavor,const char *wktName)
{
	enum EcsMapSt csMapSt;
	enum EcsNameFlavor csNmFlvr;
	unsigned short projCode = cs_PRJCOD_END;
	struct cs_Prjtab_ *pp;
	char projKeyName [64];

	// Special kludge for Oracle.  In Oracle 10g, they used the EPSG OP code
	// (EPSG Operation COde, NoT the projection code) to specify the
	// projection.  So, if the flavor is Oracle, we see if the projection
	// name maps via the op code route.
	if (flavor == wktFlvrOracle)
	{
		long32_t tmpCode = CSepsgOpCodeMap (wktName,flavor);
		if (tmpCode != 0)
		{
			projCode = static_cast<unsigned short>(tmpCode);
		}
	}

	if (projCode == cs_PRJCOD_END)
	{
		// The above didn't work, so we try the ordinary stuff.
		csNmFlvr = csWktFlvrToCsMapFlvr (flavor);
		csMapSt = csMapNameToNameC (csMapProjectionKeyName,projKeyName,
														   sizeof (projKeyName),
														   csMapFlvrCsMap,
														   csNmFlvr,
														   wktName);
		if (csMapSt == csMapOk)
		{
			for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
			{
				if (!CS_stricmp (pp->key_nm,projKeyName))
				{
					projCode = pp->code;
					break;
				}
			}
		}
	}
	return projCode;
}
//newPage//
int CS_wktAxisToQuad (const TrcWktElement* wktElement)
{
	bool swap = false;
	int quad = 1;
	size_t locateIndex = 0;

	ErcWktAxisId axisId1 = rcWktAxisIdNone;
	ErcWktAxisId axisId2 = rcWktAxisIdNone;
	ErcWktAxisValue axisValue1 = rcWktAxisNone;
	ErcWktAxisValue axisValue2 = rcWktAxisNone;
	const TrcWktElement *axis1 = 0;
	const TrcWktElement *axis2 = 0;

	axis1 = wktElement->ChildLocate (rcWktAxis,locateIndex);
	if (axis1 != 0)
	{
		axisId1    = axis1->GetAxisId ();
		axisValue1 = axis1->GetAxisValue ();
		axis2 = wktElement->ChildLocate (rcWktAxis,locateIndex);
		if (axis2 != 0)
		{
			axisId2    = axis2->GetAxisId ();
			axisValue2 = axis2->GetAxisValue ();
		}
	}
	if (axis1 != 0 && axis2 != 0)
	{
		swap = (axisId1 == rcWktAxisIdLat || axisId1 == rcWktAxisIdY);
		if (swap)
		{
			axisId1    = axis2->GetAxisId ();
			axisValue1 = axis2->GetAxisValue ();
			axisId2    = axis1->GetAxisId ();
			axisValue2 = axis1->GetAxisValue ();
		}
		if (axisId1 == rcWktAxisIdLng && axisId2 == rcWktAxisIdLat)
		{
			if      (axisValue1 == rcWktAxisEast) quad = (axisValue2 == rcWktAxisNorth) ? 1 : 4;
			else if (axisValue1 == rcWktAxisWest) quad = (axisValue2 == rcWktAxisNorth) ? 2 : 3;
		}
		else if (axisId1 == rcWktAxisIdX && axisId2 == rcWktAxisIdY)
		{
			if      (axisValue1 == rcWktAxisEast) quad = (axisValue2 == rcWktAxisNorth) ? 1 : 4;
			else if (axisValue1 == rcWktAxisWest) quad = (axisValue2 == rcWktAxisNorth) ? 2 : 3;
		}
	}
	return (swap) ? -quad : quad;
}
int CS_isWkt (const char *wellKnownText)
{
	int left = 0;
	int right = 0;
	int rtnValue = 0;
	const char* cp;

	for (cp = wellKnownText;*cp != '\0';cp++)
	{
		if (*cp == '[') left += 1;
		if (*cp == ']') right += 1;
	}
	/* Technically, a WKT string only needs one set of brackets, but such would
	   not be very useful.  We say two or more brackets qualfies as an attempt
	   at a WKT string. */
	if (left > 1 || right > 1)
	{
		if (left == right)
		{
			/* The brackets balance, we say it is a valid WKT. */
			rtnValue = 1;
		}
		else
		{
			/* Brackets don't balance, but there are at least two of them,
			   so we consider it to be an invalid WKT string. */
			rtnValue = -1;
		}
	}
	return rtnValue;
}
//newPage//
// This following is an extended version of the original CS_wktToCs function.
// It is specifically designed to work well with the CS_wktCsDefFunc function.
// Returns zero on success, -1 on hard failure.  Cause of hard failure will
// have already been reported to CS_erpt ().
//
// CS_wktToCs, CS_wktToDt, and CS_wktToEl attempt to produce a CS-MAP rendition
// of whatever was extracted from the WKT string, without any attempt to
// enhance the conversion.
//
// This extended version attempts to enhance the conversion to the degree
// possible.  The primary need here is to supply some sort of datum definition
// as such is rarely present in a WKT string.  Other enhancements rely on the
// mapping of a WKT name, using the provided (or determined) flavor, to a
// CS-MAP name.  In any case, use CS_wktToCs (et al) if you don't want any of
// these enhancements.
//
// If provided a flavor of wktFlvrNone, this function will attempt to
// determine the flavor.  The best results are obtained if the flavor
// is specified.  The difference being: If this function is to determine
// the flavor and it fails, you get a message to the effect that the flavor
// could not be determined.  If you specify a flavor, you will get a much
// more meaningful error message indicating where the WKT string departs
// from the flavor specified.
//
int CS_wktToCsEx (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const char *wellKnownText, int bRunBinaryFallback)
{
	bool mapNames = true;			// until we know different, we will map WKT names to
									// CS-MAP names wherever possible.

	EcsMapSt csMapSt;
	EcsNameFlavor nmFlavor;
	ErcWktFlavor myFlavor;
	EcsMapObjType mapType;

	int st;
	long32_t epsgNbr;
	const char* wktNamePtr;
	struct cs_Prjtab_ *pp;

	char wrkBufr [128];
	char csMapElName [cs_KEYNM_DEF];
	char csMapDtName [cs_KEYNM_DEF];
	char csMapCsName [cs_KEYNM_DEF];

	/* Establish a stable environment; mostly to keep lint happy. */
	if (csDef == NULL)
	{
		CS_erpt (cs_INV_ARG1);
		return -1;
	}
	if (dtDef == NULL)
	{
		CS_erpt (cs_INV_ARG2);
		return -1;
	}
	if (elDef == NULL)
	{
		CS_erpt (cs_INV_ARG3);
		return -1;
	}

	st = 0;
	csMapElName [0] = '\0';
	csMapDtName [0] = '\0';
	csMapCsName [0] = '\0';

	// Parse the provided string and determine the flavor if necessary.
	TrcWktElement wktElement (wellKnownText);
	if (wktElement.GetElementType () == rcWktUnknown)
	{
		// Parsing failed.  We assume that the provided string is either not
		// a WKT string, or is a badly formed WKT string.
		CS_stncp (csErrnam,wellKnownText,40);
		CS_erpt (cs_WKT_BADFORM);
		goto error;
	}
	wktElement.ParseChildren ();

	// If the user has specified a flavor, we try it first.  If the specified
	// flavor is Oracle, we use the flavor detector to see if it is an
	// Oracle 9 WKT string.  If so, we switch the flavor as appropriate.
	st = 0;
	if (flavor == wktFlvrOracle)
	{
		myFlavor = wktElement.DetermineFlavor ();
		if (myFlavor == wktFlvrOracle9)
		{
			flavor = myFlavor;
		}
	}
	if (flavor != wktFlvrNone)
	{
		st = CSwktToCs (csDef,dtDef,elDef,flavor,&wktElement);
	}
	if (flavor == wktFlvrNone || st < 0)
	{
		// Here if the user did not specify a flavor (st == 0) or
		// the parsing with that flavor failed.  We try determining
		// the flavor from the string and see if we get a successful
		// result.
		flavor = wktElement.DetermineFlavor ();
		if (flavor == wktFlvrUnknown)
		{
			if (st == 0)
			{
				// If we could not determine the flavor, and we are here
				// because the user failed to specify a flavor (st == 0),
				// we report the error as a failure to determine the flavor.
				// We could not determine the flavor.
				const char* cp = wktElement.GetElementNameC ();
				CS_stncp (csErrnam,cp,MAXPATH);
				CS_erpt (cs_WKT_FLAVOR);
			}
			// Otherwise, we assume that CSwktToCs has already reported
			// an appropriate error, and we simply return a failure
			// status
			goto error;
		}
		else
		{
			// We were able to determine the flavor.  Try again with the
			// determined flavor.
			st = CSwktToCs (csDef,dtDef,elDef,flavor,&wktElement);
		}
	}
	// Preserve the flavor which was successful in parsing the WKT string.
	nmFlavor = csWktFlvrToCsMapFlvr (flavor);

	// Some flavors, Oracle in particular, like to have WKT definitions without
	// any parameters.  In this situation, the remainder of this process is
	// doomed to fail.  Therefore, as we have determined a flavor (hopefully),
	// in this situation we simply see if we can map the name to a dictionary
	// name.  If so, we simply extract the named definitions from the
	// dictionary and return a valid status.
	if ((wktElement.GetElementType () == rcWktProjCS) &&
		(wktElement.GetParameterCount () == 0)
	   )
	{
		bool ok = false;
		wktNamePtr = wktElement.GetElementNameC ();
		csMapSt = csMapNameToNameC (csMapProjectedCSysKeyName,wrkBufr,sizeof (wrkBufr),csMapFlvrAutodesk,nmFlavor,wktNamePtr);
		if (csMapSt == csMapOk)
		{
			struct cs_Csdef_* csDefPtr = 0;
			struct cs_Dtdef_* dtDefPtr = 0;
			struct cs_Eldef_* elDefPtr = 0;
			// No parameters, but we have a name map.  Extract the three definitions
			// we need from the dictionaries.
			csDefPtr = CS_csdef (wrkBufr);
			if (csDefPtr != 0)
			{
				CS_stncp (csDefPtr->desc_nm,wktNamePtr,sizeof (csDefPtr->desc_nm));
				CS_stncp (csDefPtr->source,"Extracted from WKT by name only; description carries WKT name.",sizeof (csDefPtr->source));
				if (csDefPtr->dat_knm [0] != '\0')
				{
					dtDefPtr = CS_dtdef (csDefPtr->dat_knm);
					if (dtDefPtr != 0)
					{
						elDefPtr = CS_eldef (dtDefPtr->ell_knm);
						if (elDefPtr != 0)
						{
							memcpy (csDef,csDefPtr,sizeof (*csDef));
							memcpy (dtDef,dtDefPtr,sizeof (*dtDef));
							memcpy (elDef,elDefPtr,sizeof (*elDef));
							ok = true;
							CS_free (elDefPtr);
						}
						CS_free (dtDefPtr);
					}
				}
				else if (csDefPtr->elp_knm [0] != '\0')
				{
					elDefPtr = CS_eldef (csDefPtr->elp_knm);
					if (elDefPtr != 0)
					{
						memset (dtDef,'\0',sizeof (*dtDef));
						memcpy (elDef,elDefPtr,sizeof (*elDef));
						ok = true;
						CS_free (elDefPtr);
					}
				}
				CS_free (csDefPtr);	
			}
		}
		if (ok)
		{
			/* This WKT was pretty normal, we simply return success status. */
			return 0;
		}
	}

	if (st < 0)
	{
		// Oops!!! Hard error.  It should have already been reported.
		// Simply return the hard error indication.
		goto error;
	}

	// See if we can map the ellipsoid name to a CS-MAP name.  We map first,
	// in case of the unlikely event that a flavored name exists which is
	// the same as a CS-MAP name, but maps to a different ellipsoid.
	csMapSt = csMapNameToNameC (csMapEllipsoidKeyName,wrkBufr,sizeof (wrkBufr),
															  csMapFlvrAutodesk,
															  nmFlavor,
															  elDef->name);
	if (csMapSt == csMapOk)
	{
		// Ellipsoid name mapping was successful.  The proposed name is saved
		// for possible replacement.
		CS_stncp (csMapElName,wrkBufr,cs_KEYNM_DEF);
		CS_stncp (elDef->key_nm,csMapElName,cs_KEYNM_DEF);
		CS_stncp (dtDef->ell_knm,csMapElName,cs_KEYNM_DEF);

		// If ellipsoid name was truncated, it is not anymore
		st &= ~cs_EL2WKT_NMTRUNC;
	}
	if (csMapSt != csMapOk && elDef->epsgNbr != 0)
	{
		// There is an AUTHORITY element with (possibly) an EPSG number.
		csMapSt = csMapIdToNameC (csMapEllipsoidKeyName,wrkBufr,sizeof (wrkBufr),
																csMapFlvrAutodesk,
																csMapFlvrEpsg,
																(unsigned long)elDef->epsgNbr);
		if (csMapSt == csMapOk)
		{
			// Ellipsoid name mapping was successful.  The proposed name is saved
			// for possible replacement.
			CS_stncp (csMapElName,wrkBufr,cs_KEYNM_DEF);
			CS_stncp (elDef->key_nm,csMapElName,cs_KEYNM_DEF);
			CS_stncp (dtDef->ell_knm,csMapElName,cs_KEYNM_DEF);

			// If ellipsoid name was truncated, it is not anymore
			st &= ~cs_EL2WKT_NMTRUNC;
		}
	}
	if (csMapSt != csMapOk)
	{
		// If neither of those worked, we start pulling at straws.
		// If the WKT name was a valid CS-MAP name, we use it.
		if (CS_elIsValid (elDef->name))
		{
			CS_stncp (csMapElName,elDef->name,cs_KEYNM_DEF);
			CS_stncp (elDef->key_nm,csMapElName,cs_KEYNM_DEF);
			CS_stncp (dtDef->ell_knm,csMapElName,cs_KEYNM_DEF);
			st &= ~cs_EL2WKT_NMTRUNC;
		}
		else if (CS_elIsValid (elDef->key_nm))
		{
			// If the key name ended up being a valid CS-MAP name (rare)
			// we'll use it.
			CS_stncp (csMapElName,elDef->key_nm,cs_KEYNM_DEF);
			CS_stncp (dtDef->ell_knm,csMapElName,cs_KEYNM_DEF);
			st &= ~cs_EL2WKT_NMTRUNC;
		}
	}

	if ((st & cs_EL2WKT_NMTRUNC) != 0)
	{
		// If the name of the ellipsoid is still truncated, we replace the name
		// with something more meaningful if we can.  Only EPSG assigns numeric
		// codes to ellipsoids, so we use that code.
		epsgNbr = csMapNameToIdC (csMapEllipsoidKeyName,csMapFlvrEpsg,nmFlavor,elDef->name);
		if (epsgNbr != 0)
		{
			sprintf (wrkBufr,"EPSG::%d",(int)epsgNbr);
			CS_stncp (elDef->key_nm,wrkBufr,cs_KEYNM_DEF);

			// Replace the ellipsoid key name in the dtDef structure with
			// the new name as well. We leave the source name alone.
			CS_stncp (dtDef->ell_knm,wrkBufr,cs_KEYNM_DEF);

			// If ellipsoid name was truncated, it is not anymore
			st &= ~cs_EL2WKT_NMTRUNC;
		}
	}
	// Ellipsoid processing, to the degree possible, is complete.
	
	// Start processing dealing with the Datum name.
	if ((st & cs_DT2WKT_NODEF) != 0)
	{
		struct cs_Dtdef_ *dtDefPtr = 0;

		// There was no complete datum definition in the WKT string provided.
		// However, we should have a WKT datum name.  We use what name(s) we
		// have and try to map it to an MSI name.  If successful, we replace
		// the guts of the dtDef structure (which is not a complete datum
		// definition) with the datum definition from the MSI dictionary, and
		// replace the datum name in the csDef structure with the MSI name of
		// the definition which we used to obtain a complete definition.

		// Our objective for the next few blocks of code is to populate the
		// csMapDtName array with a CS-MAP datum name which, presumably, is
		// equivalent to the definition referred to in the WKT.  Since WKT
		// only ocassionally includes any datum components other than
		/// the ellipsoid, it is hard to argue with this presumption.
		csMapDtName [0] = '\0';
		
		// See if the actual WKT name extracted from the WKT (as preserved
		// in the dtDef->name element by the CS_cs2Wkt parser) maps to a
		// CS-MAP datum name.
		csMapSt = csMapNameToNameC (csMapDatumKeyName,wrkBufr,sizeof (wrkBufr),
															  csMapFlvrAutodesk,
															  nmFlavor,
															  dtDef->name);
		if (csMapSt == csMapOk)
		{
			// Yup!!! We'll go with this result.
			CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
		}
		if (csMapSt != csMapOk && dtDef->epsgNbr != 0)
		{
			// If the WKT had an AUTHORITY element, and the authority was EPSG,
			// we'll have an EPSG number we can deal with.
			csMapSt = csMapIdToNameC (csMapDatumKeyName,wrkBufr,sizeof (wrkBufr),
																  csMapFlvrAutodesk,
																  csMapFlvrEpsg,
																  (unsigned long)dtDef->epsgNbr);
			if (csMapSt == csMapOk)
			{
				// Yup!!! We'll go with this result.
				CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
			}
		}
		if (csMapSt != csMapOk)
		{
			// Still no luck; we start grasping at straws now.
			// See if the provided datum name is already a CS-MAP name.
			// If so, we use it.  We do this after the mapping in the
			// unlikely event that a flavored name is the same as a
			// CS-MAP name, yet references a different datum than the
			// CS-MAP name.
			if (CS_dtIsValid (dtDef->name))
			{
				CS_stncp (csMapDtName,dtDef->name,sizeof (csMapDtName));
			}
			else if (CS_dtIsValid (dtDef->key_nm))
			{
				CS_stncp (csMapDtName,dtDef->key_nm,sizeof (csMapDtName));
			}
		}
		if (csMapDtName [0] == '\0')
		{
			// Some flavors of WKT include an EPSG ID in the name.  Let's use
			// that if its there.
			const char* pszEpsgId=strstr(dtDef->name, "EPSG ID ");
			if (pszEpsgId!=NULL)
			{
				long lEpsgNbr=0;
				long lResult=sscanf(pszEpsgId, "EPSG ID %ld", &lEpsgNbr);
				if (lResult!=0 && lResult!=EOF)
				{
					csMapSt = csMapIdToNameC (csMapDatumKeyName,wrkBufr,
																sizeof (wrkBufr),
																csMapFlvrAutodesk,
																csMapFlvrEpsg,
																(unsigned long)lEpsgNbr);
					if (csMapSt == csMapOk)
					{
						CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
					}
				}
			}
		}

		// We have used all our tricks to arrive at a CS-MAP datum key name.
		// If we have a name, we get a pointer to the actual; definition.  If
		// the name we arrived at is not valid (not supposed to happen), we
		// will have a null pointer.
		if (csMapDtName [0] != '\0')
		{
			// dtDefPtr is a pointer to the definition we obtain from the
			// CS-MAP dictionary in response to the fact that there is
			// no datum definition in the WKT that we can use.  Note that
			// this pointer must be deleted.  Since this is now C++, perhaps
			// we should use a std::auto_ptr.
			dtDefPtr = CS_dtdef (csMapDtName);
		}

		if(csDef->dat_knm[0] == '\0')
		{
			// Create an empty datum
			dtDefPtr = (struct cs_Dtdef_ *)CS_malc (sizeof (struct cs_Dtdef_));
			if (dtDefPtr == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}
			memset(dtDefPtr, 0, sizeof(cs_Dtdef_));

			// Set the ellipsoid name 
			CS_stncp (csDef->elp_knm,elDef->key_nm,cs_KEYNM_DEF);
		}

		if (dtDefPtr != 0 && csDef->dat_knm[0] != '\0')
		{
			// Here when:
			// 1> there was no complete datum definition in the WKT string, AND
			// 2> name mapping produced the CS-MAP name of what we will consider
			//    to be the complete definition of the datum indicated by the
			//    WKT string.  Not 100% reliable by any means, but the only
			//    alternative is to return a set of unusable definitions.
			//
			// We have several jobs to do here.  First, we replace the WKT
			// datum name in the cs_Dtdef_ key name field, and also in the
			// cs_Csdef_ field that references it.

			// Below, we will copy the CS-MAP dictionary datum definition which
			// the name mapping process indicates what the the WKT datum should
			// be.  It is entirely possible that the ellipsoid referenced in
			// the CS-MAP dictionary entry is not the same as the ellipsoid
			// definition which was obtained from the WKT string.  Since the
			// WKT rendition of ellipsoids is complete and usable, we replace
			// the ellipsoid key name with that obtained from the CS-MAP
			// dictionary only after we verify that the ellipsoids are indeed
			// the same.
			//
			// If they are not the same, we leave the ellipsoid name in the
			// cs_Dtdef_ structure alone, as it should be whatever name was
			// assigned to the WKT ellipsoid during the WKT parse.  Thus, the
			// returned definition will reference the actual ellipsoid
			// definition which was provided by the WKT string.
			int same = CSelDictWktCompare (dtDefPtr->ell_knm,elDef);
			if (same != 0)
			{
				// The ellipsoid referenced by the CS-MAP datum dictionary
				// definition is the same (numerically) as the ellipsoid
				// definition extracted from the WKT string.  We choose
				// to use the CS-MAP key name.
				CS_stncp (dtDef->ell_knm,dtDefPtr->ell_knm,sizeof (dtDef->ell_knm));
				
				// Since we will be using the CS-MAP definition with its CS-MAP key
				// name, we activate the datum key name replacement.
				CS_stncp (csMapDtName,dtDefPtr->key_nm,cs_KEYNM_DEF);
			}
			else
			{
				// Since we have determined it inappropriate to map the ellipsoid
				// name, we suppress the mapping of all names.
				mapNames = false;
			}

			// Copy the rest of the CS-MAP definition to the dtDef structure.
			// Preserve the 'source', 'descriptive name', and WKT flavor from
			// the WKT parse.
			CS_stncp (dtDef->group,dtDefPtr->group,sizeof (dtDef->group));
			CS_stncp (dtDef->locatn,dtDefPtr->locatn,sizeof (dtDef->locatn));
			CS_stncp (dtDef->cntry_st,dtDefPtr->cntry_st,sizeof (dtDef->cntry_st));
			CS_stncp (dtDef->fill,dtDefPtr->fill,sizeof (dtDef->fill));
			dtDef->delta_X  = dtDefPtr->delta_X;
			dtDef->delta_Y  = dtDefPtr->delta_Y;
			dtDef->delta_Z  = dtDefPtr->delta_Z;
			dtDef->rot_X    = dtDefPtr->rot_X;
			dtDef->rot_Y    = dtDefPtr->rot_Y;
			dtDef->rot_Z    = dtDefPtr->rot_Z;
			dtDef->bwscale  = dtDefPtr->bwscale;
			dtDef->protect  = dtDefPtr->protect;
			dtDef->to84_via = dtDefPtr->to84_via;
			dtDef->epsgNbr  = dtDefPtr->epsgNbr;
			dtDef->fill01   = dtDefPtr->fill01;
			dtDef->fill02   = dtDefPtr->fill02;
			dtDef->fill03   = dtDefPtr->fill03;
			dtDef->fill04   = dtDefPtr->fill04;

			// Leave the original WKT name alone. It is useful info.
			// Use the source field to indicate where the definition came from.
			CS_stncp (dtDef->source,"Obtained from dictionary by mapping the WKT datum name.",sizeof (dtDef->source));
		}
		if (dtDefPtr != NULL)
		{
			CS_free (dtDefPtr);
		}
	}
	else
	{
		csMapDtName [0] = '\0';			// Redundant, but I'll sleep better tonight.

		// There was a complete datum definition in the WKT string.  Rare, but
		// it happens, depending upon the flavor.  See if the WKT name maps
		// to an CS-MAP name.  If so, we'll use the CS-MAP name.
		csMapSt = csMapNameToNameC (csMapDatumKeyName,wrkBufr,sizeof (wrkBufr),
															  csMapFlvrAutodesk,
															  nmFlavor,
															  dtDef->name);
		if (csMapSt == csMapOk)
		{
			CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
			CS_stncp (csDef->dat_knm,csMapDtName,cs_KEYNM_DEF);
			CS_stncp (dtDef->key_nm,csMapDtName,cs_KEYNM_DEF);

			// If datum name was truncated, it is not anymore.
			st&= ~cs_DT2WKT_NMTRUNC;
		}
		if (csMapSt != csMapOk && dtDef->epsgNbr != 0)
		{
			// The thing has an EPSG number; see if that maps to something
			// useful.
			csMapSt = csMapIdToNameC (csMapDatumKeyName,wrkBufr,sizeof (wrkBufr),
																  csMapFlvrAutodesk,
																  csMapFlvrEpsg,
																  (unsigned long)dtDef->epsgNbr);
			if (csMapSt == csMapOk)
			{
				// YUP!!! We'll accept the EPSG reference as being more useful
				// and accurate than any of the other stuff.
				CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
				CS_stncp (csDef->dat_knm,csMapDtName,cs_KEYNM_DEF);
				CS_stncp (dtDef->key_nm,csMapDtName,cs_KEYNM_DEF);

				// If datum name was truncated, it is not anymore.
				st&= ~cs_DT2WKT_NMTRUNC;
			}
		}
		if (csMapSt != csMapOk)
		{
			// Some flavors of WKT include an EPSG ID in the name.  Let's use
			// that if its there.
			const char* pszEpsgId=strstr(dtDef->name, "EPSG ID ");
			if (pszEpsgId!=NULL)
			{
				long lEpsgNbr=0;
				long lResult=sscanf (pszEpsgId,"EPSG ID %ld",&lEpsgNbr);
				if (lResult!=0 && lResult!=EOF)
				{
					csMapSt = csMapIdToNameC (csMapDatumKeyName,wrkBufr,sizeof (wrkBufr),
																		  csMapFlvrAutodesk,
																		  csMapFlvrEpsg,
																		  (unsigned long)dtDef->epsgNbr);
					if (csMapSt == csMapOk)
					{
						// YUP!!! We'll accept the EPSG reference as being more useful
						// and accurate than any of the other stuff.
						CS_stncp (csMapDtName,wrkBufr,sizeof (csMapDtName));
						CS_stncp (csDef->dat_knm,csMapDtName,cs_KEYNM_DEF);
						CS_stncp (dtDef->key_nm,csMapDtName,cs_KEYNM_DEF);

						// If datum name was truncated, it is not anymore.
						st&= ~cs_DT2WKT_NMTRUNC;
					}
				}
			}
		}
		if (csMapSt != csMapOk)
		{
			// See if the provided datum name is already a CS-MAP name.
			// If so, we use it.  We do this after the mapping in the
			// unlikely event that a flavored name is the same as a
			// CS-MAP name, yet references a different datum than the
			// CS-MAP name.
			if (CS_dtIsValid (dtDef->name))
			{
				CS_stncp (csMapDtName,dtDef->name,sizeof (csMapDtName));
			}
			else if (CS_dtIsValid (dtDef->key_nm))
			{
				CS_stncp (csMapDtName,dtDef->key_nm,sizeof (csMapDtName));
			}
			if (csMapDtName [0] != '\0')
			{
				CS_stncp (csDef->dat_knm,csMapDtName,cs_KEYNM_DEF);
				CS_stncp (dtDef->key_nm,csMapDtName,cs_KEYNM_DEF);

				// If datum name was truncated, it is not anymore.
				st&= ~cs_DT2WKT_NMTRUNC;
			}
		}
 
		// See if we can produce an untruncated name for this datum which is
		// meaningful.
		if ((st & cs_DT2WKT_NMTRUNC) != 0)
		{
			epsgNbr = csMapNameToIdC (csMapDatumKeyName,csMapFlvrEpsg,nmFlavor,dtDef->name); 
			if (epsgNbr > 0 && static_cast<unsigned long>(epsgNbr) != KcsNmInvNumber)
			{
				sprintf (wrkBufr,"EPSG::%d",(int)epsgNbr);
				CS_stncp (dtDef->key_nm,wrkBufr,cs_KEYNM_DEF);

				// Replace the datum key name in the csDef structure with
				// the new name as well.
				CS_stncp (csDef->dat_knm,wrkBufr,cs_KEYNM_DEF);

				// If the datum name was truncated, it is not anymore.
				st &= ~cs_DT2WKT_NMTRUNC;
			}
		}
		if ((st & cs_DT2WKT_NMTRUNC) != 0)
		{
			// Some flavors of WKT include an EPSG ID in the name.  Let's use
			// that if its there.
			const char* pszEpsgId=strstr(dtDef->name, "EPSG ID ");
			if (pszEpsgId!=NULL)
			{
				long lEpsgNbr=0;
				long lResult=sscanf (pszEpsgId,"EPSG ID %ld",&lEpsgNbr);
				if (lResult!=0 && lResult!=EOF)
				{
					sprintf (wrkBufr,"EPSG::%u",(unsigned)lEpsgNbr);
					CS_stncp (dtDef->key_nm,wrkBufr,cs_KEYNM_DEF);

					// Replace the datum key name in the csDef structure with
					// the new name as well.
					CS_stncp (csDef->dat_knm,wrkBufr,cs_KEYNM_DEF);

					// If the datum name was truncated, it is not anymore.
					st &= ~cs_DT2WKT_NMTRUNC;
				}
			}
		}
	}

	// All done with the datum and ellipsoid definitions.  Concentrate now on
	// the coordinate system definition.  See if we can map the WKT coordinate
	// system name into a CS-MAP name.

	// Get a pointer to the CS-MAP projection table entry for the indicated
	// projection.  We can do some more general stuff once we have that.
	csMapCsName [0] = '\0';
	for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
	{
		if (!CS_stricmp (pp->key_nm,csDef->prj_knm)) break;
	}
	mapType = (pp->code == cs_PRJCOD_UNITY) ? csMapGeographicCSysKeyName : csMapProjectedCSysKeyName; 
	csMapSt = csMapNameToNameC (mapType,wrkBufr,sizeof (wrkBufr),csMapFlvrAutodesk,nmFlavor,
																				   csDef->desc_nm);
	if (csMapSt == csMapOk)
	{
		CS_stncp (csMapCsName,wrkBufr,cs_KEYNM_DEF);
		st &= ~cs_CS2WKT_NMTRUNC;
	}
	else if (csDef->epsgNbr != 0)
	{
		// The WKT had aan EPSG code for the CRS definition.  See if that maps
		// to something useful.
		csMapSt = csMapIdToNameC (mapType,wrkBufr,sizeof (wrkBufr),csMapFlvrAutodesk,
																   csMapFlvrEpsg,
																   (unsigned long)csDef->epsgNbr);
		if (csMapSt == csMapOk)
		{
			CS_stncp (csMapCsName,wrkBufr,cs_KEYNM_DEF);
			st &= ~cs_CS2WKT_NMTRUNC;
		}
	}
	if (csMapSt != csMapOk)
	{
		// Still no luck, we start graspiong for straws.
		// If the name mapper couldn't produce a CS-MAP name, we see if the
		// name is a CS-MAP name already.  We do this after the name
		// mapper check in the unlikely event of a flavored name that is
		// the same as a CS-MAP name which maps to a different
		// coordinate system.
		if (CS_csIsValid (csDef->desc_nm))
		{
			CS_stncp (csMapCsName,csDef->desc_nm,cs_KEYNM_DEF);
			st &= ~cs_CS2WKT_NMTRUNC;
		}
		else if (CS_csIsValid (csDef->key_nm))
		{
			CS_stncp (csMapCsName,csDef->key_nm,cs_KEYNM_DEF);
			st &= ~cs_CS2WKT_NMTRUNC;
		}
	}

	// The following is structured for debugging purposes.  Since this
	// fallback business is so slow, it is nice to be able to turn it
	// off and still be able to see the conditions under which it is
	// invoked if it were active.
	if (csMapCsName [0] == '\0'  || (csMapElName [0] == '\0' && csMapDtName [0] == '\0'))
	{
		// The above code which attempts to map the WKT to a CS-MAP coordinate system
		// failed.  The following is a rather effective, though very painful, means of
		// getting around this condition.  Hopefully, it isn't invoked very often.
		if (bRunBinaryFallback)
		{
			const char *kpchCsCodes = CScsKeyNames();
			const char *kpchCode;
			char szCode[250];
			if (kpchCsCodes)
			{
				// get CS key names for the source dictionary
				for (kpchCode = kpchCsCodes; *kpchCode; kpchCode += strlen(kpchCode) + 1)
				{
					// read coordsys from dictionary & initialize it
					// this is expensive but we failed all other methods anyway
					strcpy(szCode, kpchCode);

					CS_nampp (szCode);			//lint !e534  ignoing return value
					cs_Csdef_ *pCsDefTarget = CS_csdef (szCode);
					if (pCsDefTarget)
					{
						// Check is coordinate system structures are identical
 						if (0==CS_csDefCmp(csDef, pCsDefTarget, NULL, 0))
						{
							// wow! we found a match for the CSDEF
							// check the datum
							char szElTargetName[24];
							szElTargetName[0]='\0';

							// NTO:07Mar2011::At this point we used to check if
							// the current candidate was geodetically or
							// cartographically referenced.  This produced an
							// erroneous match with the genric 'LL' system
							// occasionally.  AT this time, we correct this to/
							// check if how teh system we're trying to match is
							// referenced.
//??// hOIWEVER, WITH THIS CHANGE, PdTdEFtARGET CAN BE NULL AND WE HAVE A MEMORY EXCEPTION BELOW.
							if (pCsDefTarget->dat_knm[0] != '\0')
							//if (csDef->dat_knm [0] != '\0')
							{
								//load the datum
								cs_Dtdef_ *pDtDefTarget=CS_dtdef(pCsDefTarget->dat_knm);

								//if names are identical, we're good
								if (csMapDtName [0] != '\0'
									&& 0==strcmp(pCsDefTarget->dat_knm, csMapDtName))
								{
									//Wow! Same name, get the ellipsoid
									CS_stncp (szElTargetName,pDtDefTarget->ell_knm,cs_KEYNM_DEF);
								}
								else
								{
									//DT names differ so run a binary check
									if (0==CS_dtDefCmp(dtDef, pDtDefTarget, NULL, 0))
									{
										//Wow! DTDEF are the same so get the ellipsoid
										CS_stncp (szElTargetName,pDtDefTarget->ell_knm,cs_KEYNM_DEF);
										if (csMapDtName [0]=='\0')
										{
											CS_stncp (csMapDtName,pDtDefTarget->key_nm,cs_KEYNM_DEF);
										}
									}
								}
								CS_free(pDtDefTarget);
							}
							else if (csDef->elp_knm && *csDef->elp_knm)
							{
								CS_stncp (szElTargetName,pCsDefTarget->elp_knm,cs_KEYNM_DEF);
							}

							//check the ellipsoids
							if (szElTargetName != 0 && (*szElTargetName) != '\0')
							{
								bool bIdentical=false;
								//if names are identical, we're good
								if (csMapElName [0] != '\0'
									&& 0==strcmp(szElTargetName, csMapElName))
								{
									//Wow! Same name, we have a complete CS
									bIdentical=true;
								}
								else
								{
									//Wow! ELDEF are the same, we have a complete CS
									cs_Eldef_ *pElDefTarget=CS_eldef(szElTargetName);
									if (0==CS_elDefCmp(elDef, pElDefTarget, NULL, 0))
									{
										if (csMapElName [0]=='\0')
										{
											CS_stncp (csMapElName,pElDefTarget->key_nm,cs_KEYNM_DEF);
										}
										bIdentical=true;
									}
									CS_free(pElDefTarget);
								}

								if (bIdentical)
								{
									//perfect match
									if (csDiagnostic != 0)
									{
										fprintf (csDiagnostic,"%.64s,%.64s,%.64s,%.24s,%.24s,%.24s,%.24s\n",csDef->desc_nm,
																											dtDef->name,
																											elDef->name,
																											csMapCsName,
																											csMapDtName,
																											csMapElName,
																											pCsDefTarget->key_nm);
									}
									CS_stncp (csDef->key_nm,pCsDefTarget->key_nm,cs_KEYNM_DEF);
									CS_stncp (csMapCsName,pCsDefTarget->key_nm,cs_KEYNM_DEF);
									CS_free(pCsDefTarget);
									break;
								}
							}
						}
						CS_free(pCsDefTarget);
					}
				}
			}
		}
	}

	if ((st & cs_CS2WKT_NMTRUNC) != 0)
	{
		// If the key-name name of the resulting cs_Csdef_ was truncated,
		// do the best we can to replace it with something meaningful, and
		// untruncated.
        epsgNbr = csMapNameToIdC (csMapProjGeoCSys,csMapFlvrEpsg,nmFlavor,csDef->desc_nm); 
		if (epsgNbr > 0 && static_cast<unsigned long>(epsgNbr) != KcsNmInvNumber)
		{
			sprintf (wrkBufr,"EPSG::%u",(unsigned)epsgNbr);
			CS_stncp (csDef->key_nm,wrkBufr,cs_KEYNM_DEF);

	        // If the coordinate system name was truncated, it's not anymore.
		    st &= ~cs_CS2WKT_NMTRUNC;
		}
	}

	// If it has been determined appropriate to map all three names, we do so
	// now, but only if all three names have been mapped successfully.
	if (mapNames && csMapElName [0] != '\0' &&
					csMapDtName [0] != '\0' &&
					csMapCsName [0] != '\0')
	{
		// Map all three names.
		CS_stncp (csDef->key_nm,csMapCsName,cs_KEYNM_DEF);
		CS_stncp (csDef->dat_knm,csMapDtName,cs_KEYNM_DEF);
		CS_stncp (dtDef->key_nm,csMapDtName,cs_KEYNM_DEF);
		CS_stncp (dtDef->ell_knm,csMapElName,cs_KEYNM_DEF);
		CS_stncp (elDef->key_nm,csMapElName,cs_KEYNM_DEF);
		st &= ~(static_cast<int>(cs_DT2WKT_NMTRUNC | cs_EL2WKT_NMTRUNC));
		st &= ~(static_cast<int>(cs_CS2WKT_NMTRUNC));
	}
	else if (mapNames && csMapElName [0] != '\0' &&
						 csMapDtName [0] == '\0' &&
						 csMapCsName [0] != '\0')
	{
		// Map all three names.
		CS_stncp (csDef->key_nm,csMapCsName,cs_KEYNM_DEF);
		CS_stncp (csDef->dat_knm,"",cs_KEYNM_DEF);
		CS_stncp (csDef->elp_knm,csMapElName,cs_KEYNM_DEF);
		CS_stncp (dtDef->key_nm,"",cs_KEYNM_DEF);
		CS_stncp (dtDef->ell_knm,"",cs_KEYNM_DEF);
		CS_stncp (elDef->key_nm,csMapElName,cs_KEYNM_DEF);
		st &= ~(static_cast<int>(cs_DT2WKT_NMTRUNC | cs_EL2WKT_NMTRUNC));
		st &= ~(static_cast<int>(cs_CS2WKT_NMTRUNC));
	}
	if (st >= 0)
	{
		st = 0;

		if (dtDef->key_nm[0] != '\0')
		{
			CS_stncp (dtDef->ell_knm,elDef->key_nm,cs_KEYNM_DEF);
			CS_stncp (csDef->dat_knm,dtDef->key_nm,cs_KEYNM_DEF);
		}
		else
		{
			CS_stncp (dtDef->ell_knm,"",cs_KEYNM_DEF);
			CS_stncp (csDef->elp_knm,elDef->key_nm,cs_KEYNM_DEF);
			CS_stncp (csDef->dat_knm,"",cs_KEYNM_DEF);
		}
	}
	return st;
error:
	return -1;
}
///////////////////////////////////////////////////////////////////////////////
// The following converts a WKT PROJCS or GEOGCS object to CS-MAP form
// consisting of a a cs_Csdef_ structure, a cs_Dtdef_ structure, and a
// cs_Eldef_ structure.  The WKT PROJCS object often does not contain enough
// information for a cs_Dtdef_ structure.  The name of the referenced datum,
// if there is one, is inserted in the dat_knm member of the cs_Csdef_
// structure.
//
// Notes:
//
//	1> Note the "C" linkage!  Intended to be callable from the normal CS-MAP
//	   environment.
//	2> Use CS_wktToDt to process a WKT GEOTRAN object (ESRI) or a WKT DATUM
//	   object to obtain a cs_Dtdef_ structure.
//	3> This function will process a GEOGCS object and return a cs_Csdef_
//	   structure for the CS-MAP equivalent.
//	4> Use the flavor enumeration to indicate the types of names to be expected.
//	5> Returns zero to indicate success.
//	6> Returns negative value to indicate hard failure.  Use CS_errmsg to get a
//	   textual description.
//	7> A positive return may have any of the following bits set to indicate
//	   the state of the resulting conversion:
//		cs_EL2WKT_NMTRUNC (1)	Ellipsoid key name truncated
//		cs_DT2WKT_NMTRUNC (2)	Datum key name truncated
//		cs_CS2WKT_NMTRUNC (4)	Coordinate system key name truncated
//		cs_DT2WKT_DTDEF   (8)	Datum definition extracted from dictionary by name.
//		cs_DT2WKT_NODEF  (16)	No datum definition is present in dtDef.
//	8> The 'desc_nm' element will always carry the full WKT system name (up to 63 characters).
//	9> In the case of key names of all types,  they are generally trunctated to
//	   the 23 charcater maximum supported by CS-MAP.  In the case of a DATUM
//	   key name, a trailing "_To_WGS84" is removed before truncation.  A
//	   leading "D_" is removed from datum names as well.
// 10> In the case of processing a GEOTRAN, the name assigned to the result is
//	   extracted from the GEOTRAN name, after any embedded "_ToWGS84" is removed
//	   from the name.
//
// The bulk of the processing is embodied in the CSwktToCs function defined
// below.  This refactoring eliminates multiple parsings of the WKT string.
int CS_wktToCs (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const char *wellKnownText)
{
	int st;
	ErcWktFlavor myFlavor;

	// Convert the text string to TrcWktElement form.  This object will get
	// deleted automatically as soon as it goes out of scope.
	TrcWktElement wktElement (wellKnownText);
	if (wktElement.GetElementType () == rcWktUnknown)
	{
		// Parsing failed.
		CS_stncp (csErrnam,wellKnownText,40);
		CS_erpt (cs_WKT_BADFORM);
	}
	wktElement.ParseChildren ();

	/* If the caller has specified a flavor, we try it first.  If the specified
	   flavor is Oracle, we see if the WKT string is really an Oracle 9
	   WKT string.  The user is unlikely to know that. */	
	if (flavor == wktFlvrOracle)
	{
		myFlavor = wktElement.DetermineFlavor ();
		if (myFlavor == wktFlvrOracle9)
		{
			flavor = myFlavor;
		}
	}
	st = 0;
	if (flavor != wktFlvrNone)
	{
		st = CSwktToCs (csDef,dtDef,elDef,flavor,&wktElement);
	}
	/* If the user did not specify a flavor, or if the user specified flavor
	   failed, we try it with the flavor determined by the DetermineFlavor
	   member of the TrcWktElement object. */
	if (flavor == wktFlvrNone || st < 0)	
	{
		flavor = wktElement.DetermineFlavor ();
		if (flavor == wktFlvrUnknown)
		{
			/* If we couldn't determine the flavor, and the user did not
			   specify a flavor (st == 0), we report the fact that the flavor
			   could not be determined.  Otherwise, we just return a fatal
			   status as the CSwktToCs function should have already reported
			   a cause for the failure. */
			if (st == 0)
			{
				const char* cp = wktElement.GetElementNameC ();
				CS_stncp (csErrnam,cp,MAXPATH);
				CS_erpt (cs_WKT_FLAVOR);
			}
			st = -1;
		}
		else
		{
			/* We were able to determine a flavor (perhaps a different one than
			   that provided by the user).  Try it. */
			st = CSwktToCs (csDef,dtDef,elDef,flavor,&wktElement);
		}
	}
	return st;
}
// The following function embodies the majority of the functionality of
// the CS_wktToCs function defined immediately above, it takes a parsed
// TrcWktElement as an argument instead of a normal character string.
int CSwktToCs (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktElement)
{
	short unitType;
	short projCode;
	short parmIdx;
	short parmCode;

	int status = 0;
	size_t nameLength;
	ulong32_t projFlags;

	double *dblPtr;
	const char *cp;
	const char *cp1;
	struct cs_Prjtab_ *pp;
	struct cs_PrjprmMap_ *mp;
	struct cs_Prjprm_ *csPrjPrmPtr;
	// These are all constant pointers to sub-elements of the main wktElement
	// object declared below.  Do not 'delete' these.
	const TrcWktElement* unit = 0;
	const TrcWktElement* datum = 0;
	const TrcWktElement* geogcs = 0;
	const TrcWktElement* authority = 0;
	const TrcWktElement* parameter = 0;
	const TrcWktElement* projection = 0;
	const TrcWktElement* primeMeridian = 0;
	const TrcWktElement* sclRedParm = 0;
	const TrcWktElement* orgLatParm = 0;

	double parmValue;
	double unitsFactor;
	double geogcsUnitsFactor;
	double primeMeridianLongitude;				// in degrees

	char unitName [64];
	char geogcsUnitName [64];
	char reducedName [64];

	// Erase any garbage which may be in the provided structures.
	if (csDef != 0)
	{
		memset (csDef,0,sizeof (*csDef));
	}
	if (dtDef != 0)
	{
		memset (dtDef,0,sizeof (*dtDef));
	}
	if (elDef != 0)
	{
		memset (elDef,0,sizeof (*elDef));
	}

	// If this is a local coordinate system, we use a separate function.
	// Since GEOSGCS is a component of a PROJCS, it is convenient to handle
	// both of those in a single function.   Not so with LOCAL_CS.
	if (wktElement->GetElementType () == rcWktLocalCS)
	{
		status = CSwktToNerth (csDef,elDef,flavor,wktElement);
		return status;
	}

	// We only process PROJCS or GEOGCS objects.
	if (wktElement->GetElementType () != rcWktProjCS && wktElement->GetElementType () != rcWktGeogCS)
	{
		CS_erpt (cs_WKT_WRNGTYP);
		goto error;
	}

	// Process the datum portion.  This does the ellipsoid as well.
	if (dtDef != 0)
	{
		int dtStatus;
		datum = wktElement->ChildSearch (rcWktDatum);
		if (datum == 0)
		{
			CS_erpt (cs_WKT_NODATUM);
			goto error;
		}
		dtStatus = CS_wktEleToDt (dtDef,elDef,flavor,datum);
		if (dtStatus < 0)
		{
			goto error;
		}
		status |= dtStatus;
	}

	// Some stuff common to PROJCS and GEOGCS.
	memset (csDef,'\0',sizeof (*csDef));
	csDef->wktFlvr = static_cast<short>(flavor);
	cp = wktElement->GetElementNameC ();
	CS_stncp (csDef->desc_nm,cp,sizeof (csDef->desc_nm));
	CS_stncp (reducedName,cp,sizeof (reducedName));
	nameLength = strlen (reducedName);
	if (nameLength > cs_KEYNM_MAX)
	{
		CS_wktCsNameFix (reducedName,sizeof (reducedName),reducedName);
	}
	CS_stncp (csDef->key_nm,reducedName,sizeof (csDef->key_nm));

	unitType = (wktElement->GetElementType () == rcWktGeogCS) ? cs_UTYP_ANG : cs_UTYP_LEN;

	// In a ProjCS, there will be two unit specifications.  Must use ChildLocate
	// here to be sure to get the correct one.
	unit = wktElement->ChildLocate (rcWktUnit);
	if (unit == 0)
	{
		CS_erpt (cs_WKT_NOUNIT);
		goto error;
	}
	
	// Convert to CS-MAP form.
	unitsFactor = unit->GetFieldDouble (0);
	if (unitType == cs_UTYP_ANG)
	{
		unitsFactor *= cs_Radian;
	}
	cp = CS_unitluByFactor (unitType,unitsFactor);
	if (cp != 0)
	{
		CS_stncp (unitName,cp,sizeof (unitName));
	}
	else
	{
		// Otherwise, we use the WKT name.  We try to fix it up to make
		// it fit in the array we have.
		cp = unit->GetElementNameC ();
		CS_wktUnitNameFix (unitName,sizeof (unitName),flavor,cp);
	}

	// At this point, we have the best units representation we can get in the
	// local variables named unitsName and unitsFactor.  If the unit name
	// extracted from the WKT is the null string, it will have been reset to
	// <unknown>.  The unitsFactor local variable will have been set to the
	// value extracted from the WKT.
	if (unitsFactor != 0.0)
	{
		// We go with the value of unitsName, whtever it is at this point.
		CS_stncp (csDef->unit,unitName,sizeof (csDef->unit));
	}
	else // if (unitsFactor == 0.0)
	{
		// Couldn't extract a meaningful unit specification from the WKT.
		CS_stncp (csErrnam,unitName,sizeof (csErrnam));
		CS_erpt (cs_WKT_INVUNIT);
		goto error;
	}

	geogcs = (wktElement->GetElementType () == rcWktGeogCS) ? wktElement : wktElement->ChildLocate (rcWktGeogCS);
	if (geogcs == 0)
	{
		CS_erpt (cs_WKT_NOGEOCS);
		goto error;
	}
	unit = geogcs->ChildLocate (rcWktUnit);
	if (unit == 0)
	{
		CS_erpt (cs_WKT_NOGUNIT);
		goto error;
	}
	geogcsUnitsFactor = unit->GetFieldDouble (0) * cs_Radian;
	cp = CS_unitluByFactor (cs_UTYP_ANG,geogcsUnitsFactor);
	if (cp != 0)
	{
		CS_stncp (geogcsUnitName,cp,sizeof (unitName));
	}
	else
	{
		// Otherwise, we use the WKT name.  We try to fix it up to make
		// it fit in the array we have.
		cp = unit->GetElementNameC ();
		CS_wktUnitNameFix (geogcsUnitName,sizeof (geogcsUnitName),flavor,cp);
	}
	if (geogcsUnitsFactor == 0.0)
	{
		CS_stncp (csErrnam,geogcsUnitName,sizeof (csErrnam));
		CS_erpt (cs_WKT_INVGUNIT);
		goto error;
	}

	if (datum != 0)
	{
		CS_stncp (csDef->dat_knm,dtDef->key_nm,sizeof (csDef->dat_knm));
	}

	primeMeridianLongitude = cs_Zero;
	primeMeridian = geogcs->ChildLocate (rcWktPrimeM);
	if (primeMeridian != 0)
	{
		// For ESRI, the prime meridian is always given in degrees.  For
		// Oracle9, Oracle10, and GeoTools, the prime meridian is
		// given in system units.  Don't kno about the other flavors.
		primeMeridianLongitude = primeMeridian->GetFieldDouble (0);
		if (flavor == wktFlvrOracle || flavor == wktFlvrOracle9 || flavor == wktFlvrGeoTools)
		{
			primeMeridianLongitude *= geogcsUnitsFactor;
		}
	}

	csDef->scl_red = cs_One;
	csDef->unit_scl = unitsFactor;
	csDef->map_scl = cs_One;
	csDef->scale = cs_One / csDef->unit_scl;
	csDef->quad = static_cast<short>(CS_wktAxisToQuad (wktElement));

	// Process the easier of the two: GEOGCS.
	if (wktElement->GetElementType () == rcWktGeogCS)
	{
		CS_stncp (csDef->prj_knm,"LL",sizeof (csDef->prj_knm));
		CS_stncp (csDef->group,"LL",sizeof (csDef->group));
		// Best I can tell, Prime Meridian values are always in degrees.
		csDef->org_lng = primeMeridianLongitude;

		// There is no range information in the WKT, so we choose the
		// CS-MAP default by leaving parameters 1 and 2 at zero.
		csDef->zero [0] = csDef->zero [1] = 1.0E-12 / csDef->unit_scl;

		csDef->ll_min [0] = cs_Km180;
		csDef->ll_min [1] = cs_Km90;
		csDef->ll_max [0] = cs_K180;
		csDef->ll_max [1] = cs_K90;

		csDef->xy_min [0] = cs_Km180 / csDef->unit_scl;
		csDef->xy_min [1] = cs_Km90  / csDef->unit_scl;
		csDef->xy_max [0] = cs_K180  / csDef->unit_scl;
		csDef->xy_max [1] = cs_K90   / csDef->unit_scl;

		CS_stncp (dtDef->source,"Extracted from WKT string; description field carries WKT name.",sizeof (dtDef->source));
		// I think we are all done for a GEOGCS.
	}
	else if (wktElement->GetElementType () == rcWktProjCS)
	{
		// Do the stuff that is common to all projected systems.  Remember,
		// we initialized the whole thing to zeros above.  Key_nm, dat_knm,
		// elp_knm, and unit have already been taken care of.
		CS_stncp (csDef->group,"USER",sizeof (csDef->group));
		parameter = wktElement->ParameterLocate (flavor,cs_WKTCOD_FEAST);
		if (parameter != 0) csDef->x_off = parameter->GetFieldDouble (0);
		parameter = wktElement->ParameterLocate (flavor,cs_WKTCOD_FNORTH);
		if (parameter != 0) csDef->y_off = parameter->GetFieldDouble (0);
		parameter = wktElement->ParameterLocate (flavor,cs_WKTCOD_ORGLNG);
		if (parameter == 0)
		{
			parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_CNTMER);
		}
		if (parameter == 0)
		{
			parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_GCPLNG);
		}
		if (parameter != 0)
		{
			parmValue = parameter->GetFieldDouble (0);
			parmValue *= geogcsUnitsFactor;					// convert to degrees
			parmValue += primeMeridianLongitude;			// adjust to greenwich
			// Adjust for possible noise in the least significant bits.
			if (fabs (fabs (parmValue) - 180.0) < 1.0E-12)
			{
				parmValue = (parmValue < 0.0) ? -180.0 : 180.0;
			}
			csDef->org_lng = parmValue;
		}
		parameter = wktElement->ParameterLocate (flavor,cs_WKTCOD_ORGLAT);
		if (parameter == 0)
		{
			parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_GCPLAT);
		}
		if (parameter != 0)
		{
			parmValue = parameter->GetFieldDouble (0);
			parmValue *= geogcsUnitsFactor;
			// Adjust for possible noise in the least significant bits.
			if (fabs (fabs (parmValue) - 90.0) < 1.0E-12)
			{
				parmValue = (parmValue < 0.0) ? -90.0 : 90.0;
			}
			csDef->org_lat = parmValue;
		}
		parameter = wktElement->ParameterLocate (flavor,cs_WKTCOD_SCLRED);
		if (parameter != 0)
		{
			csDef->scl_red = parameter->GetFieldDouble (0);
		}
		csDef->zero [0] = csDef->zero [1] = 0.001 / csDef->unit_scl;

		projection = wktElement->ChildSearch (rcWktProjection);
		if (projection == 0)
		{
			CS_erpt (cs_WKT_NOPROJ);
			goto error;	
		}
		cp = projection->GetElementNameC ();
		projCode = CS_wktProjLookUp (flavor,cp);
		if (projCode == cs_PRJCOD_END)
		{
			CS_stncp (csErrnam,cp,sizeof (csErrnam));
			CS_erpt (cs_WKT_INVPROJ);
			goto error;
		}

		// Get a pointer to the CS-MAP projection table entry for the indicated
		// projection.  We can do some more general stuff once we have that.
		for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
		{
			if (pp->code == projCode) break;
		}
		if (*pp->key_nm == '\0' || pp->setup == NULL || pp->code == cs_PRJCOD_END)
		{
			CS_stncp (csErrnam,cp,sizeof (csErrnam));
			CS_erpt (cs_WKT_INVPROJ);
			goto error;
		}
		projFlags = pp->flags;

		// Reject certain projections of certain flavors which are known to be
		// problematical; at least for now.
		//
		// Krovak: The parameterization used by others is not the same as CS-MAP.
		//
		if (projCode == cs_PRJCOD_KRVK95 && flavor != wktFlvrAutodesk)
		{
			CS_stncp (csErrnam,cp,sizeof (csErrnam));
			CS_erpt (cs_WKT_PRJSUPRT);
			goto error;
		}

		// Special kludge.  In certain flavors of WKT, the projection name
		// "Lambert_Conformal_Conic" is used for both the 1 pararllel and
		// 2 parallel variations.  The NameMapper will always return
		// cs_PROJCOD_LM2SP as it is the most frequently used projection.
		if ((flavor == wktFlvrEsri   ||
			 flavor == wktFlvrOracle ||
			 flavor == wktFlvrOracle9) &&
			(projCode == cs_PRJCOD_LM1SP ||
			 projCode == cs_PRJCOD_LM2SP)
		   )
		{
			// Here if there is a potential problem.  We determine which
			// variation of the Lambert Conformal Conic based on the
			// number and type of parameters available.
			parmCode = cs_PRMCOD_SSTDPLL;
			parameter = wktElement->ParameterLocate (flavor,parmCode);
			projCode = (parameter == 0) ? cs_PRJCOD_LM1SP : cs_PRJCOD_LM2SP;

			// Reset the projection table pointer.
			for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
			{
				if (pp->code == projCode) break;
			}
			if (*pp->key_nm == '\0' || pp->setup == NULL || pp->code == cs_PRJCOD_END)
			{
				// This should never happen.
				CS_stncp (csErrnam,cp,sizeof (csErrnam));
				CS_erpt (cs_WKT_INVPROJ);
				goto error;
			}
		}

		// Another kludge.  In certain flavors of WKT, the same name is used
		// Polar Stereographic and Polar Stereographic with Standard Parallel.
		// The projection mapping code will always return cs_PRJCOD_PSTEREO
		// as that is the projection name used in the WKT.  Here we see if the
		// definition also includes a Standard Parallel 1 parameter and if
		// so, switch the projection code to cs_PSRJCOD_PSTROSL.
		//
		// Also, many flavors like to use Stereographic for all sorts of things.
		// I'm just beginning to understand the why's and wherefore's.
		// In any case, if we have parsed a Polar Stereographic, it could
		// easily mean that it is an oblique stereographic.  This can't be
		// handled in the mapping tables, as the mapping tables don't know
		// about the parameters.  Thus, we do it here in a rather elaborate
		// kludge.
		if ((flavor == wktFlvrEsri || flavor == wktFlvrOracle) &&
			(projCode == cs_PRJCOD_PSTRO)
		   )
		{
			parmValue = -90.0;
			sclRedParm = wktElement->ParameterLocate (flavor,cs_WKTCOD_SCLRED);
			orgLatParm = wktElement->ParameterLocate (flavor,cs_WKTCOD_ORGLAT);
			if (orgLatParm != 0)
			{
				parmValue = orgLatParm->GetFieldDouble (0);
				parmValue *= geogcsUnitsFactor;
			}
			if (fabs (fabs (parmValue) - 90.0) > 1.0E-02)			
			{
				// We have an Oblique Stereographic.  Note, this maps to what most of
				// the world calls the Double Stereographic.
				projCode = cs_PRJCOD_OSTRO;
			}
			else
			{
				parmCode = cs_PRMCOD_NSTDPLL;
				parameter = wktElement->ParameterLocate (flavor,parmCode);
				projCode = (parameter == 0) ? cs_PRJCOD_PSTRO : cs_PRJCOD_PSTROSL;

				// Also, these flavors of WKT often refer to (eg)
				// "Stereographic_South_Pole" as a projection, thus the origin
				// latitude is implied in the projection name.  We try to deal with
				// that here.
				cp = projection->GetElementNameC ();
				if (cp != 0)
				{
					cp1 = CS_stristr (cp,"South");
					if (cp1 != 0)
					{
						csDef->org_lat = -90.0;
					}
					cp1 = CS_stristr (cp,"North");
					if (cp1 != 0)
					{
						csDef->org_lat = 90.0;
					}
				}
			}

			// Reset the projection table pointer.
			for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
			{
				if (pp->code == projCode) break;
			}
			if (*pp->key_nm == '\0' || pp->setup == NULL || pp->code == cs_PRJCOD_END)
			{
				// This should never happen.
				CS_stncp (csErrnam,cp,sizeof (csErrnam));
				CS_erpt (cs_WKT_INVPROJ);
				goto error;
			}
		}

		// Another special kludge.  This is to handle ESRI's (and others I
		// presume) use of the "Hotine_Oblique_Mercator_Azimuth_Center"
		// projection to do what is really the job of the "Oblique Cylindrical"
		// projection.  Essentially, if the above produced any variation of
		// the Oblique Mercator, and the azimuth value is +/- 90.0; then
		// we switch to the either the Swiss or Oblique Cylindrical, based
		// on whether there is a "Standard Parallel" parameter or not.  I don't
		// know that I've ever seen a standard parallel in this case, but maybe
		// I can prevent a patch by puting this in.
		if ((projCode >= cs_PRJCOD_RSKEW && projCode <= cs_PRJCOD_RSKEWO) ||
			 projCode == cs_PRJCOD_HOM1XY)
		{
			// One of the single point rectified Oblique Mercator specifications.
			// There must be an azimuth parameter.  If there isn't, then none of
			// this applies.
			parmCode = cs_PRMCOD_GCAZM;
			parameter = wktElement->ParameterLocate (flavor,parmCode);
			if (parameter != 0)
			{
				parmValue = parameter->GetFieldDouble (0);
				parmValue *= geogcsUnitsFactor;
				if (fabs (fabs (parmValue) - 90.0) < 1.0E-04)
				{
					// OK. The azimuth value is present, and it is +/-
					// 90 degrees.  We switch to either the Swiss Cylindrical
					// or Oblique Cylindrical projection depending upon
					// the existence of a standard parallel parameter.
					parmCode = cs_PRMCOD_NRMLPLL;
					parameter = wktElement->ParameterLocate (flavor,parmCode);
					projCode = (parameter == 0) ? cs_PRJCOD_SWISS : cs_PRJCOD_OBQCYL;
					for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
					{
						if (pp->code == projCode) break;
					}
				}
			}
		}

		// OK, back to normal.
		CS_stncp (csDef->prj_knm,pp->key_nm,sizeof (csDef->prj_knm));
		projFlags = pp->flags;

		for (mp = cs_PrjprmMap;mp->prj_code != cs_PRJCOD_END;mp += 1)
		{
			if (mp->prj_code == projCode) break;
		}
		if (mp->prj_code == cs_PRJCOD_END)
		{
			CS_stncp (csErrnam,"CS_wellKnownText.cpp:1",MAXPATH);
			CS_erpt (cs_ISER);
			goto error;
		}

		// Now for each possible parameter.
		for (parmIdx = 0;parmIdx < 24;parmIdx += 1)
		{
			parmCode = mp->prm_types [parmIdx];
			if (parmCode <= 0) break;
			parameter = wktElement->ParameterLocate (flavor,parmCode);
			// Some flavors of WKT use Standard_Parallel_1 whenever
			// only one parallel is in use.
			if (parameter == 0 && parmCode == cs_PRMCOD_STDPLL)
			{
				parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_NSTDPLL);
			}
			// Some flavors of WKT use Standard_Parallel_1 for a standard
			// circle.
			if (parameter == 0 && parmCode == cs_PRMCOD_STDCIR)
			{
				parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_NSTDPLL);
			}
			if (parameter != 0)
			{
				dblPtr = &csDef->prj_prm1 + parmIdx;
				parmValue = parameter->GetFieldDouble (0);
				// If the logical type of this parameter is of the angle logical type,
				// we need to adjust by the geogcs unitsto convert to degrees.
				csPrjPrmPtr = &csPrjprm [parmCode];
				if (csPrjPrmPtr->log_type == cs_PRMLTYP_LNG ||
					csPrjPrmPtr->log_type == cs_PRMLTYP_LAT ||
					csPrjPrmPtr->log_type == cs_PRMLTYP_AZM ||
					csPrjPrmPtr->log_type == cs_PRMLTYP_ANGD)
				{
					if (flavor != wktFlvrGeoTools)
					{
						parmValue *= geogcsUnitsFactor;
					}
				}
				if (csPrjPrmPtr->log_type == cs_PRMLTYP_LNG)
				{
					// Some flavors don't support this feature which is
					// considered, by most, to be standard.
					if (flavor != wktFlvrGeoTools)
					{
						parmValue += primeMeridianLongitude;
					}
					
					// GetFieldDouble uses atof to convert the ASCII data in
					// the WKT string to double form.  Often, there is some
					// extraneous noise in the conversion.  This raises some
					// havoc (for a longitude) if a 180.0 is actually
					// converted to (for example) 180.0000000000014.  We deal
					// with this here since we know the parameter value is
					// a longitude value.
					if (fabs (fabs (parmValue) - 180.0) < 1.0E-12)
					{
						parmValue = (parmValue < 0.0) ? -180.0 : 180.0;
					}
				}
				if (csPrjPrmPtr->log_type == cs_PRMLTYP_LAT)
				{
					// As above for longitudes, we adjust latitudes to
					// account for some noise in the least sugnificant
					// bits.
					if (fabs (fabs (parmValue) - 90.0) < 1.0E-12)
					{
						parmValue = (parmValue < 0.0) ? -90.0 : 90.0;
					}
				}
				*dblPtr = parmValue;
			}
		}

		// The following will enforce the CS-MAP convention that for
		// conic projections with two standard parallels, prm1 is the
		// northern parallel and prm2 is the southern parallel.  The
		// projection mathemagics doesn't care, but the coordinate system
		// compariosn functions do care.
		CSwktConicAdj (csDef);

		// The above is supposed to work for all projections, all parameters.
		// When working with WKT, we know better than that.  Here below we
		// make provisions for twiddleing as necessary on a projection by
		// projection basis.
		switch (projCode) {
		case cs_PRJCOD_TRMER:
			break;
		case cs_PRJCOD_LM1SP:
			// The following verifies that if present, the Northern Standard Parallel
			// value is the same as the origin latitude.
			parmValue = csDef->org_lat;
			parameter = wktElement->ParameterLocate (flavor,cs_PRMCOD_NSTDPLL);
			if (parameter != 0)
			{
				parmValue = parameter->GetFieldDouble (0);
				parmValue *= geogcsUnitsFactor;
			}
			if (!CS_cmpDbls (parmValue,csDef->org_lat))
			{
				CS_stncp (csErrnam,csDef->key_nm,sizeof (csErrnam));
				CS_erpt (cs_WKT_PRMSUPRT);
				goto error;
			}
			break;
		case cs_PRJCOD_LM2SP:
			break;
		case cs_PRJCOD_LMBLG:
			break;
		case cs_PRJCOD_ALBER:
			break;
		case cs_PRJCOD_MRCAT:
			break;
		case cs_PRJCOD_MRCATPV:
			break;
		case cs_PRJCOD_AZMED:
			break;
		case cs_PRJCOD_LMTAN:
			break;
		case cs_PRJCOD_PLYCN:
			break;
		case cs_PRJCOD_MODPC:
			break;
		case cs_PRJCOD_AZMEA:
			break;
		case cs_PRJCOD_EDCNC:
			break;
		case cs_PRJCOD_MILLR:
			break;
		case cs_PRJCOD_MSTRO:
			break;
		case cs_PRJCOD_NZLND:
			break;
		case cs_PRJCOD_SINUS:
			break;
		case cs_PRJCOD_ORTHO:
			break;
		case cs_PRJCOD_GNOMC:
			break;
		case cs_PRJCOD_EDCYL:
		case cs_PRJCOD_EDCYLE:
			break;
		case cs_PRJCOD_VDGRN:
			break;
		case cs_PRJCOD_CSINI:
			break;
		case cs_PRJCOD_ROBIN:
			break;
		case cs_PRJCOD_BONNE:
			break;
		case cs_PRJCOD_EKRT4:
			break;
		case cs_PRJCOD_EKRT6:
			break;
		case cs_PRJCOD_MOLWD:
			break;
		case cs_PRJCOD_HMLSN:
			break;
		case cs_PRJCOD_NACYL:
			break;
		case cs_PRJCOD_TACYL:
			break;
		case cs_PRJCOD_BPCNC:
			break;
		case cs_PRJCOD_SWISS:
			break;
		case cs_PRJCOD_PSTRO:
			break;
		case cs_PRJCOD_OSTRO:
			break;
		case cs_PRJCOD_SSTRO:
			break;
		case cs_PRJCOD_HOM1XY:
			break;
		case cs_PRJCOD_HOM2XY:
			break;
        case cs_PRJCOD_WCCSL:
			break;
        case cs_PRJCOD_WCCST:
			break;
        case cs_PRJCOD_MNDOTL:
			break;
        case cs_PRJCOD_MNDOTT:
			break;
        case cs_PRJCOD_SOTRM:
			break;
        case cs_PRJCOD_UTM:
			break;
        case cs_PRJCOD_TRMRS:
			break;
        case cs_PRJCOD_GAUSSK:
			break;
        case cs_PRJCOD_KROVAK:
			status = CSwellKnownTextKrovak (csDef,elDef,wktElement,primeMeridianLongitude,
																   geogcsUnitsFactor,
																   flavor);
			if (status != 0)
			{
				goto error;
			}
			break;
        case cs_PRJCOD_MRCATK:
			break;
        case cs_PRJCOD_KRVK95:
			break;
        case cs_PRJCOD_PSTROSL:
			break;
        case cs_PRJCOD_TRMERAF:
			break;
        case cs_PRJCOD_NERTH:
			break;
        case cs_PRJCOD_OBQCYL:
			break;
        case cs_PRJCOD_SYS34:
			break;
        case cs_PRJCOD_OSTN97:
			break;
        case cs_PRJCOD_AZEDE:
			break;
        case cs_PRJCOD_OSTN02:
			break;
        case cs_PRJCOD_SYS34_99:
			break;
        case cs_PRJCOD_TRMRKRG:
			break;
        case cs_PRJCOD_WINKL:
			break;
        case cs_PRJCOD_NRTHSRT:
			break;
        case cs_PRJCOD_LMBRTAF:
			break;
        case cs_PRJCOD_RSKEW:
			break;
        case cs_PRJCOD_RSKEWC:
			break;
        case cs_PRJCOD_RSKEWO:
			break;
		case cs_PRJCOD_PCARREE:
			break;
		default:
			break;
		}
	}

	authority = wktElement->ChildLocate (rcWktAuthority);
	if (authority != 0)
	{
		long32_t epsgCode;

		cp = authority->GetElementNameC ();
		if (cp == 0 || *cp == '\0') cp = "EPSG";
		epsgCode = authority->GetFieldLong (0);
		csDef->epsgNbr = static_cast<short>(epsgCode);
		sprintf (reducedName,"%s:%d",cp,(int)epsgCode);
		CS_stncp (csDef->key_nm,reducedName,sizeof (csDef->key_nm));
	}

	CS_stncp (csDef->source,"Extracted from WKT string; description field carries WKT name.",sizeof (csDef->source));

	// Hopefully, that's it.
	if ( strlen (reducedName) > cs_KEYNM_MAX) status |= cs_CS2WKT_NMTRUNC;
	return status;
error:
	// Delete, free, whatever stuff is necessary; none just yet.
	return -1;
}
void CSwktConicAdj (struct cs_Csdef_ *csDef)
{
	unsigned short projCode = cs_PRJCOD_END;
	struct cs_Prjtab_ *pp;
	double dblTmp;

	for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
	{
		if (!CS_stricmp (pp->key_nm,csDef->prj_knm))
		{
			projCode = pp->code;
			break;
		}
	}
	if (projCode == cs_PRJCOD_LM2SP   ||
		projCode == cs_PRJCOD_LMBLG   ||
		projCode == cs_PRJCOD_WCCSL   ||
		projCode == cs_PRJCOD_MNDOTL  ||
		projCode == cs_PRJCOD_ALBER   ||
		projCode == cs_PRJCOD_EDCNC   ||
		projCode == cs_PRJCOD_LMBRTAF)
	{
		// This is a conic where the first two parameters are the northern and
		// southern standard parallels.  The mathematics of the projection
		// does not care about thje order.  However, the compariosn functions
		// do.  So, when we read a WKT, we like to make sure the prm1 is indeed
		// the northern parallel and prm2 is indeed the southern parallel.
		if (!CS_cmpDbls (csDef->prj_prm1,csDef->prj_prm2))
		{
			// The two parallels are not the same, so it is valid
			// to do the following test.
			if (csDef->prj_prm1 < csDef->prj_prm2)
			{
				// OK, we need to swap to honor the CS-MAP convention that
				// prj_prm1 is the northern parallel and prm2 is the
				// southern parallel.
				dblTmp = csDef->prj_prm1;
				csDef->prj_prm1 = csDef->prj_prm2;
				csDef->prj_prm2 = dblTmp;
			}
		}
	}
	return;
}
int CSwktToNerth (struct cs_Csdef_ *csDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktElement)
{
	int status;
	short unitType;

	size_t nameLength;

	const TrcWktElement* unit = 0;
	const TrcWktElement* authority = 0;

	const char *cp;

	double unitsFactor;

	char unitName [64];
	char reducedName [64];

	// Setup the ellipsoid which may be present.  It usually is not.
	status = CS_wktEleToEl (elDef,flavor,wktElement);
	if (status < 0)
	{
		// There was no ellipsoid in the LOCAL_CS WKT string.  Simply
		// provide a copy of WGS84.
		struct cs_Eldef_* elDefPtr = CS_eldef ("WGS84");
		if (elDefPtr != 0)
		{
			memcpy (elDef,elDefPtr,sizeof (*elDef));
			CS_free (elDefPtr);
			elDefPtr = NULL;
			status = 0;
		}
		else
		{
			goto error;
		}
	}

	// Set up the relatively easy stuff, such as name and description.
	memset (csDef,'\0',sizeof (*csDef));
	csDef->wktFlvr = static_cast<short>(flavor);
	cp = wktElement->GetElementNameC ();
	CS_stncp (csDef->desc_nm,cp,sizeof (csDef->desc_nm));
	CS_stncp (csDef->source,"Extracted from WKT string; description field carries WKT name.",sizeof (csDef->source));
	CS_stncp (csDef->prj_knm,"NERTH",sizeof (csDef->prj_knm));
	CS_stncp (csDef->group,"USER",sizeof (csDef->group));

	CS_stncp (reducedName,cp,sizeof (reducedName));
	nameLength = strlen (reducedName);
	if (nameLength > cs_KEYNM_MAX)
	{
		CS_wktCsNameFix (reducedName,sizeof (reducedName),reducedName);
	}
	CS_stncp (csDef->key_nm,reducedName,sizeof (csDef->key_nm));

	// Set the units.
	unitType = cs_UTYP_LEN;
	unit = wktElement->ChildLocate (rcWktUnit);			// using ChildSearch here is bad news 
	if (unit == 0)
	{
		CS_erpt (cs_WKT_NOUNIT);
		goto error;
	}
	unitsFactor = cs_Zero;
	cp = unit->GetElementNameC ();
	CS_wktUnitNameFix (unitName,sizeof (unitName),flavor,cp);
	if(CS_stricmp (unitName,"<unknown>") == 0)
	{
		// Unit name is not known to CS-MAP.  See if the WKT name without
		// flavor mapping works.
		CS_stncp (unitName,cp,sizeof (unitName));
		unitsFactor = CS_unitlu (unitType,unitName);
	}
	else
	{
		unitsFactor = CS_unitlu (unitType,unitName);
	}

	if (unitsFactor == 0.0)
	{
		// Still no match.  Lets try matching up by the factor value.
		unitsFactor = unit->GetFieldDouble (0);
		cp = CS_unitluByFactor (unitType,unitsFactor);
		if (cp != 0)
		{
			// OK, we have a factor and a name.
			CS_stncp (unitName,cp,sizeof (unitName));
		}
		else
		{
			unitsFactor = 0.0;
		}
	}
	if (unitsFactor == 0.0)
	{
		CS_erpt (cs_WKT_NOUNIT);
		goto error;
	}
	CS_stncp (csDef->unit,unitName,sizeof (csDef->unit));

	//Continue on with the standard stuff.
	csDef->unit_scl = unitsFactor;
	csDef->scl_red = cs_One;
	csDef->map_scl = cs_One;
	csDef->scale = cs_One / csDef->unit_scl;
	csDef->quad = static_cast<short>(CS_wktAxisToQuad (wktElement));
	csDef->zero [0] = csDef->zero [1] = 0.001 / csDef->unit_scl;

	// Maybe, we set the four parameters as well.  Not Yet!!!

	// Not likely, but maybe an Authority is present.
	authority = wktElement->ChildLocate (rcWktAuthority);
	if (authority != 0)
	{
		long32_t epsgCode;

		cp = authority->GetElementNameC ();
		if (cp == 0 || *cp == '\0') cp = "EPSG";
		epsgCode = authority->GetFieldLong (0);
		csDef->epsgNbr = static_cast<short>(epsgCode);
		sprintf (reducedName,"%s:%d",cp,(int)epsgCode);
		CS_stncp (csDef->key_nm,reducedName,sizeof (csDef->key_nm));
	}

	return status;
error:
	return -1;
}
int EXP_LVL1 CS_wktToDict (const char *csKeyName,const char *dtKeyName,const char *elKeyName,const char *wellKnownText,int flavor)
{
	int rtnValue = -1;

	struct cs_Csdef_ csDef;
	struct cs_Dtdef_ dtDef;
	struct cs_Eldef_ elDef;

	rtnValue = CS_wktToCs (&csDef,&dtDef,&elDef,(ErcWktFlavor)flavor,wellKnownText);
	if (rtnValue >= 0)
	{
		if (elKeyName != 0 && *elKeyName != '\0')
		{
			CS_stncp (elDef.key_nm,elKeyName,sizeof (elDef.key_nm));
			rtnValue = CS_elupd (&elDef,0);
		}
		if (rtnValue == 0 && dtKeyName != 0 && *dtKeyName != '\0')
		{
			CS_stncp (dtDef.key_nm,dtKeyName,sizeof (dtDef.key_nm));
			rtnValue = CS_dtupd (&dtDef,0);
		}
		if (rtnValue == 0 && csKeyName != 0 && *csKeyName != '\0')
		{
			CS_stncp (csDef.key_nm,csKeyName,sizeof (csDef.key_nm));
			rtnValue = CS_csupd (&csDef,0);
		}
	}
	return rtnValue;
}
//newPage//
// Use this function to extract a cs_Dtdef_ and cs_Eldef_ structure from a
// pre-parsed DATUM WKT element.  If the provided definition does not have
// a TOWGS84 sub-element, a default datum definition with the datum and
// ellipsoid names set, but a to84_via value of cs_DTCTYP_NONE.
//
// Normally, a zero value is returned to indicate success.  A negative
// value is returned for a hard failure, use CS_errmsg to get a desrciption.
// The returned value, if positive, will have the cs_DT2WKT_DTDEF bit set in
// the event that the datum name was that of a datum already defined in the
// CS-MAP dictionary, and that very definition is returned in the location
// pointed to by the dtDef argument.  The cs_DT2WKT_NODEF is set in a positive
// return value to indicate that there was no specific datum definition in the
// WKT string and a dictionary lookup on the datum name provided failed.  That
// is, the  returned if the cs_DT2WKT_NODEF bit is set if there is no real
// datum definition in the WKT string, which is quite common.
//
// The cs_DT2WKT_NMTRUNC bit is set in a positive return value if the adjusted
// datum name was longer than what a CS-MAP key name can accomodate, i.e. the
// key name field contains a truncated value.  Note, if an EPSG number is available,
// the name will have the form of EPSG:####, and a zero value will be returned.
// It is assumed that an Authority WKT element will carry the appropriate
// EPSG number.
//
// The following function is, usually, not given a pointer to a full element;
// only the Datum element of a larger definition.  Thus, it is erroneous to
// expect this function to be able to determine the flavor of the WKT string
// being processed.
int CS_wktEleToDt (struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,
										   ErcWktFlavor flavor,
										   const TrcWktElement* wktPtr)
{
	int status = 0;
	const char *cp;
	const char* namePtr;
	char datumName [64];

	// These are all constant pointers to sub-elements of the main wktElement
	// object declared below.  Do not delete these.
	const TrcWktElement* spheroid;
	const TrcWktElement* toWgs84;
	const TrcWktElement* authority;

	double scaleDelta;

	struct cs_Eldef_ myElDef;
	memset (&myElDef,0,sizeof (myElDef));

	if (elDef == 0) elDef = &myElDef;
	if (dtDef != 0)
	{
		memset (dtDef,'\0',sizeof (*dtDef));
	}
	if (wktPtr == 0)
	{
		// We report this as fatal, it's not supposed to happen.
		CS_erpt (cs_WKT_NODATUM);
		goto error;
	}

	// We only process DATUM objects.
	if (wktPtr->GetElementType () != rcWktDatum)
	{
		CS_erpt (cs_WKT_WRNGTYP);
		goto error;
	}

	// Take care of the ellipsoid portion.  This is rather simple, and applies
	// to most any WKT object.  ChildSearch searches the entire tree structure
	// of the element.  CS_wktEleToEl can handle a null pointer correctly.
	spheroid = wktPtr->ChildSearch (rcWktSpheroid);
	status |= CS_wktEleToEl (elDef,flavor,spheroid);
	if (status < 0)
	{
		goto error;
	}

	// Some basic stuff in preparation for doing the cs_Dtdef_ portion.
	cp = wktPtr->GetElementNameC ();
	CS_stncp (dtDef->name,cp,sizeof (dtDef->name));
	CS_wktDtNameFix (datumName,sizeof (datumName),cp);
	CS_stncp (dtDef->key_nm,datumName,sizeof (dtDef->key_nm));

	// Set the stuff independent of the TOWGS84 element.
	CS_stncp (dtDef->ell_knm,elDef->key_nm,sizeof (dtDef->ell_knm));
	dtDef->group  [0]   = '\0';
	dtDef->locatn [0]   = '\0';
	dtDef->cntry_st [0] = '\0';
	dtDef->delta_X = cs_Zero;
	dtDef->delta_Y = cs_Zero;
	dtDef->delta_Z = cs_Zero;
	dtDef->rot_X   = cs_Zero;
	dtDef->rot_Y   = cs_Zero;
	dtDef->rot_Z   = cs_Zero;
	dtDef->bwscale = cs_Zero;
	dtDef->source [0]   = '\0';
	dtDef->to84_via = cs_DTCTYP_NONE;
	dtDef->protect = 0;
	dtDef->epsgNbr = 0;
	dtDef->wktFlvr = static_cast<short>(flavor);

	// Process the TOWGS84 element if its there.  For Oracle flavor,
	// the datum shift parameters may be tacked on to the DATUM
	// element.
	toWgs84 = wktPtr->ChildSearch (rcWktToWgs84);
	if (toWgs84 == 0 && flavor == wktFlvrOracle)
	{
	    // Oracle WKT will, at times, include a datum transformation
	    // definition in the DATUM element.  This consists of a list
	    // of real numbers following the datum name.  I've never seen
	    // a definition with only three values, but WKT being what it
	    // is, this may appear at some time.
	    unsigned dtValueCount = wktPtr->GetValueCount ();
	    if (dtValueCount == 3 || dtValueCount == 7)
	    {
	        toWgs84 = wktPtr;
	    }
	}
	if (toWgs84 != 0)
	{
	    // Extract the parameters.  GetFieldDouble will return a zero if
	    // the requested value is not present.
		dtDef->delta_X = toWgs84->GetFieldDouble (0);
		dtDef->delta_Y = toWgs84->GetFieldDouble (1);
		dtDef->delta_Z = toWgs84->GetFieldDouble (2);
		dtDef->rot_X   = toWgs84->GetFieldDouble (3);
		dtDef->rot_Y   = toWgs84->GetFieldDouble (4);
		dtDef->rot_Z   = toWgs84->GetFieldDouble (5);
		dtDef->bwscale = toWgs84->GetFieldDouble (6);

		if (flavor == wktFlvrOracle && dtDef->bwscale != 0.0)
		{
			// Our experience with Oracle prior to release 11 suggests that
			// they actually put the actual scale factor in the parameter
			// string, rather than the "signed parts per million" value that
			// is required by the cs_Dtdef_ structure and used in the samples
			// of WKT that have TOWGS84 elements.  APpears to be OK in
			// Oracle 11.  So what's a poor WKT reader supposed to do???
			
			// Generally, elsewhere in the WKT processor, we ignore these
			// TOPWGS84 numbers anyway; so here we'll make a guess as to
			// how to deal with this number.
			scaleDelta = fabs (dtDef->bwscale - cs_One);
			if (scaleDelta < 1.0E-03)
			{
				// We presume that the actual scale value was provided.
				scaleDelta = (dtDef->bwscale - cs_One) * 1.0E+06;
			}
			// Otherwise, we assume the correct parts per million value was supplied.
			// This is usualy the case for Oracle 11; all bets are off for
			// Oracle 9 & 10.
		}

		// Set the appropriate method value.
		int rotCnt = 0;
		if (dtDef->rot_X != 0.0) rotCnt += 1;
		if (dtDef->rot_Y != 0.0) rotCnt += 1;
		if (dtDef->rot_Z != 0.0) rotCnt += 1;

		int deltaCnt = 0;
		if (dtDef->delta_X != 0.0) deltaCnt += 1;
		if (dtDef->delta_Y != 0.0) deltaCnt += 1;
		if (dtDef->delta_Z != 0.0) deltaCnt += 1;

		if (rotCnt != 0)
		{
		    dtDef->to84_via = cs_DTCTYP_BURS;
		}
		else if (deltaCnt != 0)
		{
		    dtDef->to84_via = cs_DTCTYP_GEOCTR;
		}
		else
		{
		    dtDef->to84_via = cs_DTCTYP_WGS84;
		}
		CS_stncp (dtDef->source,"Extracted from WKT string; description field carries WKT name.",sizeof (dtDef->source));
	}
	else
	{
		// There was no TOWGS84 element, so this WKT definition has no
		// datum definition.  If it doesn't have a name, we report it as an
		// error.  Otherwise, we let the calling module deal with it.
		namePtr = wktPtr->GetElementNameC ();
		if (namePtr == 0)
		{
			// This doesn't work if the datum doesn't have a name.
			CS_erpt (cs_WKT_NODTMNM);
			goto error;
		}
		status |= cs_DT2WKT_NODEF;
		CS_stncp (dtDef->source,"Name extracted from WKT, no definition present.",sizeof (dtDef->source));
	}

	// See if this WKT datum has an AUTHORITY element.  If so, there may
	// be a valid EPSG datum number in it.  This is valuable information.
	// We save it in epsgNbr for the calling module.
	authority = wktPtr->ChildLocate (rcWktAuthority);
	if (authority != 0)
	{
		long32_t epsgCode;

		cp = authority->GetElementNameC ();
		// If there is no Autority specified, we'll assume EPSG as that's the
		// only AUTHORITY we've ever seen used.
		if (cp == 0 || *cp == '\0') cp = "EPSG";
		if (!CS_stricmp (cp,"EPSG"))
		{
			// We've only seen EPSG used as an authority.  Wouldn't know what
			// to do with any other.
			epsgCode = authority->GetFieldLong (0);
			dtDef->epsgNbr = static_cast<short>(epsgCode);
			sprintf (datumName,"%s:%d",cp,(int)epsgCode);
			CS_stncp (dtDef->key_nm,datumName,sizeof (dtDef->key_nm));
		}
	}

	// Hopefully, that's it.
	if (strlen (datumName) > cs_KEYNM_MAX) status |= cs_DT2WKT_NMTRUNC;
	return status;
error:
	// Delete, free, whatever stuff is necessary; none just yet.
	return -1;
}
//newPage//
// Use this function to extract a cs_Eldef_ structure from a pre-parsed
// SPHEROID WKT element.  This is pretty straight forward stuff.
//
// The following function returns the MSI equivalent to a WKT ellipsoid
// definition.  No mapping occurs.  The returned value is a faithful
// rendition (in CS-MAP form) of what was in the WKT string.  We can do this
// as the WKT definition of ellipsoids is always complete, and there are no
// mapping problems such as unsupported projections.
int CS_wktEleToEl (struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktPtr)
{
	int status = 0;
	const char *cp;
	double tempValue;
	char ellipsoidName [64];
	const TrcWktElement* authority;

	memset (elDef,'\0',sizeof (*elDef));
	if (wktPtr == 0)
	{
		// We deal with the case where the calling module didn't find a
		// SPHEROID element.	
		CS_stncp (elDef->key_nm,"WGS84",sizeof (elDef->key_nm));
		elDef->group [0] = '\0';
		CS_stncp (ellipsoidName,"<unspecified, defaulted to WGS84>",sizeof (ellipsoidName));
		CS_stncp (elDef->name,ellipsoidName,sizeof (elDef->name));
		CS_stncp (elDef->source,"<definition not supplied>",sizeof (elDef->source));
		elDef->e_rad = cs_Wgs84_aa;
		elDef->p_rad = cs_Wgs84_aa * (cs_One - cs_Wgs84_ff);
		elDef->flat  = cs_Wgs84_ff;
		elDef->ecent = sqrt (cs_Wgs84_esq);
		elDef->protect = 0;
		elDef->epsgNbr = 7030;
		elDef->wktFlvr = static_cast<short>(flavor);
	}
	else
	{
		// We only process SPHEROID objects.
		if (wktPtr->GetElementType () != rcWktSpheroid)
		{
			CS_erpt (cs_WKT_WRNGTYP);
			goto error;
		}

		elDef->p_rad = cs_Zero;
		elDef->flat  = cs_Zero;
		elDef->ecent = cs_Zero;

		// Set the ellipsoid key name to the first 23 characters of the WKT
		// spheroid name.  It is likely that this will be adjusted below.
		cp = wktPtr->GetElementNameC ();

		// Save the entire WKT ellipsoid name in the descriptive name field
		// of the ellipsoid definition structure.  We will want to have this
		// available to us later on.
		CS_stncp (elDef->name,cp,sizeof (elDef->name));

		// Use some standard trickery to reduce the size of the ellipsoid
		// name to the 23 character maximum supported by CS-MAP.
		CS_wktElNameFix (ellipsoidName,sizeof (ellipsoidName),cp);
		CS_stncp (elDef->key_nm,ellipsoidName,sizeof (elDef->key_nm));

		// Extract the numeric values from the WKT element.  tempValue
		// is the reciprocal of the flattening.
		elDef->e_rad = wktPtr->GetFieldDouble (0);
		tempValue  = wktPtr->GetFieldDouble (1);

		if (elDef->e_rad < cs_One)
		{
			// Something is very wrong.
			CS_erpt (cs_WKT_NOELLIP);
			goto error;
		}

		// Some flavors of WKT use a huge value for the reciprocal of
		// flattening to indicate a sphere instead of an ellipsoid.  We
		// try to honor that here.
		if (tempValue > 1.0E+03)
		{
			elDef->p_rad = elDef->e_rad;
			elDef->ecent = cs_Zero;
			elDef->flat = cs_Zero;
		}

		// Since tempValue is the reciprocal of the flattening, we need to
		// divide by it.  Make sure we don't divide by zero.
		else if (tempValue > 0.1)
		{
			elDef->flat  = cs_One / tempValue;
			elDef->p_rad = elDef->e_rad * (cs_One - elDef->flat);
			tempValue = ((elDef->e_rad * elDef->e_rad) - (elDef->p_rad * elDef->p_rad));
			tempValue /= (elDef->e_rad * elDef->e_rad);
			elDef->ecent = sqrt (tempValue);
		}
		else
		{
			// Here if the reciprocal of flattening value is absurd, like
			// zero or negative.  We essentially assume a sphere.  Maybe we
			// should produce a fatal error.
			elDef->p_rad = elDef->e_rad;
			elDef->ecent = cs_Zero;
			elDef->flat = cs_Zero;
		}
		elDef->protect = 0;
		elDef->wktFlvr = static_cast<short>(flavor);

		// If there is an AUTHORITY specification, we use it to name this
		// ellipsoid.  The full WKT name will always be in the "name"
		// element, so we don't lose much by overwriting the "key_nm"
		// element.
		authority = wktPtr->ChildLocate (rcWktAuthority);
		if (authority != 0)
		{
			long32_t epsgCode;

			cp = authority->GetElementNameC ();
			if (cp == 0 || *cp == '\0') cp = "EPSG";
			if (!CS_stricmp (cp,"EPSG"))
			{
				epsgCode = authority->GetFieldLong (0);
				elDef->epsgNbr = static_cast<short>(epsgCode);
				sprintf (ellipsoidName,"%s:%d",cp,(int)epsgCode);
				CS_stncp (elDef->key_nm,ellipsoidName,sizeof (elDef->key_nm));
			}
		}
		CS_stncp (elDef->source,"Extracted from WKT string; 'Description' is full WKT name.",sizeof (elDef->source));
		if (strlen (ellipsoidName) > cs_KEYNM_MAX) status |= cs_EL2WKT_NMTRUNC;
	}
	return status;
error:
	return -cs_Errno;
}
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following converts a WKT GEOTRANS object to CS-MAP format in the form
// of a cs_Dtdef_ and cs_Eldef_ structures.  If you have the newer format
// where the Datum element in the PROJCS element has the TOWGS84 element,
// simply use CS_wktToCs and convert the whole mess in one shot.  Use this
// function only when dealing with the older GEOTRAN WKT type string.
int CS_wktToDt (struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const char *wellKnownText)
{
	int status;
	int rotCnt = 0;
	int deltaCnt = 0;
	int sclCnt = 0;
	size_t index = 0;

	const char *cp;
	const char *cpName;

	// These are all constant pointers to sub-elements of the main wktElement
	// object declared below.  Do not delete these.
	const TrcWktElement *datum;
	const TrcWktElement *method;
	const TrcWktElement *parameter;
	const TrcWktElement *srcGeogCS;
	const TrcWktElement *trgGeogCS;

	struct cs_Dtdef_ myDtDef;
	struct cs_Eldef_ myElDef;

	// The user should always provide a dtDef argument.  The elDef argument is
	// optional.  In any case, let's prevent a crash.
	if (dtDef == 0) dtDef = &myDtDef;
	if (elDef == 0) elDef = &myElDef;

	// Prepare for any kind of error; make sure the results are unusable.
	memset (dtDef,'\0',sizeof (*dtDef));
	CS_stncp (dtDef->key_nm,"<error>",sizeof (dtDef->key_nm));
	CS_stncp (dtDef->ell_knm,"<unknown>",sizeof (dtDef->ell_knm));
	dtDef->to84_via = cs_DTCTYP_NONE;
	memset (elDef,'\0',sizeof (*elDef));
	CS_stncp (elDef->key_nm,"<error>",sizeof (elDef->key_nm));

	// Convert the text string to TrcWktElement form.  This object will get
	// deleted automatically as soon as it goes out of scope.
	TrcWktElement wktElement (wellKnownText);
	wktElement.ParseChildren ();

	// We only process GEOTRAN objects.
	if (wktElement.GetElementType () != rcWktGeogTran)
	{
		CS_erpt (cs_WKT_WRNGTYP);
		goto error;
	}
	cpName = wktElement.GetElementNameC ();

	// Determine the flavor if the calling application has not specified it.
	if (flavor == wktFlvrNone)
	{
		flavor = wktElement.DetermineFlavor ();
		if (flavor == wktFlvrUnknown)
		{
			CS_stncp (csErrnam,cpName,MAXPATH);
			CS_erpt (cs_WKT_FLAVOR);
		}
	}

	// We can only deal with GEOTRAN's which convert from something to
	// WGS84.  Therefore, we get the first GEOGCS object (that's where we get
	// the actual datum name and the spheroid), and then the second
	// an verify that the datum name is "WGS84" (actually "WGS_1984").
	srcGeogCS = wktElement.ChildLocate (rcWktGeogCS,index);
	trgGeogCS = wktElement.ChildLocate (rcWktGeogCS,index);
	if (srcGeogCS == 0 || trgGeogCS == 0)
	{
		CS_stncp (csErrnam,cpName,sizeof (csErrnam));
		CS_erpt (cs_WKT_GEOGCNT);
		goto error;
	}
	cp = trgGeogCS->GetElementNameC ();

	// Verify that the target is WGS84.  "GCS_WGS_1984" is what will
	// be there in an ESRI GEOTRAN.  Currently, we don't know what would be
	// present in any other flavor (as we've never seen a GEOTRAN from any
	// other source.
	if (CS_stricmp (cp,"GCS_WGS_1984"))
	{
		CS_stncp (csErrnam,cpName,sizeof (csErrnam));
		CS_erpt (cs_WKT_WRNGTRG);
		goto error;
	}

	// OK, we should be able to do something with this.  We process the normal
	// datum and ellipsoid stuff.
	datum = srcGeogCS->ChildSearch (rcWktDatum);
	if (datum == 0)
	{
		CS_stncp (csErrnam,cpName,sizeof (csErrnam));
		CS_erpt (cs_WKT_NOSRCDT);
		goto error;
	}
	status = CS_wktEleToDt (dtDef,elDef,flavor,datum);
	if (status < 0) goto error;

	// The TOWGS84 element will not be there.  Process the parameters.  We'll
	// deal with the method after all parameter values have been extracted.
	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_DELTAX);
	dtDef->delta_X = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;
	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_DELTAY);
	dtDef->delta_Y = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;
	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_DELTAZ);
	dtDef->delta_Z = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;

	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_ROTATX);
	dtDef->rot_X = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;
	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_ROTATY);
	dtDef->rot_Y = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;
	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_ROTATZ);
	dtDef->rot_Z = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;

	parameter = wktElement.ParameterLocate (flavor,cs_WKTCOD_BWSCAL);
	dtDef->bwscale = (parameter != 0) ? parameter->GetFieldDouble (0) : cs_Zero;

	// Set the appropriate method value based on the number and type of
	// parameters present.  We'll adjust for METHOD later.
	if (dtDef->rot_X != 0.0) rotCnt += 1;
	if (dtDef->rot_Y != 0.0) rotCnt += 1;
	if (dtDef->rot_Z != 0.0) rotCnt += 1;

	if (dtDef->delta_X != 0.0) deltaCnt += 1;
	if (dtDef->delta_Y != 0.0) deltaCnt += 1;
	if (dtDef->delta_Z != 0.0) deltaCnt += 1;

	if (dtDef->bwscale != 0.0) sclCnt += 1;

	if      (rotCnt != 0  ) dtDef->to84_via = cs_DTCTYP_7PARM;
	else if (sclCnt != 0  ) dtDef->to84_via = cs_DTCTYP_4PARM;
	else if (deltaCnt != 0) dtDef->to84_via = cs_DTCTYP_MOLO;
	else                    dtDef->to84_via = cs_DTCTYP_WGS84;

	// OK, get the method and see if we should do anything to what we have
	// already determined.
	method = wktElement.ChildLocate (rcWktMethod);
	if (method == 0)
	{
		CS_stncp (csErrnam,cpName,sizeof (csErrnam));
		CS_erpt (cs_WKT_NOMETH);
		goto error;
	}
	cp = method->GetElementNameC ();
	if (!CS_stricmp (cp,"Geocentric_Translation"))
	{
		if (dtDef->to84_via != cs_DTCTYP_MOLO)
		{
			CS_stncp (csErrnam,cpName,sizeof (csErrnam));
			CS_erpt (cs_WKT_MTHERR);
			goto error;
		}		
		dtDef->rot_X   = cs_Zero;
		dtDef->rot_Y   = cs_Zero;
		dtDef->rot_Z   = cs_Zero;
		dtDef->bwscale = cs_Zero;
	}
	else if (!CS_stricmp (cp,"Position_Vector"))
	{
		if (dtDef->to84_via != cs_DTCTYP_7PARM)
		{
			CS_stncp (csErrnam,cpName,sizeof (csErrnam));
			CS_erpt (cs_WKT_MTHERR);
			goto error;
		}		
		dtDef->rot_X = -dtDef->rot_X;
		dtDef->rot_Y = -dtDef->rot_Y;
		dtDef->rot_Z = -dtDef->rot_Z;
	}
	else if (!CS_stricmp (cp,"Coordinate_Frame"))
	{
		if (dtDef->to84_via != cs_DTCTYP_7PARM)
		{
			CS_stncp (csErrnam,cpName,sizeof (csErrnam));
			CS_erpt (cs_WKT_MTHERR);
			goto error;
		}		
	}
	else
	{
		CS_stncp (csErrnam,cpName,sizeof (csErrnam));
		CS_erpt (cs_WKT_UKMETH);
		goto error;
	}
	return status;

error:
	// Delete, free, whatever stuff is necessary; none just yet.
	return -cs_Errno;
}
// Use this function to replace any of the supplied definitions with
// definitions from the CS-MAP Dictionaries if they do exist.  That is,
// for each definition parameter which is not zero, look the name up in
// the appropriate dictionary and replace the definition if an entry
// with the appropriate name in the dictionary already exists.
//
// The function returns -1 for a hard error.  A bit map of the substitutions
// made is returned in the case of success.  The bit map values are:
// 1 --> ellipsoid definition
// 2 --> datumn definition
// 4 --> coordinate definition.
//
// This implies that zero is returned if no subsitutions are made.
//
extern "C" int cs_Error;
int CS_wktDictRpl (struct cs_Csdef_ *csDef,struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef)
{

	int status;
	struct cs_Csdef_ *csPtr;
	struct cs_Dtdef_ *dtPtr;
	struct cs_Eldef_ *elPtr;
	
	status = 0;
	if (csDef != 0)
	{
		csPtr = CS_csdef (csDef->key_nm);
		if (csPtr != 0)
		{
			memcpy (csDef,csPtr,sizeof (*csDef));
			CS_free (csPtr);
			status |= 4;
		}
		else if (cs_Error != cs_CS_NOT_FND)
		{
			status = -1;
		}
	}
	if (status >= 0 && dtDef != 0)
	{
		dtPtr = CS_dtdef (dtDef->key_nm);
		if (dtPtr != 0)
		{
			memcpy (dtDef,dtPtr,sizeof (*dtDef));
			CS_free (dtPtr);
			status |= 2;
		}
		else if (cs_Error != cs_DT_NOT_FND)
		{
			status = -1;
		}
	}
	if (status >= 0 && elDef != 0)
	{
		elPtr = CS_eldef (elDef->key_nm);
		if (elPtr != 0)
		{
			memcpy (elDef,elPtr,sizeof (*elDef));
			CS_free (elPtr);
			status |= 1;
		}
		else if (cs_Error != cs_EL_NOT_FND)
		{
			status = -1;
		}
	}
	return status;
}
// The following function adjusts the provided cs_def with the appropriate
// information extracted from the WKT element.  If the parameterization is
// consistent with the CS-MAP parameterization, we leave it alone.
int CSwellKnownTextKrovak (struct cs_Csdef_* csDef,struct cs_Eldef_ *elDef,const TrcWktElement* wktElement,
																		   double primeMeridian,
																		   double geogcsUnitsFactor,
																		   ErcWktFlavor flavor)
{
	const char* wktName;
	const TrcWktElement *paramCNTMER;
	const TrcWktElement *paramPOLELNG;
	const TrcWktElement *paramPOLELAT;
	const TrcWktElement *paramGCPLNG;
	const TrcWktElement *paramGCPLAT;
	const TrcWktElement *paramGCAZM;
	const TrcWktElement *paramOSTDPLL;
	
	double e_rad;
	double e_sq;
	double paramValue;

	if (flavor == wktFlvrGeoTools)
	{
		geogcsUnitsFactor = 1.0;
	}

	// Determine what parameters are there, and which are not.
	paramCNTMER  = wktElement->ParameterLocate (flavor,cs_PRMCOD_CNTMER);
	paramPOLELNG = wktElement->ParameterLocate (flavor,cs_PRMCOD_POLELNG);
	paramPOLELAT = wktElement->ParameterLocate (flavor,cs_PRMCOD_POLELAT);
	paramGCPLNG  = wktElement->ParameterLocate (flavor,cs_PRMCOD_GCPLNG);
	paramGCPLAT  = wktElement->ParameterLocate (flavor,cs_PRMCOD_GCPLAT);
	paramGCAZM   = wktElement->ParameterLocate (flavor,cs_PRMCOD_GCAZM);
	paramOSTDPLL = wktElement->ParameterLocate (flavor,cs_PRMCOD_OSTDPLL);

	// Based on the presence of various parameters, we determine how to
	// proceed.
	if (paramPOLELNG == 0 && paramPOLELAT == 0 &&
		paramOSTDPLL != 0 && paramGCPLNG != 0 && paramGCPLAT != 0 && paramGCAZM != 0)
	{
		// If the origin longitude is not present, we set it equal to the prime
		// meridian.  This is due to a legacy issue with the CS-MAP implementation
		// of Krovak.  The original documentation, in Czech, was misinterpreted by
		// the developer who did not understand Czech.
		if (paramCNTMER == 0)
		{
			csDef->org_lng = primeMeridian;
		}

		// This is the EPSG parameterization.  The complication here is
		// that we need to compute the latitude of the oblique pole.
		// As you can see, this is not trivial.
		csDef->org_lat = paramGCPLAT->GetFieldDouble (0)  * geogcsUnitsFactor;
		csDef->prj_prm1 = paramGCPLNG->GetFieldDouble (0)  * geogcsUnitsFactor;	// oblique pole longitude

		e_rad = elDef->e_rad * csDef->scl_red;
		e_sq = elDef->ecent * elDef->ecent;
		paramValue = paramGCAZM->GetFieldDouble (0)  * geogcsUnitsFactor;
		paramValue = CSkrovkCsMapParam (e_rad,e_sq,csDef->org_lng,csDef->org_lat,paramValue);
		csDef->prj_prm2 = paramValue;												// oblique pole latitude
		
		csDef->prj_prm3 = paramOSTDPLL->GetFieldDouble (0)  * geogcsUnitsFactor;	// oblique cone standard parallel
	}
	else if (paramPOLELNG == 0 || paramPOLELAT == 0 || paramOSTDPLL == 0 || paramCNTMER == 0)
	{
		wktName = wktElement->GetElementNameC ();
		CS_stncp (csErrnam,wktName,MAXPATH);
		CS_erpt (cs_WKT_PRMSUPRT);
		return -1;
	}
	// Otherwise, we have legitimate CS-MAP parameterization, we have nothing to do.

	return 0;
}
