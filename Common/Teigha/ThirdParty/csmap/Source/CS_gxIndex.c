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
#include "cs_ioUtil.h"

/* cs_GxIndex --> Geodetic Transformation Index

	This is a simple malloc'ed array of cs_GxIndex_ structures, a pointer to
	which is maintained in a global variable named cs_GxIndex.  The number
	of entries in the index is maintained in a global variable named
	cs_GxIndexCount.  This index is usually generated dynamically on the
	first need for such and typically remains in memory until the host
	process exits.  Estimated memory usage is approximately 100KB.
	
	This was done as the automatic generation of geodetic paths, specifically
	Phase Two and Phase Three, require several searches of the information
	contained in the index, and enhanced techniques like binary searching
	are not terribly effective in this particular context.
	
	The index is designed so that applications should never access the
	global variables directly; all access is through a small set of simple
	functions.  Thus, the application should be fairly well insulated from
	changes in the index. */

/* Get a pointer to the index; generates the index if required. */	
struct cs_GxIndex_* CS_getGxIndexPtr ()
{
	extern struct cs_GxIndex_* cs_GxIndex;

	if (cs_GxIndex == NULL)
	{
		CSgenerateGxIndex ();
	}
	return cs_GxIndex;
}

/* Releases the memory occupied by the index.  Should not be needed by
   applications, but a call to this function shopuld be added to
   CS_recvr() */	
void CS_releaseGxIndex (void)
{
	extern struct cs_GxIndex_* cs_GxIndex;
	extern unsigned cs_GxIndexCount;

	if (cs_GxIndex != NULL)
	{
		CS_free (cs_GxIndex);
		cs_GxIndex = NULL;
	}
	cs_GxIndexCount = 0;
}

/* Returns the number of entries in the Geodetic Transformation Index.
   Causes generation of the index if it does not exist. */
unsigned CS_getGxIndexCount ()
{
	extern struct cs_GxIndex_* cs_GxIndex;
	extern unsigned cs_GxIndexCount;

	if (cs_GxIndex == NULL)
	{
		CSgenerateGxIndex ();
	}
	return cs_GxIndexCount;
}

/* Returns a pointer to a specific entry in the Geodetic Transformation index. */
Const struct cs_GxIndex_* CS_getGxIndexEntry (unsigned index)
{
	struct cs_GxIndex_* gxIndex;
	struct cs_GxIndex_* entry;

	entry = NULL;
	if (index <	CS_getGxIndexCount ())
	{
		gxIndex = CS_getGxIndexPtr ();
		if (gxIndex != NULL)
		{
			entry = (gxIndex + index);
		}
	}
	return entry;
}

int CS_locateGxByName (Const char* xfrmName)
{
	int result;
	unsigned index;

	unsigned indexCount;

	struct cs_GxIndex_* entry;
	struct cs_GxIndex_* gxIndex;

	result = cs_GXIDX_ERROR;
	gxIndex = CS_getGxIndexPtr ();
	if (gxIndex != NULL)
	{
		indexCount = CS_getGxIndexCount ();
		if (indexCount > 0)
		{
			result = cs_GXIDX_NOXFRM;
			for (index = 0;index < indexCount;index += 1)
			{
				entry = (gxIndex + index);
				if (!CS_stricmp (entry->xfrmName,xfrmName))
				{
					result = (int)index;
					break;
				}
			}
		}
	}
	return result;
}

