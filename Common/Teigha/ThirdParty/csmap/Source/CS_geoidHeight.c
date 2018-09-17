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

	This object is designed to support access to, and use of, various
	geoid height calculation techniques.  Upon initial writing, only
	one technique is supported; the US GEOID 96 technique and data files.
	This, of course, limits coverage to the 48 conterminous states of
	the US.  However, it is expected that additional techniques and
	data source will be supported by this object in the future.

	Due to similarity with the datum problem, much of the code generated
	for datum calculations is used to develop this object.
*/

#include "cs_map.h"

struct csGeoidHeight_ *csGeoidHeight = NULL;

/******************************************************************************
	High Level Interface access functions.
*/
int EXP_LVL1 CS_geoidHgt (Const double ll84 [2],double *height)
{
	extern double cs_Mhuge;				/* -1.0E+32  */
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern struct csGeoidHeight_ *csGeoidHeight;

	int status = -1;


	*height = cs_Mhuge;
	if (csGeoidHeight == NULL)
	{
		CS_stcpy (cs_DirP,cs_GEOID_NAME);
		csGeoidHeight = CSnewGeoidHeight (cs_Dir);
	}
	if (csGeoidHeight != NULL)
	{
		status = CScalcGeoidHeight (csGeoidHeight,height,ll84);
	}
	return status;
}
void EXP_LVL1 CS_geoidCls (void)
{
	extern struct csGeoidHeight_ *csGeoidHeight;

	if (csGeoidHeight != NULL)
	{
		CSdeleteGeoidHeight (csGeoidHeight);
		csGeoidHeight = NULL;
	}
	return;
}
/******************************************************************************
        Constructor
*/
struct csGeoidHeight_* CSnewGeoidHeight (Const char *catalog)
{
	int index;
	struct csGeoidHeight_ *__This;
	struct csDatumCatalog_ *catPtr;
	struct csDatumCatalogEntry_ *catEntryPtr;
	struct csGeoidHeightEntry_* ghEntryPtr;
	struct csGeoidHeightEntry_* findPtr;

	/* Prepare for an error. */
	__This = NULL;
	catPtr = NULL;
	catEntryPtr = NULL;
	ghEntryPtr = NULL;

	__This = (struct csGeoidHeight_*) CS_malc (sizeof (struct csGeoidHeight_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	__This->listHead = NULL;

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
		ghEntryPtr = CSnewGeoidHeightEntry (catEntryPtr);
		if (ghEntryPtr == NULL)
		{
			goto error;
		}
		/* Keep the list in the same order as they appear in the file. */
		if (__This->listHead == NULL)
		{
			__This->listHead = ghEntryPtr;
		}
		else
		{
			/* Find the last element in the list. */
			for (findPtr = __This->listHead;findPtr->next != NULL;findPtr = findPtr->next);		/*lint !e722  suspicious ';' */
			findPtr->next = ghEntryPtr;
		}
	}
	CSdeleteDatumCatalog (catPtr);
	catPtr = NULL;
	return __This;
error:
	if (catPtr != NULL) CSdeleteDatumCatalog (catPtr);
	if (__This != NULL) CS_free (__This);
	return NULL;
}

/******************************************************************************
	Destructor
*/
void CSdeleteGeoidHeight (struct csGeoidHeight_* __This)
{
	struct csGeoidHeightEntry_ *ghEntryPtr;

	if (__This != NULL)
	{
		while (__This->listHead != NULL)
		{
			ghEntryPtr = __This->listHead;
			__This->listHead = __This->listHead->next;
			CSdeleteGeoidHeightEntry (ghEntryPtr);
		}
		CS_free (__This);
	}
	return;
}

/******************************************************************************
	Coverage Locator
*/
struct csGeoidHeightEntry_* CSselectGeoidHeight (struct csGeoidHeight_* __This,Const double *ll84)
{
	double testValue;
	double bestSoFar;
	struct csGeoidHeightEntry_* ghEntryPtr;
	struct csGeoidHeightEntry_* rtnValue;

