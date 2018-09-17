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
#include "cs_Legacy.h"

/* This function will extract the definition of the named Geodetic
   Transformation from the dictionary and then use that definition to
   create a fully initialized cs_GxXform_ structure which can actually
   be used to perform Geodetic Transformations in a generic way. */
struct cs_GxXform_* EXP_LVL1 CS_gxloc (Const char* gxDefName, short userDirection)
{
    struct cs_GeodeticTransform_* xfrmDefPtr;
    struct cs_GxXform_* transform;

    /* Get the datum definition. */
	xfrmDefPtr = CS_gxdef (gxDefName);
	if (xfrmDefPtr == NULL)
	{
        //error has already been reported; we don't know here anyway whether we got NULL
        //due to an OOM situation or due to an incorrect definition name
        return NULL;
	}

    transform = CS_gxloc1(xfrmDefPtr, userDirection);
    CS_free(xfrmDefPtr); //[xfrmDefPtr] is not NULL at this point

    return transform; //can be NULL
}

struct cs_GxXform_* EXP_LVL1 CS_gxloc1 (Const struct cs_GeodeticTransform_ *xfrmDefPtr,short userDirection)
{
	extern char csErrnam [];
	extern struct cs_XfrmTab_ cs_XfrmTab [];

	int status;
	int errorCount;

	struct cs_GxXform_ *xfrmPtr;
	struct cs_Datum_ *srcDtPtr;
	struct cs_Datum_ *trgDtPtr;
	struct cs_XfrmTab_* xfrmTabPtr;

	int err_list [4];

	if (NULL == xfrmDefPtr)
	{
		CS_erpt (cs_ERSUP_SOFT);
		return NULL;
	}

	/* Prepare for any type of error. */

	xfrmPtr = NULL;
	srcDtPtr = NULL;
	trgDtPtr = NULL;

	xfrmPtr = (struct cs_GxXform_*)CS_malc (sizeof (struct cs_GxXform_));
	if (xfrmPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	memset (xfrmPtr,0,sizeof (struct cs_GxXform_));
	xfrmPtr->isNullXfrm = FALSE;
	xfrmPtr->userDirection = userDirection;

	/* Check the definition for validity.  The dictionary compiler
	   also performs this test, but we do it everytime here in case
	   a program somewhere adjusted the dictionary outside of the
	   "official" compiler. */
	errorCount = CS_gxchk (xfrmDefPtr,0,err_list,sizeof (err_list) / sizeof (int));
	if (errorCount)
	{
		CS_erpt (err_list [0]);
		goto error;
	}

	/* Transfer the method independent stuff from the definition to the
	   implementation structure. */
	memcpy (&xfrmPtr->gxDef,xfrmDefPtr,sizeof (struct cs_GeodeticTransform_));

	srcDtPtr = CS_dtloc (xfrmDefPtr->srcDatum);
	if (srcDtPtr == NULL)
	{
		goto error;
	}

	memcpy (&xfrmPtr->srcDatum,srcDtPtr,sizeof (struct cs_Datum_));
	CS_free (srcDtPtr);
	srcDtPtr = NULL;

	trgDtPtr = CS_dtloc (xfrmDefPtr->trgDatum);
	if (trgDtPtr == NULL)
	{
		goto error;
	}
	memcpy (&xfrmPtr->trgDatum,trgDtPtr,sizeof (struct cs_Datum_));
	CS_free (trgDtPtr);
	trgDtPtr = NULL;

	CS_stncp (xfrmPtr->xfrmName,xfrmDefPtr->xfrmName,sizeof (xfrmPtr->xfrmName));
	CS_stncp (xfrmPtr->group,xfrmDefPtr->group,sizeof (xfrmPtr->group));
	CS_stncp (xfrmPtr->description,xfrmDefPtr->description,sizeof (xfrmPtr->description));
	CS_stncp (xfrmPtr->source,xfrmDefPtr->source,sizeof (xfrmPtr->source));

	xfrmPtr->methodCode = xfrmDefPtr->methodCode;
	xfrmPtr->epsgNbr = (xfrmDefPtr->epsgCode >= 0) ? (unsigned long)xfrmDefPtr->epsgCode : 0UL;		/*lint !e571  suspicious cast */
	xfrmPtr->epsgVar = xfrmDefPtr->epsgVariation;
	xfrmPtr->inverseSupported = xfrmDefPtr->inverseSupported;
	xfrmPtr->maxIterations = xfrmDefPtr->maxIterations;
	xfrmPtr->protect = xfrmDefPtr->protect;
	xfrmPtr->cnvrgValue = xfrmDefPtr->cnvrgValue;
	xfrmPtr->errorValue = xfrmDefPtr->errorValue;
	xfrmPtr->accuracy = xfrmDefPtr->accuracy;

	/* Initialize this transformation. */
	for (xfrmTabPtr = cs_XfrmTab;xfrmTabPtr->methodCode != cs_DTCMTH_NONE;xfrmTabPtr++)
	{
		if (xfrmTabPtr->methodCode == xfrmPtr->methodCode)
		{
			break;
		}
	}
	if (xfrmTabPtr->methodCode == cs_DTCMTH_NONE)
	{
		CS_stncp (csErrnam,"<unknown>",MAXPATH);
		CS_erpt (cs_UNKWN_DTCMTH);
		goto error;
	}
	status = (*xfrmTabPtr->initialize)(xfrmPtr);
	if (status != 0)
	{
		goto error;
	}

	return xfrmPtr;

error:
	if (xfrmPtr != NULL)		/*lint !e774  boolean always evaluates to true */
	{
		CS_free (xfrmPtr);
		xfrmPtr = NULL;
	}
	if (srcDtPtr != NULL)		/*lint !e774  boolean always evaluates to true */
	{
		CS_free (srcDtPtr);
		srcDtPtr = NULL;
	}
	if (trgDtPtr != NULL)		/*lint !e774  boolean always evaluates to true */
	{
		CS_free (trgDtPtr);
		trgDtPtr = NULL;
	}

	//don't free [xfrmDefPtr] - owned by the caller

	return NULL;
}

/* A fallback for strange situations. */
struct cs_GxXform_*	EXP_LVL3 CS_gxlocDtm (Const struct cs_Datum_ *src_dt,Const struct cs_Datum_ *dst_dt)
{
	extern double cs_Zero;
	extern double cs_Three;
	