int CS_locateGxFromDatum (int* direction,Const char* srcDtmName)
{
	int result;
	int myDirection;

	unsigned index;
	unsigned indexCount;

	struct cs_GxIndex_* entry;
	struct cs_GxIndex_* gxIndex;

	result = cs_GXIDX_ERROR;
	myDirection = cs_DTCDIR_NONE;
	gxIndex = CS_getGxIndexPtr ();
	if (gxIndex != NULL)
	{
		indexCount = CS_getGxIndexCount ();
		if (indexCount > 0)
		{
			result = cs_GXIDX_NOXFRM;
			for (index = 0;index < indexCount;index += 1)
			{
				entry = (gxIndex + index);
				if (CS_stricmp (entry->srcDatum,srcDtmName) == 0)
				{
					if (result == cs_GXIDX_NOXFRM)
					{
						result = index;
						myDirection = cs_DTCDIR_FWD;
					}
					else
					{
						result = cs_GXIDX_DUPXFRM;
						myDirection = cs_DTCDIR_NONE;
					}
				}
				if (entry->inverseSupported &&
					CS_stricmp (entry->trgDatum,srcDtmName) == 0)
				{
					if (result == cs_GXIDX_NOXFRM)
					{
						result = index;
						myDirection = cs_DTCDIR_INV;
					}
					else
					{
						result = cs_GXIDX_DUPXFRM;
						myDirection = cs_DTCDIR_NONE;
					}
				}
			}
		}
	}
	if (result >= 0)
	{
		*direction = myDirection;
	}
	return result;
}
int CS_locateGxToDatum (int* direction,Const char* trgDtmName)
{
	int result;
	int myDirection;

	unsigned index;
	unsigned indexCount;

	struct cs_GxIndex_* entry;
	struct cs_GxIndex_* gxIndex;

	result = cs_GXIDX_ERROR;
	myDirection = cs_DTCDIR_NONE;
	gxIndex = CS_getGxIndexPtr ();
	if (gxIndex != NULL)
	{
		indexCount = CS_getGxIndexCount ();
		if (indexCount > 0)
		{
			result = cs_GXIDX_NOXFRM;
			for (index = 0;index < indexCount;index += 1)
			{
				entry = (gxIndex + index);
				if (CS_stricmp (entry->trgDatum,trgDtmName) == 0)
				{
					if (result == cs_GXIDX_NOXFRM)
					{
						result = index;
						myDirection = cs_DTCDIR_FWD;
					}
					else
					{
						result = cs_GXIDX_DUPXFRM;
						myDirection = cs_DTCDIR_NONE;
					}
				}
				if (entry->inverseSupported &&
					CS_stricmp (entry->srcDatum,trgDtmName) == 0)
				{
					if (result == cs_GXIDX_NOXFRM)
					{
						result = index;
						myDirection = cs_DTCDIR_INV;
					}
					else
					{
						result = cs_GXIDX_DUPXFRM;
						myDirection = cs_DTCDIR_NONE;
					}
				}
			}
		}
	}
	if (result >= 0)
	{
		*direction = myDirection;
	}
	return result;
}
int CS_locateGxByDatum (unsigned startAt,Const char* srcDtmName,Const char* trgDtmName)
{
	int result;
	int srcMtch;
	int trgMtch;

	unsigned index;
	unsigned indexCount;

	struct cs_GxIndex_* entry;
	struct cs_GxIndex_* gxIndex;

	result = cs_GXIDX_ERROR;
	gxIndex = CS_getGxIndexPtr ();
	if (gxIndex != NULL)
	{
		indexCount = CS_getGxIndexCount ();
		if (indexCount > 0)
		{
			result = cs_GXIDX_NOXFRM;
			for (index = startAt;index < indexCount;index += 1)
			{
				entry = (gxIndex + index);
				srcMtch = (CS_stricmp (entry->srcDatum,srcDtmName) == 0);
				trgMtch = (CS_stricmp (entry->trgDatum,trgDtmName) == 0);
				if (srcMtch && trgMtch)
				{
					result = (int)index;
					break;
				}
			}
		}
	}
	return result;
}

/* Select the better suitable geodetic transformation out of two.
   Selection is related on the lower index. It is expected that a user defined definition comes with
   a lower index then a system one. */
int CS_selectAccurateGxIndex(unsigned indexGxA, unsigned indexGxB)
{
    unsigned indexCount;

    indexCount = CS_getGxIndexCount ();
    if (indexGxA >= indexCount)
        return cs_GXIDX_ERROR;
    if (indexGxB >= indexCount)
        return cs_GXIDX_ERROR;

    // In a later step maybe select the one with lower accuracy.
    // Get entry with lower index
    return (indexGxA <= indexGxB) ? indexGxA : indexGxB;
}