	rtnValue = NULL;
	bestSoFar = 3600000.00;
	ghEntryPtr = __This->listHead;
	while (ghEntryPtr != NULL)
	{
		testValue = CStestGeoidHeightEntry (ghEntryPtr,ll84);
		if (testValue != 0.0 && testValue < bestSoFar)
		{
			bestSoFar = testValue;
			rtnValue = ghEntryPtr;
		}
		ghEntryPtr = ghEntryPtr->next;
	}
	return rtnValue;
}

/******************************************************************************
	Make First -- Used for performance.  Well used to use it for performance.
		This has been replaced with the GridCellCache business.  We now want
		to keep the order of these things the same as they appear in the
		catalog file.
*/
void CSfirstGeoidHeight (struct csGeoidHeight_* __This,struct csGeoidHeightEntry_* ghEntryPtr)
{
	struct csGeoidHeightEntry_* curPtr;
	struct csGeoidHeightEntry_* prvPtr;

	/* Take care of the already first situation very quickly. */
	if (ghEntryPtr == __This->listHead) return;

	/* Locate this guy on the list. */
	for (curPtr = __This->listHead,prvPtr = NULL;
		 curPtr != NULL;
		 prvPtr = curPtr,curPtr = curPtr->next)
	{
		if (curPtr == ghEntryPtr)
		{
			prvPtr->next = curPtr->next;						/*lint !e613 */
			curPtr->next = __This->listHead;
			__This->listHead = curPtr;
			break;
		}
	}
}

/******************************************************************************
	Calculate the Geoid height, the main man.
*/
int CScalcGeoidHeight (struct csGeoidHeight_* __This,double* geoidHgt,Const double *ll84)
{
	int status;
	struct csGeoidHeightEntry_* ghEntryPtr;

	if (__This == NULL)
	{
		CS_erpt (cs_GEOID_INIT);
		status = -1;
	}
	else
	{
		status = 0;
		ghEntryPtr = CSselectGeoidHeight (__This,ll84);
		if (ghEntryPtr != NULL)
		{
			status = CScalcGeoidHeightEntry (ghEntryPtr,geoidHgt,ll84);
		}
		else
		{
			/* We didn't find any coverage.  Use the fall back position.
			   Return a +1 to indicate an approximation. */
			status = 1;
		}
	}
	return status;
}

/******************************************************************************
	Release -- Release resources, but maintain the directory status.
*/
void CSreleaseGeoidHeight (struct csGeoidHeight_* __This)
{
	struct csGeoidHeightEntry_* ghEntryPtr;

	if (__This != NULL)
	{
		for (ghEntryPtr = __This->listHead;ghEntryPtr != NULL;ghEntryPtr = ghEntryPtr->next)
		{
			CSreleaseGeoidHeightEntry (ghEntryPtr);
		}
	}
	return;
}

/******************************************************************************
	Constructor: for the "Entry" sub-object.  A linked list of these "Entry"
	objects is the basic structure of the main object.
*/
struct csGeoidHeightEntry_* CSnewGeoidHeightEntry (struct csDatumCatalogEntry_* catPtr)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	char *cp;
	struct csGeoidHeightEntry_* __This;

	/* Prepare for an error. */
	__This = NULL;