	extern double cs_Five;
	extern double cs_Eight;

	extern char csErrnam [];
	extern struct cs_XfrmTab_ cs_XfrmTab [];

	int status;

	struct cs_GxXform_ *xfrmPtr;
	struct cs_XfrmTab_* xfrmTabPtr;

	char chrTemp [256];

    /* Prepare for any type of error. */
	xfrmPtr = NULL;

	/* Before we get too far into this, verify that this will actually work. */
	if (src_dt->to84_via != cs_DTCTYP_MOLO && src_dt->to84_via != cs_DTCTYP_BURS &&
											  src_dt->to84_via != cs_DTCTYP_7PARM)
	{
		goto error;
	}
	if (dst_dt->to84_via != cs_DTCTYP_WGS84)
	{
		goto error;
	}
	/* OK, this should work just fine. */

	xfrmPtr = (struct cs_GxXform_*)CS_malc (sizeof (struct cs_GxXform_));
	if (xfrmPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	memset (xfrmPtr,0,sizeof (struct cs_GxXform_));

	memcpy (&xfrmPtr->srcDatum,src_dt,sizeof (xfrmPtr->srcDatum));
	memcpy (&xfrmPtr->trgDatum,dst_dt,sizeof (xfrmPtr->trgDatum));

	xfrmPtr->isNullXfrm = FALSE;
	xfrmPtr->epsgVar = 0;
	xfrmPtr->inverseSupported = TRUE;
	xfrmPtr->maxIterations = 8;
	xfrmPtr->protect = 0;
	xfrmPtr->userDirection = cs_DTCDIR_FWD;
	xfrmPtr->epsgNbr = 0UL;
	xfrmPtr->cnvrgValue = 1.0E-09;
	xfrmPtr->errorValue = 1.0E-06;
	xfrmPtr->accuracy = cs_Eight;

	sprintf (chrTemp,"%s_to_%s",src_dt->key_nm,dst_dt->key_nm);
	CS_stncp (xfrmPtr->xfrmName,chrTemp,sizeof (xfrmPtr->xfrmName));
	CS_stncp (xfrmPtr->group,"USER",sizeof (xfrmPtr->group));
	CS_stncp (xfrmPtr->description,src_dt->dt_name,sizeof (xfrmPtr->description));
	CS_stncp (xfrmPtr->source,"Automatic conversion from CS-MAP 12.02 or earlier",sizeof (xfrmPtr->source));

	CS_stncp (xfrmPtr->gxDef.xfrmName,chrTemp,sizeof (xfrmPtr->gxDef.xfrmName));
	CS_stncp (xfrmPtr->gxDef.srcDatum,src_dt->key_nm,sizeof (xfrmPtr->gxDef.srcDatum));
	CS_stncp (xfrmPtr->gxDef.trgDatum,dst_dt->key_nm,sizeof (xfrmPtr->gxDef.trgDatum));
	CS_stncp (xfrmPtr->gxDef.group,"USER",sizeof (xfrmPtr->gxDef.group));
	CS_stncp (xfrmPtr->gxDef.description,src_dt->dt_name,sizeof (xfrmPtr->gxDef.description));
	CS_stncp (xfrmPtr->gxDef.source,"Automatic conversion from CS-MAP 12.02 or earlier",sizeof (xfrmPtr->gxDef.source));
	xfrmPtr->gxDef.epsgCode = 0;
	xfrmPtr->gxDef.epsgVariation = 0;
	xfrmPtr->gxDef.inverseSupported = 0;
	xfrmPtr->gxDef.maxIterations = xfrmPtr->maxIterations;
	xfrmPtr->gxDef.cnvrgValue = xfrmPtr->cnvrgValue;
	xfrmPtr->gxDef.errorValue = xfrmPtr->errorValue;
	xfrmPtr->gxDef.rangeMinLng = cs_Zero;
	xfrmPtr->gxDef.rangeMaxLng = cs_Zero;
	xfrmPtr->gxDef.rangeMinLat = cs_Zero;
	xfrmPtr->gxDef.rangeMaxLat = cs_Zero;

	xfrmPtr->gxDef.parameters.geocentricParameters.deltaX  = src_dt->delta_X;
	xfrmPtr->gxDef.parameters.geocentricParameters.deltaY  = src_dt->delta_Y;
	xfrmPtr->gxDef.parameters.geocentricParameters.deltaZ  = src_dt->delta_Z;
	xfrmPtr->gxDef.parameters.geocentricParameters.rotateX = src_dt->rot_X;
	xfrmPtr->gxDef.parameters.geocentricParameters.rotateY = src_dt->rot_Y;
	xfrmPtr->gxDef.parameters.geocentricParameters.rotateZ = src_dt->rot_Z;
	xfrmPtr->gxDef.parameters.geocentricParameters.scale   = src_dt->bwscale;

	if (src_dt->to84_via == cs_DTCTYP_MOLO)
	{
		xfrmPtr->gxDef.methodCode = cs_DTCMTH_MOLOD;
        xfrmPtr->methodCode = cs_DTCMTH_MOLOD;
		xfrmPtr->accuracy = cs_Eight;
	}
	else if (src_dt->to84_via == cs_DTCTYP_BURS)
	{
		xfrmPtr->gxDef.methodCode = cs_DTCMTH_BURSA;
        xfrmPtr->methodCode = cs_DTCMTH_BURSA;
		xfrmPtr->accuracy = cs_Five;
	}
	else if (src_dt->to84_via == cs_DTCTYP_7PARM)
	{
		xfrmPtr->gxDef.methodCode = cs_DTCMTH_7PARM;
        xfrmPtr->methodCode = cs_DTCMTH_7PARM;
		xfrmPtr->accuracy = cs_Three;
	}

	/* Initialize this transformation. */
	for (xfrmTabPtr = cs_XfrmTab;xfrmTabPtr->methodCode != cs_DTCMTH_NONE;xfrmTabPtr++)
	{
		if (xfrmTabPtr->methodCode == xfrmPtr->methodCode)
		{
			break;
		}
	}
	if (xfrmTabPtr->methodCode == cs_DTCMTH_NONE)
	{
		CS_stncp (csErrnam,"<unknown>",MAXPATH);
		CS_erpt (cs_UNKWN_DTCMTH);
		goto error;
	}
	status = (*xfrmTabPtr->initialize)(xfrmPtr);
	if (status != 0)
	{
		goto error;
	}
	return xfrmPtr;

error:
	if (xfrmPtr != NULL)
	{
		CS_free (xfrmPtr);
		xfrmPtr = NULL;
	}
	return NULL;
}
int EXP_LVL1 CS_gxFrwrd3D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3])
{
	int gxStatus;
	
	gxStatus = (*xform->frwrd3D)(&xform->xforms,trgLl,srcLl);
	return gxStatus;
}
int EXP_LVL1 CS_gxFrwrd2D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3])
{
	int gxStatus;
	
	gxStatus = (*xform->frwrd2D)(&xform->xforms,trgLl,srcLl);
	return gxStatus;
}
int EXP_LVL1 CS_gxInvrs3D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3])
{
	int gxStatus;
	
	gxStatus = (*xform->invrs3D)(&xform->xforms,trgLl,srcLl);
	return gxStatus;
}
int EXP_LVL1 CS_gxInvrs2D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3])
{
	int gxStatus;
	
	gxStatus = (*xform->invrs2D)(&xform->xforms,trgLl,srcLl);
	return gxStatus;
}
int EXP_LVL1 CS_gxchk (Const struct cs_GeodeticTransform_ *gxXform,unsigned short gxChkFlg,int err_list [],int list_sz)
{
	extern struct cs_XfrmTab_ cs_XfrmTab[];
	extern char csErrnam [MAXPATH];

	int st;
	int ii;
	int err_cnt;

	struct cs_XfrmTab_* tblPtr;

	err_cnt = -1;

	/* Locate the geodetic transform method in the table, and thus
	   verify the validity of the method code. */
	for (tblPtr = cs_XfrmTab;tblPtr->methodCode != cs_DTCMTH_NONE;tblPtr += 1)
	{
		if (tblPtr->methodCode == gxXform->methodCode)
		{
			break;
		}
	}
	if (tblPtr->methodCode == cs_DTCMTH_NONE)
	{
		/* Invalid method code. */
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_MTHCODE;
	}
	else
	{
		/* Call the check function for the indicated method code. */
		err_cnt += (*tblPtr->check)(gxXform,tblPtr->methodCode,err_list,list_sz);
	}

	/* Check the stuff which applies to all definitions.
	   Verify that the name is a valid transformation name. */
	st = CS_nampp64 (gxXform->xfrmName);
	if (st != 0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_XFRMNM;
	}

	/* Verify the two datum names are valid names but also names of
	   datums which actually exist. */
	st = CS_nampp (gxXform->srcDatum);
	if (st != 0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_SRCDTNM;
	}
	else if ((gxChkFlg & cs_GXCHK_DATUM) != 0)
	{
		st = CS_dtIsValid (gxXform->srcDatum);
		if (st == 0)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_SRCDTNM;
		}
	}

	st = CS_nampp (gxXform->trgDatum);
	if (st != 0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_TRGDTNM;
	}
	else if ((gxChkFlg & cs_GXCHK_DATUM) != 0)
	{
		st = CS_dtIsValid (gxXform->trgDatum);
		if (st == 0)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_TRGDTNM;
		}
	}

	/* Verify maxIterations, convergence value, error value, and accuracy. */
	if (gxXform->maxIterations < 0 || gxXform->maxIterations >= 40)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_MAXITR;
	}
	if (gxXform->cnvrgValue <= 1.0E-16 || gxXform->cnvrgValue >= 1.0E-02)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_CNVRGV;
	}
	if (gxXform->errorValue <= 1.0E-14 || gxXform->cnvrgValue >= 1.0 ||
	    gxXform->errorValue <= gxXform->cnvrgValue)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ERRORV;
	}
	if (gxXform->accuracy < 0.0 || gxXform->accuracy >= 1000.0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ACCRCY;
	}

	/* If so instructed, report all of the errors detected so far. */
	if ((gxChkFlg & cs_GXCHK_REPORT) != 0)
	{
		CS_stncp (csErrnam,gxXform->xfrmName,MAXPATH);
		for (ii = 0;ii <= err_cnt && ii < list_sz;ii++)
		{
			CS_erpt (err_list [ii]);
		}
	}
	return (err_cnt + 1);
}
int EXP_LVL1 CS_gxIsNull (struct cs_GxXform_ *xfrmPtr)
{
	extern struct cs_XfrmTab_ cs_XfrmTab[];

	int isNull;
	struct cs_XfrmTab_* tblPtr;

	/* Locate the geodetic transform method in the table, and thus
	   verify the validity of the method code. */
	for (tblPtr = cs_XfrmTab;tblPtr->methodCode != cs_DTCMTH_NONE;tblPtr += 1)
	{
		if (tblPtr->methodCode == xfrmPtr->methodCode)
		{
			break;
		}
	}
	isNull = FALSE;
	if (tblPtr->methodCode != cs_DTCMTH_NONE)
	{
		/* Call the "isNull" function for the indicated method code. */
		isNull = (*tblPtr->isNull)(&xfrmPtr->xforms);
	}
	return isNull;
}
void EXP_LVL1 CS_gxDisable (struct cs_GxXform_ *xfrmPtr)
{
	xfrmPtr->isNullXfrm = TRUE;
}
int	EXP_LVL1 CS_isGxfrmReentrant (Const struct cs_GxXform_ *gxXform)
{
	extern char csErrnam [MAXPATH];
	extern struct cs_XfrmTab_ cs_XfrmTab[];
	extern struct cs_GridFormatTab_ cs_GridFormatTab [];

	short idx;
	short fileCount;
	int isReentrant;
	int filesAreReentrant;
	unsigned short tblFormatCode; 

	struct cs_XfrmTab_* tblPtr;
	struct cs_GridFormatTab_* frmtTblPtr;
	Const struct cs_GridFile_* fileDefPtr;
	Const struct csGridi_ *filesPtr;

	/* Locate the entry in the transformation table for this transformation
	   method. */
	isReentrant = -1;
	if (gxXform != NULL)
	{
		for (tblPtr = cs_XfrmTab;tblPtr->methodCode != cs_DTCMTH_NONE;tblPtr += 1)
		{
			if (tblPtr->methodCode == gxXform->methodCode)
			{
				break;
			}
		}
		if (tblPtr->methodCode != cs_DTCMTH_NONE)
		{
			if (tblPtr->methodCode == cs_DTCMTH_GFILE)
			{
				/* Reentrant status dependent upon the file format.  The
				   transformation method of all files in the list must be
				   reentrant if the result is to be reentrant. */
				filesAreReentrant = TRUE;				/* Until we know different. */
				filesPtr = &gxXform->xforms.gridi;
				fileCount = gxXform->xforms.gridi.fileCount;
				for (idx = 0;idx < fileCount && filesAreReentrant == TRUE;idx += 1)
				{
					fileDefPtr = filesPtr->gridFiles [idx];

					/* Locate the file format in the list table.  This is
					   another unfortunate, but necessary, kludge.  The file
					   format stored in the Grid Format Table is a define code
					   value, while the value in the cs_GridFile_ structure is
					   an enumerator.  The result of getting things done in a
					   compressed development cycle.  A bit ugly, but I've seen
					   worse.

					   So, we simply need to convert the enumerator in the cs_GridFile_
					   structure to the appropriate which exists in the cs_GridFormatTab. */
					switch (fileDefPtr->format) {
					case gridFrmtNTv1:  tblFormatCode = cs_DTCFRMT_CNTv1; break;
					case gridFrmtNTv2:  tblFormatCode = cs_DTCFRMT_CNTv2; break;
					case gridFrmtNadcn: tblFormatCode = cs_DTCFRMT_NADCN; break;
					case gridFrmtFrnch: tblFormatCode = cs_DTCFRMT_FRNCH; break;
					case gridFrmtJapan: tblFormatCode = cs_DTCFRMT_JAPAN; break;
					case gridFrmtAts77: tblFormatCode = cs_DTCFRMT_ATS77; break;
					case gridFrmtOst97: tblFormatCode = cs_DTCFRMT_OST97; break;
					case gridFrmtOst02: tblFormatCode = cs_DTCFRMT_OST02; break;
					case gridFrmtGeocn: tblFormatCode = cs_DTCFRMT_GEOCN; break;
					case gridFrmtNone:
					case gridFrmtUnknown:
					default:
						tblFormatCode = cs_DTCFRMT_NONE;
						CS_stncp (csErrnam,"CS_gxprm:1",MAXPATH);
						CS_erpt (cs_ISER);
						break;
					}

					for (frmtTblPtr = cs_GridFormatTab;frmtTblPtr->formatCode != cs_DTCFRMT_NONE;frmtTblPtr += 1)
					{
						if (frmtTblPtr->formatCode == tblFormatCode)
						{
							break;
						}
					}
					if (frmtTblPtr->formatCode != cs_DTCFRMT_NONE)
					{
						filesAreReentrant = ((frmtTblPtr->formatFlags & cs_FRMTFLG_RNTRNT) != 0) ? TRUE : FALSE;
					}
					else
					{
						filesAreReentrant = -1;
					}
				}
				isReentrant = filesAreReentrant;
			}
			else
			{
				/* The method flags indicate the nature of the transformation. */
				isReentrant = ((tblPtr->methodFlags & cs_XFRMFLG_RNTRNT) != 0);
			}
		}
		else
		{
			CS_stncp (csErrnam,"<unknown>",MAXPATH);
			CS_erpt (cs_UNKWN_DTCMTH);
		}
	}
	else
	{
		CS_stncp (csErrnam,"CS_gxprm:2",MAXPATH);
		CS_erpt (cs_ISER);
	}
	return isReentrant;
}
int EXP_LVL1 CS_isGxDefReentrant (Const struct cs_GeodeticTransform_ *gxDef)
{
	extern char csErrnam [MAXPATH];
	extern struct cs_XfrmTab_ cs_XfrmTab[];
	extern struct cs_GridFormatTab_ cs_GridFormatTab [];

	short idx;
	short fileCount;
	int isReentrant;
	int filesAreReentrant;

	struct cs_XfrmTab_* tblPtr;
	struct cs_GridFormatTab_* frmtTblPtr;
	struct csGeodeticXfromParmsFile_* fileDefPtr;
	struct csGeodeticXformParmsGridFiles_* filesPtr;

	isReentrant = -1;
	if (gxDef != NULL)
	{
		for (tblPtr = cs_XfrmTab;tblPtr->methodCode != cs_DTCMTH_NONE;tblPtr += 1)
		{
			if (tblPtr->methodCode == gxDef->methodCode)
			{
				break;
			}
		}
		if (tblPtr->methodCode != cs_DTCMTH_NONE)
		{
			if (tblPtr->methodCode == cs_DTCMTH_GFILE)
			{
				/* Reentrant status dependent upon the file format. */
				filesAreReentrant = TRUE;				/* Until we know different. */
				filesPtr = &gxDef->parameters.fileParameters;
				fileCount = filesPtr->fileReferenceCount;
				for (idx = 0;idx < fileCount && filesAreReentrant == TRUE;idx += 1)
				{
					fileDefPtr = &filesPtr->fileNames [idx];

					/* Locate the file format in the list table. */
					for (frmtTblPtr = cs_GridFormatTab;frmtTblPtr->formatCode != cs_DTCFRMT_NONE;frmtTblPtr += 1)
					{
						if (frmtTblPtr->formatCode == fileDefPtr->fileFormat)
						{
							break;
						}
					}
					if (frmtTblPtr->formatCode != cs_DTCFRMT_NONE)
					{
						filesAreReentrant = ((frmtTblPtr->formatFlags & cs_FRMTFLG_RNTRNT) != 0) ? TRUE : FALSE;
					}
					else
					{
						filesAreReentrant = -1;
					}
				}
				isReentrant = filesAreReentrant;
			}
			else
			{
				/* The method flags indicate the nature of the transformation. */
				isReentrant = ((tblPtr->methodFlags & cs_XFRMFLG_RNTRNT) != 0);
			}
		}
		else
		{
			CS_stncp (csErrnam,"<unknown>",MAXPATH);
			CS_erpt (cs_UNKWN_DTCMTH);
		}
	}
	else
	{
		CS_stncp (csErrnam,"CS_gxprm:3",MAXPATH);
		CS_erpt (cs_ISER);
	}
	return isReentrant;
}