int CS_locateGxByDatum2 (int* direction,Const char* srcDtmName,Const char* trgDtmName)
{
	extern char csErrnam [MAXPATH];

	int result;
	int chosenResult;

	unsigned startAt;
	unsigned xfrmCount;

	Const struct cs_GxIndex_* gxIdxPtr;

	char errMsg [256];

	/* We haven't found anything yet. */
	chosenResult = cs_GXIDX_NOXFRM;		/* Code below requires this to be negative */
	*direction = cs_DTCDIR_NONE;

	/* Make a linear search through the Geodetic Path dictionary looking
	   for an entry where the source and target match the provided names.
	   We look for transformations in the forward direction first; and use
	   any unique definition which we locate this way.

	   We always search the entire dictionary and count all the matching
	   entries.  Thus, we know if the entry we found was indeed unique
	   or not. */
	result = 0;
	startAt = 0;
	xfrmCount = 0;
	while (result >= 0)
	{
		result = CS_locateGxByDatum (startAt,srcDtmName,trgDtmName);
		if (result >= 0)
		{
			xfrmCount += 1;
			if (chosenResult < 0)
			{
				chosenResult = result;
			}
			else
			{
				// When more than one suitable geodetic transformation is found the best one is selected.
				chosenResult = CS_selectAccurateGxIndex((unsigned int)chosenResult,(unsigned int)result);
				if (chosenResult < 0) goto error;
			}
			startAt = (unsigned)result + 1;
		}
	}
	if (chosenResult >= 0)
	{
		if (xfrmCount >= 1)
		{
			*direction = cs_DTCDIR_FWD;
		}
		else
		{
			chosenResult = cs_GXIDX_DUPXFRM;
			sprintf (errMsg,"%s to %s",srcDtmName,trgDtmName);
			CS_stncp (csErrnam,errMsg,MAXPATH);
			CS_erpt (cs_GEOXFRM_DUP);
			goto error;
		}
	}
	else
	{
		/* Here if the above search didn't pan out.  We look for reversible
		   transformations where the source and target datums provided match
		   target and source datums in the transformations. */
		result = 0;
		startAt = 0;
		xfrmCount = 0;
		while (result >= 0)
		{
			result = CS_locateGxByDatum (startAt,trgDtmName,srcDtmName);
			if (result >= 0)
			{
				/* Since a positive result value indicates that the entry
				   does indeed exist, the following function cannot fail to
				   produce a valid pointer. */
				gxIdxPtr = CS_getGxIndexEntry ((unsigned)result);
				if (gxIdxPtr->inverseSupported)		/*lint !e613  possible use of null pointer */
				{
					xfrmCount += 1;
					if (chosenResult < 0)
					{
						chosenResult = result;
					}
					else
					{
						/* When more than one suitable geodetic transformation is found the best one is selected. */
						chosenResult = CS_selectAccurateGxIndex((unsigned int)chosenResult,(unsigned int)result);
						if (chosenResult < 0) goto error;
					}
				}
				startAt = (unsigned)result + 1;
			}
		}
		if (chosenResult >= 0)
		{
			if (xfrmCount >= 1)
			{
				*direction = cs_DTCDIR_INV;
			}
			else
			{
				chosenResult = cs_GXIDX_DUPXFRM;
				sprintf (errMsg,"%s to %s",srcDtmName,trgDtmName);
				CS_stncp (csErrnam,errMsg,MAXPATH);
				CS_erpt (cs_GEOXFRM_DUP);
				goto error;
			}
		}
	}
	return chosenResult;
error:
	return cs_GXIDX_ERROR;
}

/* This function actually generates the index.  It is not intended for use by
   an application. */
void CSgenerateGxIndex (void)
{
	extern struct cs_GxIndex_* cs_GxIndex;
	extern unsigned cs_GxIndexCount;

	int i;
	int gxCount;

	struct cs_GxIndex_* gxIdxPtr;

	struct cs_GeodeticTransform_* pGxDef;
	struct cs_GeodeticTransform_** pAllGxDefs;

	pAllGxDefs = NULL;
	gxCount = 0;

	CS_releaseGxIndex();

	gxCount = CS_gxDefinitionAll(&pAllGxDefs);
	if (gxCount < 0)
	{
		goto error;
	}

	//malloc' the array of [cs_GxIndex] entries - note, that this included 1 more padding item
	cs_GxIndex = (struct cs_GxIndex_*)CS_malc (sizeof (struct cs_GxIndex_) * (gxCount + 1));
	if (NULL == cs_GxIndex)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	//get hold onto the first index entry in the array...
	gxIdxPtr = cs_GxIndex;
	for(i = 0; i < gxCount; ++i)
	{
		//...and fill it with the information we've got from the dictionaries
		pGxDef = pAllGxDefs[i];
		CS_stncp (gxIdxPtr->xfrmName,pGxDef->xfrmName,sizeof (gxIdxPtr->xfrmName));
		CS_stncp (gxIdxPtr->srcDatum,pGxDef->srcDatum,sizeof (gxIdxPtr->srcDatum));
		CS_stncp (gxIdxPtr->trgDatum,pGxDef->trgDatum,sizeof (gxIdxPtr->trgDatum));
		gxIdxPtr->accuracy = pGxDef->accuracy;
		gxIdxPtr->inverseSupported = pGxDef->inverseSupported;
		gxIdxPtr->methodCode = pGxDef->methodCode;
		++gxIdxPtr; //forward our pointer to the next entry in the array
	}

	//zero out the last [cs_GxIndex_] entry
	//if [gxCount] had been 0, [gxIdxPtr] still points to the beginning of [cs_GxIndex]
	gxIdxPtr->xfrmName [0] = '\0';
	gxIdxPtr->srcDatum [0] = '\0';
	gxIdxPtr->trgDatum [0] = '\0';
	gxIdxPtr->accuracy = 0.0;
	gxIdxPtr->inverseSupported = 0;
	
	cs_GxIndexCount = (unsigned) gxCount;

	if (NULL != pAllGxDefs)
	{
		for(i = 0; i < gxCount; ++i)
		{
			CS_free((pAllGxDefs)[i]);
		}

		CS_free(pAllGxDefs);
		pAllGxDefs = NULL;
	}

	return;

error:
	if (NULL != pAllGxDefs)
	{
		for(i = 0; i < gxCount; ++i)
		{
			CS_free((pAllGxDefs)[i]);
		}

		CS_free(pAllGxDefs);
		pAllGxDefs = NULL;
	}

	CS_releaseGxIndex ();
	return;
}