	/* Allocate some storage. */
	__This = (struct csGeoidHeightEntry_*) CS_malc (sizeof (struct csGeoidHeightEntry_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	__This->next = NULL;
	__This->type = csGeoidHgtTypeNone;
	__This->pointers.geoid96Ptr = NULL;

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

	/* Do what's appropriate for this extension. */
	if (!CS_stricmp (cp,"GEO"))
	{
		/* Must not set the type until allocated for correct error handling. */
		__This->pointers.geoid96Ptr = CSnewGeoid96GridFile (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (__This->pointers.geoid96Ptr == NULL)
		{
			goto error;
		}
		__This->type = csGeoidHgtTypeGeoid96;
	}
	else if (!CS_stricmp (cp,"bin"))
	{
		/* These are supposed to be Geoid99 type files. */
		/* Must not set the type until allocated for correct error handling. */
		__This->pointers.geoid99Ptr = CSnewGeoid99GridFile (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (__This->pointers.geoid99Ptr == NULL)
		{
			goto error;
		}
		__This->type = csGeoidHgtTypeGeoid99;
	}
	else if (!CS_stricmp (cp,"txt"))
	{
		/* These are supposed to be OSGM91 type files. */
		/* Must not set the type until allocated for correct error handling. */
		__This->pointers.osgm91Ptr = CSnewOsgm91 (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (__This->pointers.osgm91Ptr == NULL)
		{
			goto error;
		}
		__This->type = csGeoidHgtTypeOsgm91;
	}
	else if (!CS_stricmp (cp,"byn"))
	{
		/* Must not set the type until allocated for correct error handling. */
		__This->pointers.bynGridFilePtr = CSnewBynGridFile (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (__This->pointers.bynGridFilePtr == NULL)
		{
			goto error;
		}
		__This->type = csGeoidHgtTypeBynGridFile;
	}
	else if (!CS_stricmp (cp,"grd"))
	{
		/* Must not set the type until allocated for correct error handling. */
		__This->pointers.egm96Ptr = CSnewEgm96 (catPtr->pathName,catPtr->bufferSize,catPtr->flags,catPtr->density);
		if (__This->pointers.egm96Ptr == NULL)
		{
			 goto error;
		}
		__This->type = csGeoidHgtTypeEgm96;
	}
	else
	{
		CS_erpt (cs_GHGT_EXT);
		goto error;
	}
	return __This;
error:
	CSdeleteGeoidHeightEntry (__This);
	return NULL;
}

/******************************************************************************
	Destructor: for the "Entry" sub-object.
*/
void CSdeleteGeoidHeightEntry (struct csGeoidHeightEntry_* __This)
{
	if (__This != NULL)
	{
		switch (__This->type){
		case csGeoidHgtTypeGeoid96:
			CSdeleteGeoid96GridFile (__This->pointers.geoid96Ptr);
			break;
		case csGeoidHgtTypeGeoid99:
			CSdeleteGeoid99GridFile (__This->pointers.geoid99Ptr);
			break;
		case csGeoidHgtTypeOsgm91:
			CSdeleteOsgm91 (__This->pointers.osgm91Ptr);
			break;
		case csGeoidHgtTypeBynGridFile:
			CSdeleteBynGridFile (__This->pointers.bynGridFilePtr);
			break;
		case csGeoidHgtTypeEgm96:
			CSdeleteEgm96 (__This->pointers.egm96Ptr);
			break;
		case csGeoidHgtTypeWorld:
		case csGeoidHgtTypeAustralia:
		case csGeoidHgtTypeNone:
		default:
			break;
		}
		CS_free (__This);
	}
	return;
}

/******************************************************************************
	Release resources: for the "Entry" sub-object.
*/
void CSreleaseGeoidHeightEntry (struct csGeoidHeightEntry_* __This)
{
	if (__This != NULL)
	{
		switch (__This->type){
		case csGeoidHgtTypeGeoid96:
			CSreleaseGeoid96GridFile (__This->pointers.geoid96Ptr);
			break;
		case csGeoidHgtTypeGeoid99:
			CSreleaseGeoid99GridFile (__This->pointers.geoid99Ptr);
			break;
		case csGeoidHgtTypeOsgm91:
			CSreleaseOsgm91 (__This->pointers.osgm91Ptr);
			break;
		case csGeoidHgtTypeBynGridFile:
			CSreleaseBynGridFile (__This->pointers.bynGridFilePtr);
			break;
		case csGeoidHgtTypeEgm96:
			CSreleaseEgm96 (__This->pointers.egm96Ptr);
			break;
		case csGeoidHgtTypeWorld:
		case csGeoidHgtTypeAustralia:
		case csGeoidHgtTypeNone:
		default:
			break;
		}
	}
	return;
}

/******************************************************************************
	coverage Test: for the "Entry" sub-object.
*/
double CStestGeoidHeightEntry (struct csGeoidHeightEntry_* __This,Const double* ll84)
{
	double rtnValue;

	rtnValue = 0.0;
	if (__This != NULL)
	{
		switch (__This->type){
		case csGeoidHgtTypeGeoid96:
			rtnValue = CStestGeoid96GridFile (__This->pointers.geoid96Ptr,ll84);
			break;
		case csGeoidHgtTypeGeoid99:
			rtnValue = CStestGeoid99GridFile (__This->pointers.geoid99Ptr,ll84);
			break;
		case csGeoidHgtTypeOsgm91:
			rtnValue = CStestOsgm91 (__This->pointers.osgm91Ptr,ll84);
			break;
		case csGeoidHgtTypeBynGridFile:
			rtnValue = CStestBynGridFile (__This->pointers.bynGridFilePtr,ll84);
			break;
		case csGeoidHgtTypeEgm96:
			rtnValue = CStestEgm96 (__This->pointers.egm96Ptr,ll84);
			break;
		case csGeoidHgtTypeWorld:
		case csGeoidHgtTypeAustralia:
		case csGeoidHgtTypeNone:
		default:
			break;
		}
	}
	return rtnValue;
}

/******************************************************************************
	Calculate the geoid height, given the specific "Entry" item which is to
	be used.
*/
int CScalcGeoidHeightEntry (struct csGeoidHeightEntry_* __This,double* geoidHgt,Const double *ll84)
{
	extern char csErrnam [];

	int status;

	status = -1;
	if (__This != NULL)
	{
		switch (__This->type) {
		case csGeoidHgtTypeGeoid96:
			status = CScalcGeoid96GridFile (__This->pointers.geoid96Ptr,geoidHgt,ll84);
			break;
		case csGeoidHgtTypeGeoid99:
			status = CScalcGeoid99GridFile (__This->pointers.geoid99Ptr,geoidHgt,ll84);
			break;
		case csGeoidHgtTypeOsgm91:
			status = CScalcOsgm91 (__This->pointers.osgm91Ptr,geoidHgt,ll84);
			break;
		case csGeoidHgtTypeBynGridFile:
			status = CScalcBynGridFile (__This->pointers.bynGridFilePtr,geoidHgt,ll84);
			break;
		case csGeoidHgtTypeEgm96:
			status = CScalcEgm96 (__This->pointers.egm96Ptr,geoidHgt,ll84);
			break;
		case csGeoidHgtTypeWorld:
		case csGeoidHgtTypeAustralia:
		case csGeoidHgtTypeNone:
		default:
			CS_stncp (csErrnam,"CS_usGridFile:3",MAXPATH);
			CS_erpt (cs_ISER);
			break;
		}
	}
	return status;
}
