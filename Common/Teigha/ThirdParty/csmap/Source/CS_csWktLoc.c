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
#include "cs_wkt.h"

/* The following static variables are used to carry the translation of a WKT
   string to CS-MAP format.
   
   On the initial call to the hook function for CS_csdef (), the WKT is parsed
   by CS_wktToCsEx and the results of that parse are saved in these static
   variables.
   
   On a successful parse, CS_wktCsDefFunc returns the definition saved in
   wktCsDef.  On the subsequent CS_wktDtDefFunc and CS_wktDtDefFunc calls,
   the respective functions compare the requested key name with that stored in
   the static definition structures below.  If the names match, the respective
   definition is returned.  If the name does not match, the respective
   functions return a zero which instructs the respective CS_??def function
   to do its own thing.
   
   Note, that CS_wktToCsEx pretty much verifies that all is OK before returning
   a non-negative status value.  In the case of a negative status value, an
   approriate error indication will have already been reported to CS_erpt, and
   the CS_csdef function will cause that indication to be reported to the end
   user in the same way that an invalid key name would. */

static struct cs_Csdef_ wktCsDef;
static struct cs_Dtdef_ wktDtDef;
static struct cs_Eldef_ wktElDef;

int CS_wktCsDefFunc (struct cs_Csdef_* csDefPtr,Const char* wktString)
{
	extern char csErrnam [];

	int st;
	enum ErcWktFlavor flavor;

	/* Verify that what we have been given is indeed a WKT string.  If it
	   isn't, we can promptly return to the CS_csdef function and let it
	   do it's own thing with whatever it is that it was given. */
	st = CS_isWkt (wktString);
	if (st < 0)
	{
		/* It looked like a WKT string, but it was badly formed.  That is,
		   the left/right bracket count was not zero, and the left count
		   did not agree with the right count. */
		CS_stncp (csErrnam,wktString,48);
		CS_erpt (cs_WKT_BADFORM);
		return -1;
	}
	if (st == 0)
	{
		/* Not a single reason to believe this is a WKT string.  Tell CS_csdef
		   to do its own thing. */
		return 1;
	}

	/* We must now zero out the three static structures
	   defined above.  A call to this function initiates a cycle that will
	   involve two additional functions.  We need to make sure that the
	   results of one cycle do not get used by another. */
	memset (&wktCsDef,0,sizeof (wktCsDef));
	memset (&wktDtDef,0,sizeof (wktDtDef));
	memset (&wktElDef,0,sizeof (wktElDef));

	/* If we're still here, we have what appears to be a WKT string.
	   We will be calling CS_wktToCsEx.  Providing it with a flavor
	   of wktFlvrNone instaructs that module to determine the flavor
	   and report an error if it can't. */
	flavor = wktFlvrNone;

	/* Parse the well known text and produce a set of three structures.
	   If any error occur during the parse, we report to CS_erpt and
	   return a -1.  The error report will evenutally get back to the
	   user the same as if an invalid key name was given. */	
	st = CS_wktToCsEx (&wktCsDef,&wktDtDef,&wktElDef,flavor,wktString, 0);
	if (st < 0)
	{
		/* An error occurred parsing the WKT.  CS_wktToCsEx will have
		   reported the error to CS_erpt.  We need only tell CS_csdef
		   that an error occurred.  We do that be returning -1. */
		return -1;
	}

	/* OK, we have successfully parsed the WKT into the three components.
	   Return the coordinate system component now. */
	memcpy (csDefPtr,&wktCsDef,sizeof (struct cs_Csdef_));

	/* Tell CS_csdef () that we have returneded a valid cs_CSdef_ structure. */
	return 0;
}
int CS_wktDtDefFunc (struct cs_Dtdef_* dtDefPtr,Const char* dtKeyName)
{
	int st = 1;
	/* This is rather simple.  If the provided key name is the same as that
	   in the static cs_Dtdef_ structure defined above, we return a copy of
	   that structure.  Otherwise, we simply tell CS_dtdef () to do its own
	   thing. */
	if (!CS_stricmp (wktDtDef.key_nm,dtKeyName))
	{
		memcpy (dtDefPtr,&wktDtDef,sizeof (struct cs_Dtdef_));
		st = 0;
	}
	return st;
}
int CS_wktElDefFunc (struct cs_Eldef_* elDefPtr,Const char* elKeyName)
{
	int st = 1;
	/* This is rather simple.  If the provided key name is the same as that
	   in the static cs_Eldef_ structure defined above, we return a copy of
	   that structure.  Otherwise, we simply tell CS_eldef () to do its own
	   thing. */
	if (!CS_stricmp (wktElDef.key_nm,elKeyName))
	{
		memcpy (elDefPtr,&wktElDef,sizeof (struct cs_Eldef_));
		st = 0;
	}
	return st;
}
/* Given a coordinate system key name, return a WKT string which represents
   the equivalent in WKT format.  This one is easy, so we deal with it
   first. */
int EXP_LVL1 CS_msiCs2Wkt (char *wktBufr,size_t bufrSize,Const char* msiCsName,enum ErcWktFlavor flavor)
{
	int rtnValue = -1;
	struct cs_Csdef_ *csDefPtr = NULL;

	if (bufrSize > 0)
	{
		*wktBufr = '\0';	
		csDefPtr = CS_csdef (msiCsName);
		if (csDefPtr != NULL)
		{
			rtnValue = CScs2Wkt (wktBufr,bufrSize,flavor,csDefPtr,NULL,NULL);
			CS_free (csDefPtr);
		}
	}
	return rtnValue;
}

/*	Given a WKT string, we create a coordinate system definition, a datum
	definition, and an ellipsoid definition as appropriate.  We use the names
	the user provides us.  If the overwrite flag is set, we overwrite existing
	definitions silently.  If not set, we return an appropriate error condition.
int EXP_LVL1 CS_wkt2MsiCs (const char *csKeyName,const char *dtKeyName,const char *elKeyName,const char* wktBufr,int flavor)
{
	struct cs_Csdef_ csDef;
	struct cs_Csdef_ dtDef;
	struct cs_Csdef_ elDef;

	const TrcWktElement *wktPtr = new TrcWktElement
	int CS_wktEleToDt (struct cs_Dtdef_ *dtDef,struct cs_Eldef_ *elDef,ErcWktFlavor flavor,const TrcWktElement* wktPtr)
}
*/
