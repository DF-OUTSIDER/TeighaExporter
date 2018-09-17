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

	This object maintains a list of cs_NadconFile_ objects which contain the
	difference in orthometric height in the form of NADV88 - NADV29, in meters.
	Thus, the conversion of the Z coordinate from one to the other consists of
	a single object.
*/

#include "cs_map.h"

/******************************************************************************
        Constructor
*/
struct csVertconUS_* CSnewVertconUS (Const char *catalog)
{
	int index;
	struct csVertconUS_ *thisPtr;
	struct csDatumCatalog_ *catPtr;
	struct csDatumCatalogEntry_ *catEntryPtr;
	struct csVertconUSEntry_* vcEntryPtr;
	struct csVertconUSEntry_* findPtr;

	/* Prepare for an error. */
	thisPtr = NULL;
	catPtr = NULL;
	catEntryPtr = NULL;
	vcEntryPtr = NULL;

	thisPtr = (struct csVertconUS_*) CS_malc (sizeof (struct csVertconUS_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	thisPtr->listHead = NULL;

	/* Open the catalog file. */
	catPtr = CSnewDatumCatalog (catalog);
	if (catPtr == NULL) goto error;

	/* For each entry in the catalog, we build an appropriate geoid height
	   grid file entry.  Right now, this is based on file names and file
	   extensions.  Not very good, but that's life. */

	index = 0;
	for (;;)
	{
		catEntryPtr = CSgetDatumCatalogEntry (catPtr,index++);
		if (catEntryPtr == NULL) break;
		vcEntryPtr = CSnewVertconUSEntry (catEntryPtr);
		if (vcEntryPtr == NULL)
		{
			goto error;
		}
		/* Keep the list in the same order as they appear in the file. */
		if (thisPtr->listHead == NULL)
		{
			thisPtr->listHead = vcEntryPtr;
		}
		else
		{
			for (findPtr = thisPtr->listHead;findPtr->next != NULL;findPtr = findPtr->next);	/*lint !e722  suspicious use of ';' */
			findPtr->next = vcEntryPtr;
		}
	}
	CSdeleteDatumCatalog (catPtr);
	catPtr = NULL;

	/* OK, we ;re done. */
	return thisPtr;
error:
	if (catPtr != NULL) CSdeleteDatumCatalog (catPtr);
	if (thisPtr != NULL) CS_free (thisPtr);
	return NULL;
}

/******************************************************************************
	Destructor
*/
void CSdeleteVertconUS (struct csVertconUS_* thisPtr)
{
	struct csVertconUSEntry_ *vcEntryPtr;

	if (thisPtr != NULL)
	{
		while (thisPtr->listHead != NULL)
		{
			vcEntryPtr = thisPtr->listHead;
			thisPtr->listHead = thisPtr->listHead->next;
			CSdeleteVertconUSEntry (vcEntryPtr);
		}
		CS_free (thisPtr);
	}
	return;
}

/******************************************************************************
	Coverage Locator -- All files have the same density (so far) so a little of
	this is unecessary.
*/
struct csVertconUSEntry_* CSselectVertconUS (struct csVertconUS_* thisPtr,Const double *ll83)
{
	double testValue;
	double bestSoFar;
	struct csVertconUSEntry_* vcEntryPtr;
	struct csVertconUSEntry_* rtnValue;

	rtnValue = NULL;
	bestSoFar = 3600000.00;
	vcEntryPtr = thisPtr->listHead;
	while (vcEntryPtr != NULL)
	{
		testValue = CStestVertconUSEntry (vcEntryPtr,ll83);
		if (testValue != 0.0 && testValue < bestSoFar)
		{
			bestSoFar = testValue;
			rtnValue = vcEntryPtr;
		}
		vcEntryPtr = vcEntryPtr->next;
	}
	return rtnValue;
}

/******************************************************************************
	Make First -- Used for performance.  Well used to use it for performance.
	    This has been replaced with the GridCellCache business.  We now want
		to keep the order of these things the same as they appear in the
		catalog file.
*/
void CSfirstVertconUS (struct csVertconUS_* thisPtr,struct csVertconUSEntry_* vcEntryPtr)
{
	struct csVertconUSEntry_* curPtr;
	struct csVertconUSEntry_* prvPtr;

	/* Take care of the already first situation very quickly. */
	if (vcEntryPtr == thisPtr->listHead) return;

	/* Locate this guy on the list. */
	for (curPtr = thisPtr->listHead,prvPtr = NULL;
		 curPtr != NULL;
		 prvPtr = curPtr,curPtr = curPtr->next)
	{
		if (curPtr == vcEntryPtr)
		{
			/* prvPtr will not be null if vcEntryPtr is indeed in the
			   list.  We do not get here is vcEntryPtr is the first
			   entry in the list.  Thus, we can never get here on
			   the first iteration of the loop. */
			prvPtr->next = curPtr->next;		/*lint !e613  possible use of null pointer */
			curPtr->next = thisPtr->listHead;
			thisPtr->listHead = curPtr;
			break;
		}
	}
}

/******************************************************************************
	Calculate the elevation shift, the main man.  Note, result of this
	calculation is in millimeters.
*/
int CScalcVertconUS (struct csVertconUS_* thisPtr,double* v88Mv29,Const double *ll83)
{
	int status;
	struct csVertconUSEntry_* vcEntryPtr;

	vcEntryPtr = CSselectVertconUS (thisPtr,ll83);
	if (vcEntryPtr != NULL)
	{
		status = CScalcVertconUSEntry (vcEntryPtr,v88Mv29,ll83);
	}
	else
	{
		/* We didn't find any coverage.  Use the fall back position.
		   Return a +1 to indicate an approximation. */
		status = 1;
	}
	return status;
}

/******************************************************************************
	Release -- Release resources, but maintain the directory status.
*/
void CSreleaseVertconUS (struct csVertconUS_* thisPtr)
{
	struct csVertconUSEntry_* vcEntryPtr;

	if (thisPtr != NULL)
	{
		for (vcEntryPtr = thisPtr->listHead;vcEntryPtr != NULL;vcEntryPtr = vcEntryPtr->next)
		{
			CSreleaseVertconUSEntry (vcEntryPtr);
		}
	}
	return;
}

/******************************************************************************
	Constructor  (for an 'Entry' sub object)
*/
/*lint -esym(429,thisPtr)    pointer is not freed */
/* Verified that it is indeed free'ed in the event of an error.  I suspect the
   rather elaborate macro in CS_free has confused PC-Lint. */
struct csVertconUSEntry_* CSnewVertconUSEntry (struct csDatumCatalogEntry_* catPtr)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	char *cp;
	struct csVertconUSEntry_* thisPtr;

	/* Prepare for an error. */
	thisPtr = NULL;

	/* Allocate some storage. */
	thisPtr = (struct csVertconUSEntry_*) CS_malc (sizeof (struct csVertconUSEntry_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	thisPtr->next = NULL;
	thisPtr->usGridPtr = NULL;

	/* Isolate the file name from the path, and the extension from the file name. */
	cp = strrchr (catPtr->pathName,cs_DirsepC);
	if (cp == NULL)
	{
		/* Is not supposed to happen, but we can't allow a bomb. */
		CS_stncp (csErrnam,catPtr->pathName,MAXPATH);
		CS_erpt (cs_DTC_PATH);
		goto error;
	}

	/* Isolate the extension on the file. */
	cp = strrchr (catPtr->pathName,cs_ExtsepC);
	if (cp == NULL)
	{
		/* Is not supposed to happen. */
		CS_stncp (csErrnam,catPtr->pathName,MAXPATH);
		CS_erpt (cs_NAD_EXT);
		goto error;
	}
	cp += 1;

	/* Report special errors concerning the extensions.  Again, specific
	   error messages will reduce frustration. */
	if (CS_stricmp (cp,"94"))
	{
		CS_erpt (cs_VCON_94);
		goto error;
	}
	/* Do what's appropriate for this extension. */
	if (!CS_stricmp (cp,"94"))
	{
		thisPtr->usGridPtr = CSnewNadconFile (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (thisPtr->usGridPtr == NULL)
		{
			goto error;
		}
	}
	/* When we know how to do the others, we add that stuff here. */
	else
	{
		CS_erpt (cs_VCON_EXT);
		goto error;
	}
	return thisPtr;
error:
	if (thisPtr != NULL) CSdeleteVertconUSEntry (thisPtr);
	return NULL;
}
/*lint +esym(429,thisPtr) */

/******************************************************************************
	Destructor  (for an 'Entry' sub object)
*/
void CSdeleteVertconUSEntry (struct csVertconUSEntry_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->usGridPtr != NULL)
		{
			CSdeleteNadconFile (thisPtr->usGridPtr);
		}
		CS_free (thisPtr);
	}
	return;
}							/*lint !e429  custodial pointer not frre'd -- PC-Lint bug */

/******************************************************************************
	Release resources  (for an 'Entry' sub object)
*/
void CSreleaseVertconUSEntry (struct csVertconUSEntry_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->usGridPtr != NULL)
		{
			CSreleaseNadconFile (thisPtr->usGridPtr);
		}
	}
	return;
}

/******************************************************************************
	Locate appropriate "entry' sub object  (for an 'Entry' sub object)
*/
double CStestVertconUSEntry (struct csVertconUSEntry_* thisPtr,Const double* ll83)
{
	double rtnValue;

	rtnValue = 0.0;
	if (thisPtr != NULL)
	{
		if (thisPtr->usGridPtr != NULL)
		{
			rtnValue = CStestNadconFile (thisPtr->usGridPtr,ll83);
		}
	}
	return rtnValue;
}

/******************************************************************************
	Calculate vertical datum shift, given a specific 'Entry' sub object.
	Note, result of this calculation is in millimeters.
*/
int CScalcVertconUSEntry (struct csVertconUSEntry_* thisPtr,double* v88Mv29,Const double *ll83)
{
	extern double cs_Zero;

	int status;

	*v88Mv29 = cs_Zero;
	status = -1;
	if (thisPtr != NULL && thisPtr->usGridPtr != NULL)
	{
		status = CScalcNadconFile (thisPtr->usGridPtr,v88Mv29,ll83);
	}
	return status;
}
