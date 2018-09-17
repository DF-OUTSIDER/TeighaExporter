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

struct csDtmBridge_* CSnewDtmBridge (Const struct cs_Datum_* src_dt,
									 Const struct cs_Datum_* dst_dt)
{
	int idx;
	struct csDtmBridge_ *bridgePtr;

	bridgePtr = (struct csDtmBridge_*) CS_malc (sizeof (struct csDtmBridge_));
	if (bridgePtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Initialize as appropriate. */
	bridgePtr->srcIndex = -1;
	CS_stncp (bridgePtr->srcDatumName,src_dt->key_nm,sizeof (bridgePtr->srcDatumName));
	for (idx = 0;idx < csPATH_MAXXFRM;idx += 1)
	{
		bridgePtr->bridgeXfrms [idx].xfrmPtr = NULL;
		bridgePtr->bridgeXfrms [idx].direction = cs_DTCDIR_NONE;
	}
	CS_stncp (bridgePtr->trgDatumName,dst_dt->key_nm,sizeof (bridgePtr->trgDatumName));
	bridgePtr->trgIndex = csPATH_MAXXFRM;
	return bridgePtr;
error:
	if (bridgePtr != NULL)			/*lint !e774  boolean always evaluates to true */
	{	
		CS_free (bridgePtr);
		bridgePtr = NULL;
	}
	return NULL;
}
Const char* CSdtmBridgeGetSourceDtm (struct csDtmBridge_* thisPtr)
{
	extern char csErrnam [MAXPATH];

	Const char* cpSrc;
	Const struct csDtmBridgeXfrm_* bridgeXfrmPtr;
	
	if (thisPtr->srcIndex >= 0)
	{
		bridgeXfrmPtr = &thisPtr->bridgeXfrms[thisPtr->srcIndex];
		if (bridgeXfrmPtr->direction == cs_DTCDIR_FWD)
		{
			cpSrc = bridgeXfrmPtr->xfrmPtr->trgDatum;
		}
		else if (bridgeXfrmPtr->direction == cs_DTCDIR_INV)
		{
			cpSrc = bridgeXfrmPtr->xfrmPtr->srcDatum;
		}
		else
		{
			CS_stncp (csErrnam,"CS_dtmBridge:1",MAXPATH);
			CS_erpt (cs_ISER);
			goto error;
		}
	}
	else
	{
		cpSrc = thisPtr->srcDatumName;
	}
	return cpSrc;
error:
	return NULL;
}
Const char* CSdtmBridgeGetTargetDtm (struct csDtmBridge_* thisPtr)
{
	extern char csErrnam [MAXPATH];

	Const char* cpTrg;
	Const struct csDtmBridgeXfrm_* bridgeXfrmPtr;
	
	if (thisPtr->trgIndex < csPATH_MAXXFRM)
	{
		bridgeXfrmPtr = &thisPtr->bridgeXfrms[thisPtr->trgIndex];
		if (bridgeXfrmPtr->direction == cs_DTCDIR_FWD)
		{
			cpTrg = bridgeXfrmPtr->xfrmPtr->srcDatum;
		}
		else if (bridgeXfrmPtr->direction == cs_DTCDIR_INV)
		{
			cpTrg = bridgeXfrmPtr->xfrmPtr->trgDatum;
		}
		else
		{
			CS_stncp (csErrnam,"CS_dtmBridge:2",MAXPATH);
			CS_erpt (cs_ISER);
			goto error;
		}
	}
	else
	{
		cpTrg = thisPtr->trgDatumName;
	}
	return cpTrg;
error:
	return NULL;
}

int CSdtmBridgeIsComplete (struct csDtmBridge_* thisPtr)
{
	int complete;

	unsigned idx;
	unsigned xfrmPtrsCnt;

	Const char* dtmFrom;
	Const char* dtmThis;
	Const char* dtmNext;
	Const char* dtmTo;
	struct cs_GxIndex_* xfrmPtr;

	short directions [csPATH_MAXXFRM];
	struct cs_GxIndex_* xfrmPtrs [csPATH_MAXXFRM];

	if (CS_strnicmp (thisPtr->srcDatumName,thisPtr->trgDatumName,cs_KEYNM_DEF) == 0)
	{
		/* This is the null bridge, it is, by definition, complete. */
		return cs_DTCBRG_COMPLETE;
	}

	if (thisPtr->srcIndex < 0 && thisPtr->trgIndex >= csPATH_MAXXFRM)
	{
		/* The bridge is empty.  It is, by definition, in the building phase. */
		return cs_DTCBRG_BUILDING;
	}

	/* We have stuff in the bridge.  We need to determine if the contents
	   define a continuous path from the bridge's initial source datum to
	   the bridge's initial target datum.  If so, it is then complete.
	   
	   We'll use the xfrmPtrs array to eliminate the gaps which may exist
	   inside the bridge. */
	for (idx = 0;idx < csPATH_MAXXFRM;idx += 1)
	{
		xfrmPtrs [idx]   = NULL;
		directions [idx] = cs_DTCDIR_NONE;
	}

	xfrmPtrsCnt = 0;
	for (idx = 0;idx < csPATH_MAXXFRM;idx += 1)
	{
		xfrmPtr = thisPtr->bridgeXfrms [idx].xfrmPtr;
		if (xfrmPtr == NULL) continue;
		xfrmPtrs [xfrmPtrsCnt] = xfrmPtr;
		directions [xfrmPtrsCnt++] = thisPtr->bridgeXfrms [idx].direction;
	}

	/* Now we can walk through the list of transformations and determine
	   if the trail of datums represents a complete path from the source
	   datum to the target datum. */
	complete = TRUE;				/* until we know different. */
	for (idx = 0;complete && idx < xfrmPtrsCnt;idx += 1)
	{
		if (idx == 0)
		{
			xfrmPtr = xfrmPtrs [idx];
			dtmFrom = (directions [idx] == cs_DTCDIR_FWD) ? xfrmPtr->srcDatum : xfrmPtr->trgDatum;
			complete = (CS_strnicmp (dtmFrom,thisPtr->srcDatumName,cs_KEYNM_DEF) == 0);
		}
		if (complete && ((idx + 1) < xfrmPtrsCnt))
		{
			xfrmPtr = xfrmPtrs [idx];
			dtmThis = (directions [idx] == cs_DTCDIR_FWD) ? xfrmPtr->trgDatum : xfrmPtr->srcDatum;
			xfrmPtr = xfrmPtrs [idx + 1];
			dtmNext = (directions [idx + 1] == cs_DTCDIR_FWD) ? xfrmPtr->srcDatum : xfrmPtr->trgDatum;
			complete = (CS_strnicmp (dtmThis,dtmNext,cs_KEYNM_DEF) == 0);
		}		
		if (complete && ((idx + 1) == xfrmPtrsCnt))
		{
			xfrmPtr = xfrmPtrs [idx];
			dtmTo = (directions [idx] == cs_DTCDIR_FWD) ? xfrmPtr->trgDatum : xfrmPtr->srcDatum;
			complete = (CS_strnicmp (dtmTo,thisPtr->trgDatumName,cs_KEYNM_DEF) == 0);
		}
	}
	return 	(complete) ? cs_DTCBRG_COMPLETE : cs_DTCBRG_BUILDING;
}
int CSdtmBridgeIsFull (Const struct csDtmBridge_* thisPtr)
{
	short deltaIdx;
	
	deltaIdx = thisPtr->trgIndex - thisPtr->srcIndex;
	return (deltaIdx <= 1);
}
int CSdtmBridgeAddSrcPath (struct csDtmBridge_* thisPtr,Const struct cs_GeodeticPath_* pathPtr,
														short direction)
{
	extern char csErrnam [MAXPATH];

	int gxIndex;
	int bridgeStatus;

	unsigned idx;
	unsigned idxCount;

	Const struct cs_GxIndex_* xfrmPtr;
	Const struct cs_GeodeticPathElement_* pathElePtr;

	idxCount = (unsigned)pathPtr->elementCount;		/*lint !e571  elementCount should be defined as unsigned */
	if (direction == cs_DTCDIR_FWD)
	{
		for (idx = 0;idx < idxCount;idx += 1)
		{
			/* Here once for each transformation in the provided Geodetic
			   Path definition. */
			pathElePtr = &pathPtr->geodeticPathElements [idx];
			
			/* The direction as it applies to the entire path is forward.
			   Thus, we copy the direction assigned to eaach element of the
			   provided path. */
			direction = pathElePtr->direction;
			gxIndex = CS_locateGxByName (pathElePtr->geodeticXformName);
			if (gxIndex < 0)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:3",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			xfrmPtr = CS_getGxIndexEntry ((unsigned int)gxIndex); 
			if (xfrmPtr == NULL)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:4",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			CSdtmBridgeAddSrcTransformation (thisPtr,xfrmPtr,direction);	/*lint !e534  ignoring return value */
		}
	}
	else if (direction == cs_DTCDIR_INV)
	{
		/* We need to use the inverse of the _ENTIRE_ path.  This implies
		   reverseing the order of the transformations. */
		for (idx = idxCount;idx > 0;idx -= 1)
		{
			/* Here once for each transformation in the provided Geodetic
			   Path definition. */
			pathElePtr = &pathPtr->geodeticPathElements [idx - 1];

			/* The direction as it applies to the entire path is inverse.
			   Thus, we copy the opposite direction assigned to each element
			   of the provided path. */
			direction = (pathElePtr->direction == cs_DTCDIR_FWD) ? cs_DTCDIR_INV : cs_DTCDIR_FWD;
			gxIndex = CS_locateGxByName (pathElePtr->geodeticXformName);
			if (gxIndex < 0)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:5",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			xfrmPtr = CS_getGxIndexEntry ((unsigned int)gxIndex); 
			if (xfrmPtr == NULL)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:6",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			CSdtmBridgeAddSrcTransformation (thisPtr,xfrmPtr,direction);	/*lint !e534  ignoring return value */
		}
	}
	else
	{
		CS_stncp (csErrnam,"CS_dtmBridge:7",MAXPATH);
		CS_erpt (cs_ISER);
		goto error;
	}
	bridgeStatus = CSdtmBridgeIsComplete (thisPtr);
	return bridgeStatus;
error:
	return cs_DTCBRG_ERROR;
}
int CSdtmBridgeAddSrcTransformation (struct csDtmBridge_* thisPtr,
									 Const struct cs_GxIndex_* xfrmPtr,
									 short direction)
{
	extern char csErrnam [MAXPATH];

	int bridgeStatus;

	struct csDtmBridgeXfrm_* bridgeXfrmPtr;

	char errMsg [256];

	if (CSdtmBridgeIsFull (thisPtr))
	{
		sprintf (errMsg,"%s to %s",thisPtr->srcDatumName,thisPtr->trgDatumName);
		CS_stncp (csErrnam,errMsg,MAXPATH);
		CS_erpt (cs_GX_TOOMANY);
		goto error;
	}
	thisPtr->srcIndex += 1;
	bridgeXfrmPtr = &thisPtr->bridgeXfrms[thisPtr->srcIndex];
	bridgeXfrmPtr->xfrmPtr = xfrmPtr;			/*lint !e158  increases capability */
	bridgeXfrmPtr->direction = direction;
	
	bridgeStatus = CSdtmBridgeIsComplete (thisPtr);
	return bridgeStatus;
error:
	return cs_DTCBRG_ERROR;
}

/* Things get tricky here when adding a path to the target side of the bridge.
   Keep in mind, that when adding transformations to the "target end of the
   bridge" we are working the detination of the bridge back towrd the source.
   
   Thus, when we are adding a path whose direction indication is forward,
   we need to work through the path from the end of the path toward the
   beginning of the path.
   
   Just the opposit of what you might think, or at least the way I originally
   coded this. */
   
int CSdtmBridgeAddTrgPath (struct csDtmBridge_* thisPtr,Const struct cs_GeodeticPath_* pathPtr,
														short direction)
{
	extern char csErrnam [MAXPATH];

	int gxIndex;
	int bridgeStatus;

	unsigned idx;
	unsigned idxCount;

	Const struct cs_GxIndex_* xfrmPtr;
	struct cs_GeodeticPathElement_* pathElePtr;

	idxCount = (unsigned)pathPtr->elementCount;		/*lint !e571  elementCount should be defined as unsigned */
	if (direction == cs_DTCDIR_FWD)
	{
		for (idx = idxCount;idx > 0;idx -= 1)
		{
			/* Here once for each transformation in the provided Geodetic
			   Path definition. */
			pathElePtr = &pathPtr->geodeticPathElements [idx - 1];					/*lint !e158  increases capability */
			
			/* The direction as it applies to the entire path is forward.
			   Thus, we copy the direction assigned to each element of the
			   provided path as is. */
			direction = pathElePtr->direction;
			gxIndex = CS_locateGxByName (pathElePtr->geodeticXformName);
			if (gxIndex < 0)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:8",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			xfrmPtr = CS_getGxIndexEntry ((unsigned int)gxIndex); 
			if (xfrmPtr == NULL)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:9",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			CSdtmBridgeAddTrgTransformation (thisPtr,xfrmPtr,direction);	/*lint !e534 ignoring return value */
		}
	}
	else if (direction == cs_DTCDIR_INV)
	{
		/* We need to use the inverse of the _ENTIRE_ path.  This implies
		   reverseing the order of the transformations. */
		for (idx = 0;idx < idxCount;idx += 1)
		{
			/* Here once for each transformation in the provided Geodetic
			   Path definition. */
			pathElePtr = &pathPtr->geodeticPathElements [idx];			/*lint !e158  increases capability */

			/* The direction as it applies to the entire path is inverse.
			   Thus, we copy the opposite of the direction assigned to each
			   element of the provided path. */
			direction = (pathElePtr->direction == cs_DTCDIR_FWD) ? cs_DTCDIR_INV : cs_DTCDIR_FWD;
			gxIndex = CS_locateGxByName (pathElePtr->geodeticXformName);
			if (gxIndex < 0)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:10",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			xfrmPtr = CS_getGxIndexEntry ((unsigned int)gxIndex); 
			if (xfrmPtr == NULL)
			{
				CS_stncp (csErrnam,"CS_dtmBridge:11",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
			CSdtmBridgeAddTrgTransformation (thisPtr,xfrmPtr,direction);	/*lint !e534 ignoring return value */
		}
	}
	else
	{
		CS_stncp (csErrnam,"CS_dtmBridge:12",MAXPATH);
		CS_erpt (cs_ISER);
		goto error;
	}
	bridgeStatus = CSdtmBridgeIsComplete (thisPtr);
	return bridgeStatus;
error:
	return cs_DTCBRG_ERROR;
}
int CSdtmBridgeAddTrgTransformation (struct csDtmBridge_* thisPtr,
									 Const struct cs_GxIndex_* xfrmPtr,
									 short direction)
{
	extern char csErrnam [MAXPATH];

	int bridgeStatus;

	struct csDtmBridgeXfrm_* bridgeXfrmPtr;
	
	char errMsg [256];

	if (CSdtmBridgeIsFull (thisPtr))
	{
		sprintf (errMsg,"%s to %s",thisPtr->srcDatumName,thisPtr->trgDatumName);
		CS_stncp (csErrnam,errMsg,MAXPATH);
		CS_erpt (cs_GX_TOOMANY);
		goto error;
	}
	thisPtr->trgIndex -= 1;
	bridgeXfrmPtr = &thisPtr->bridgeXfrms[thisPtr->trgIndex];
	bridgeXfrmPtr->xfrmPtr = xfrmPtr;			/*lint !e158  increases capability */
	bridgeXfrmPtr->direction = direction;

	bridgeStatus = CSdtmBridgeIsComplete (thisPtr);
	return bridgeStatus;
error:
	return cs_DTCBRG_ERROR;
}
