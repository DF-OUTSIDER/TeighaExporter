/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 *	* Neither the name of the Autodesk, Inc. nor the names of its
 *	  contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
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

/**********************************************************************
**	Calling Sequence:	strm = CS_gxopn (mode);
**
**	int mode;					mode of the file open, ala fcntl.h
**	csFILE *strm;				returns a file descriptor to the open file.
**
**	Will return NULL if a problem was encountered.
**
**	File is positioned past the magic number on the front.
**********************************************************************/
csFILE * EXP_LVL3 CS_gxopn (Const char *mode)
{
	return CS_gxFileOpen(mode);
}

/**********************************************************************
**	flag = CS_gxrd (strm,gx_def);
**
**	csFILE strm;				file/device from which the possibly enrypted
**								ellipsoid definition is to be read.
**	struct cs_GeodeticTransformDef_ *gx_rec;
**								next geodetic path definition record is returned here.
**	int flag;					returns +1 for successful read, 0 for EOF,
**								-1 for error.
**********************************************************************/
int EXP_LVL3 CS_gxrd (csFILE *strm,struct cs_GeodeticTransform_ *gx_def)
{
	char datumKeyName[cs_KEYNM_DEF] = { '\0' };

	int result = CS_gxRead(strm, gx_def);
	if (result <= 0)
		return result;

	CS_gxsep (gx_def);

	CS_stncp (datumKeyName, gx_def->srcDatum, sizeof (datumKeyName));
	if (CS_nampp (datumKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}

	CS_stncp (datumKeyName,gx_def->trgDatum,sizeof (datumKeyName));
	if (CS_nampp (datumKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}

	return result;
}
/**********************************************************************
**	st = CS_gxwr (strm,gx_rec);
**
**	csFILE *strm;				file/device to which the possibly enrypted
**								ellipsoid definition is to be written.
**	struct cs_GeodeticTransformDef_ *gx_rec;
**								the geodetic path definition record which is
**								to be written.
**	int st;						returns FALSE if write was completed successfully,
**								else returns TRUE.
**********************************************************************/
int EXP_LVL3 CS_gxwr (csFILE *strm,Const struct cs_GeodeticTransform_ *gx_def)
{
	return CS_gxWrite(strm, gx_def);
}

/**********************************************************************
**	st = CS_gxdel (gp_def);
**
**	struct cs_GeodeticTransform_ *gp_def;
**								a pointer to the geodetic path definition
**								structure which is to be deleted from the
**								Geodetic Path Dictionary.
**	int st;						returns a zero if the delete was successfully
**								completed, else returns -1.
**********************************************************************/
int EXP_LVL3 CS_gxdel (struct cs_GeodeticTransform_ *gx_def)
{
	int result = CS_gxDelete(gx_def);

	if (0 == result)
	{
		/* Looks like we've deleted a Geodetic Transformation definition.
		   We delete any existiung Geodetic Transformation Index so that the
		   next time it is needed, it will be regenerated and the result will
		   not have this entry in it any longer. */
		CS_releaseGxIndex ();
	}

	return result;
}


int CS_gxGridOvrly(struct cs_GeodeticTransform_ *gx_target, Const struct cs_GeodeticTransform_ *gx_source)
{
	__ALIGNMENT__1
	struct csGeodeticXformParmsGridFiles_ updatedGridFiles;

	//keep this definition write protected; if it's not a transformation using grid files
	if (cs_DTCMTH_GFILE != gx_source->methodCode)
		return 1; //we didn't do any overlaying here

	//we just update the potentially updated grid files information; everything else we
	//take from the dictionary definition
	updatedGridFiles = gx_target->parameters.fileParameters;
	*gx_target = *gx_source;
	gx_target->parameters.fileParameters = updatedGridFiles;

	return 0; //we've done some overlaying here
}

char Const* CS_gxkey(Const struct cs_GeodeticTransform_ *gx_def)
{
	if (NULL == gx_def)
	{
		CS_erpt(cs_INV_ARG1);
		return NULL;
	}

	return gx_def->xfrmName;
}

int CS_gxwrtchk(struct cs_GeodeticTransform_ *gx_target, Const struct cs_GeodeticTransform_ *gx_source, int* isProtected)
{
	int overlayStatus;

	if (NULL == isProtected)
		return -1;

	if (FALSE == *isProtected) //nothing we'd wanted to change here; it's write enabled already
		return 0;

	overlayStatus = CS_gxGridOvrly(gx_target, gx_source);
	if (0 == overlayStatus) //gx_target has been overlaid with gx_source, i.e. the definition can be updated in the dictionaries
	{
		*isProtected = FALSE;
	}
	return 0;
}


/******************************************************************************
**	flag = CS_gxupd (gxdef,crypt);
**
**	struct cs_GeodeticTransform_ *gxdef;
**								a pointer to the geodetic transformation
**								definition structure which is to be written to
**								the Geodetic Transformation Dictionary.
**	int flag;					returns TRUE (+1) if the indicated geodetic
**								path previously existed and was simply
**								updated, returns FALSE (0) if the geodetic
**								transformation had to be added as a new
**								geodetic transformation, returns
**								-1 if the update process failed.
**
**	If the Geodetic Transformation Dictionary does not already contain an entry
**	with the indicated key name, the entry is added.
*******************************************************************************/
int EXP_LVL3 CS_gxupd (struct cs_GeodeticTransform_ *gx_def)
{
	int result = CS_gxUpdate(gx_def);

	if (result >= 0)
	{
		//looks like we've added a geodetic transformation;
		//if we did just update an existing definition, we don't have to release the index here; the code is the same anyway
		CS_releaseGxIndex();
	}

	return result;
}

/**********************************************************************
**	cmp_val = CS_gxcmp (pp,qq);
**
**	struct cs_GeodeticTransformDef_ *pp;
**	struct cs_GeodeticTransformDef_ *qq;
**								The geodetic path definition records which
**								are to be compared.
**	int cmp_val;				return is negative if pp follows qq in the
**								collating sequence, positive if pp preceeds
**								qq, and zero if they are equivalent.
**
** Compares only the three elements required to make the key value of
** each record unique.  Thus, rcrdCnt and protect are not in cluided
** in thecomparison.
**********************************************************************/
int EXP_LVL7 CS_gxcmp (Const struct cs_GeodeticTransform_ *pp,
					   Const struct cs_GeodeticTransform_ *qq)
{
	int st;

	/* OK, now we can compare the two structures.  For sorting
	   and binary search purposes, we only look at the source and
	   target key names and the record number. */

	st = CS_stricmp (pp->xfrmName,qq->xfrmName);
	return (st);
}
/******************************************************************************
**	gx_ptr = CS_gxdef (Const char* xfrmName);
**
**	char *xfrmName;				key name of the geodetic path definition
**								which is to be fetched.
**	struct cs_GeodeticTransform_ *gx_ptr;
**								returns a pointer to a malloc'ed geodetic path
**								definition structure.  Calling module owns the
**								returned structure and is responsible for
**								freeing it.
**
**	This function only fetches the geodetic path definition as recorded in
**	the geodetic path dictionary.
**********************************************************************/
struct cs_GeodeticTransform_ * EXP_LVL3 CS_gxdef (Const char *xfrmName)
{
	return CS_gxdef2(xfrmName, NULL);
}

struct cs_GeodeticTransform_* EXP_LVL3 CS_gxdef2 (Const char *xfrmName, char* pszDirPath)
{
	return CS_gxDefinition(xfrmName, pszDirPath);
}

/**********************************************************************
**	count = CS_gxdefAll (pDefArray);
**
**	cs_GeodeticTransform_ **pDefArray[];	A pointer to an array of cs_GeodeticTransform_ instances.
**	
**	This function reads all available cs_GeodeticTransform_ definitions from all available
**	dictionary files. [pDefArray] will be set to a malloc'ed array
**	of cs_GeodeticTransform_ pointers where both, i.e. the array and the pointers contained,
**	will have to be CS_free'd() by the caller. The number
**	of pointers contained in the array is returned by [count].
**	That is, count will be >= 0 in case of success and a negative value
**	otherwise in which case [pDefArray] must not be examined by the caller.
**********************************************************************/
int EXP_LVL3 CS_gxdefAll (struct cs_GeodeticTransform_ **pDefArray[])
{
	return CS_gxDefinitionAll(pDefArray);
}

/**********************************************************************
**	gx_ptr = CS_gxdefEx (Const char* srcDatum,Const char* trgDatum);
**
**	char *srcDatum;				key name of the source datum of the path
**								definition which is to be fetched.
**	char *trgDatum;				key name of the target datum of the path
**								definition which is to be fetched.
**	struct cs_GeodeticTransform_ *gx_ptr;
**								returns a pointer to a malloc'ed ellipsoid
**								definition geodetic transformation
**								structure.
**********************************************************************/
/*lint -esym(550,direction)   not used in this module, retained to assist in debugging */
struct cs_GeodeticTransform_ * EXP_LVL3 CS_gxdefEx (Const char *srcDatum,
													Const char *trgDatum)
{
	extern char csErrnam [];
	extern char cs_UserDir[];
	extern int cs_Error;

	char currentDir[MAXPATH] = { '\0' };
	char targetPaths[2][MAXPATH] = { {'\0'}, {'\0'} };
	char const* pTargetPath;

	int globalFoundForward;
	int globalFoundBackward;

	
	int st;
	size_t i;
	int direction;

	long fwdFpos;
	long invFpos;

	char *cp;
	csFILE *strm;
	struct cs_GeodeticTransform_ *gx_def;

	char tmpKeyName [64];

	__ALIGNMENT__1				/* For some versions of Sun compiler. */
	struct cs_GeodeticTransform_ gx_rec;

	/* Prepare for the potential error condition. */
	strm = NULL;
	gx_def = NULL;

	globalFoundForward = 0;
	globalFoundBackward = 0;

	/* Make sure the provided names are OK. */
	CS_stncp (tmpKeyName,srcDatum,sizeof (tmpKeyName));
	st = CS_nampp (tmpKeyName);
	if (st != 0) goto error;
	CS_stncp (tmpKeyName,trgDatum,sizeof (tmpKeyName));
	st = CS_nampp (tmpKeyName);
	if (st != 0) goto error;

	st = CS_getdr(currentDir);
	if (st != 0)
	{
		goto error;
	}

	CS_stncp(targetPaths[0], cs_UserDir, sizeof(targetPaths[0]));
	CS_stncp(targetPaths[1], currentDir, sizeof(targetPaths[1]));

	//go through all directories we've
	for (i = 0; i < (sizeof(targetPaths) / sizeof(targetPaths[0])); ++i)
	{
		if (NULL != strm)
		{
			CS_fclose (strm);
			strm = NULL;
		}

		/* Search the file for the requested coordinate system definition. */
		pTargetPath = targetPaths[i];
		if ('\0' == *pTargetPath)
			continue;

		//switch [cs_dir] and [cs_DirP] to whatever the current target directory is
		if (CS_setdr(pTargetPath, NULL))
			goto error;

		/* Open up the geodetic transformation dictionary file. */
		strm = CS_gxopn (_STRM_BINRD);
		if (strm == NULL) 
		{
			cs_Error = 0;
			continue;
		}

		/* Search the file for the requested transformation definition. */
		fwdFpos = 0L;
		invFpos = 0L;
		for (;;)
		{
			st = CS_gxrd (strm,&gx_rec);
			if (st < 0)
			{
				goto error;	
			} 
			if (st == 0)
			{
				break;
			}
		
			/* See if we have a match in the forward direction. */
			if (!CS_stricmp (gx_rec.srcDatum,srcDatum) &&
				!CS_stricmp (gx_rec.trgDatum,trgDatum))
			{
				direction = cs_DTCDIR_FWD;
				if (fwdFpos == 0L && 0 == globalFoundForward)
				{
					fwdFpos = CS_ftell (strm);
					globalFoundForward = 1;
				}
				else
				{
					cp = CS_stncp (tmpKeyName,srcDatum,cs_KEYNM_DEF);
					*cp++ = ':';
					*cp++ = ':';
					CS_stncp (cp,trgDatum,cs_KEYNM_DEF);
					CS_stncp (csErrnam,tmpKeyName,MAXPATH);
					CS_erpt (cs_GEOPATH_DUP);
					goto error;
				}
			}
			/* See if we have a match in the inverse direction. */
			if (gx_rec.inverseSupported != 0 &&
				!CS_stricmp (gx_rec.srcDatum,trgDatum) &&
				!CS_stricmp (gx_rec.trgDatum,srcDatum))
			{
				direction = cs_DTCDIR_INV;
				if (invFpos == 0L && 0 == globalFoundBackward)
				{
					invFpos = CS_ftell (strm);
					globalFoundBackward = 1;
				}
				else
				{
					cp = CS_stncp (tmpKeyName,srcDatum,cs_KEYNM_DEF);
					*cp++ = ':';
					*cp++ = ':';
					CS_stncp (cp,trgDatum,cs_KEYNM_DEF);
					CS_stncp (csErrnam,tmpKeyName,MAXPATH);
					CS_erpt (cs_GEOPATH_DUP);
					goto error;
				}
			}
		}

		/* We always return the forward definition if we found one. */
		if (fwdFpos != 0L)
		{
			if (!gx_def) //we might have allocated that in the first round (inverse)
				gx_def = (struct cs_GeodeticTransform_ *)CS_malc (sizeof (struct cs_GeodeticTransform_));

			if (gx_def == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}
			memset ((void*)gx_def,0,sizeof (*gx_def));
			st = CS_fseek (strm,fwdFpos,SEEK_SET);
			if (st != 0)
			{
				CS_erpt (cs_IOERR);
				goto error;
			}
			if (!CS_gxrd (strm,gx_def))
			{
				goto error;
			}
		}
		else if (invFpos != 0L && !gx_def) //we prefer the forward paths over the backward ones
		{
			gx_def = (struct cs_GeodeticTransform_ *)CS_malc (sizeof (struct cs_GeodeticTransform_));
			if (gx_def == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}
			memset ((void*)gx_def,0,sizeof (*gx_def));
			st = CS_fseek (strm,fwdFpos,SEEK_SET);
			if (st != 0)
			{
				CS_erpt (cs_IOERR);
				goto error;
			}
			if (!CS_gxrd (strm,gx_def))
			{
				goto error;
			}
		}
		
		if (strm != NULL)			/*lint !e774  boolean always evaluates to true */
		{
			CS_fclose (strm);
			strm = NULL;
		}
	}

	if (NULL == gx_def)
	{
		cp = CS_stncp (tmpKeyName,srcDatum,cs_KEYNM_DEF);
		*cp++ = ':';
		*cp++ = ':';
		CS_stncp (cp,trgDatum,cs_KEYNM_DEF);
		CS_stncp (csErrnam,tmpKeyName,MAXPATH);
		CS_erpt (cs_GX_NOT_FND);
		goto error;
	}

	st = CS_setdr(currentDir, NULL);
	if (st != 0)
	{
		goto error;
	}

	/* Return a pointer to the malloc'ed geodetic path definition to the
	   user. */
	return (gx_def);

error:
	if (strm != NULL) CS_fclose (strm);
	if (gx_def != NULL)
	{
		CS_free (gx_def);
		gx_def = NULL;
	}

	CS_setdr(currentDir, NULL);			/*lint !e534  ignoring return value, can't do much here on failure */

	return (gx_def);
}
/*lint +esym(550,direction) */

/**********************************************************************
**	CS_gxfnm (new_name);
**
**	char *new_name;		the name of the geodetic path dictionary file.
**
**	This function specifies the name only.  The directory,
**	and possibly drive, are specified using CS_altdr.
**********************************************************************/
void EXP_LVL1 CS_gxfnm (Const char *new_name)
{
	extern char cs_Gxname [];

	(void)CS_stncp (cs_Gxname,new_name,cs_FNM_MAXLEN);
	return;
}
int EXP_LVL5 CS_gxswp (struct cs_GeodeticTransform_* gx_def,int writeFlag)
{
	int swap;

	short lclMethodCode;

	lclMethodCode = gx_def->methodCode;

	/* Swap the elements which are common to all variations of this
	   definition. */
	swap = CS_bswap ((void *)gx_def,cs_BSWP_GXDEF_BASE);
	if (writeFlag == 0)
	{
		lclMethodCode = gx_def->methodCode;
	}
	
	/* If we are indeed swapping, swap the items which are variation
	   dependent. */
	if (swap)
	{
		switch (lclMethodCode & cs_DTCPRMTYP_MASK) {
		
		case cs_DTCPRMTYP_GEOCTR:
			CS_bswap (&gx_def->parameters.geocentricParameters,cs_BSWP_GXDEF_GEOCTR);
			break;
		case cs_DTCPRMTYP_GRIDINTP:
			CS_bswap (&gx_def->parameters.fileParameters,cs_BSWP_GXDEF_FILPRM);
			break;
		case cs_DTCPRMTYP_MULRG:
			CS_bswap (&gx_def->parameters.dmaMulRegParameters,cs_BSWP_GXDEF_MULREG);
			break;
		case cs_DTCPRMTYP_PWRSRS:
			CS_bswap (&gx_def->parameters.pwrSeriesParameters,cs_BSWP_GXDEF_PWRSRS);
			break;
		case cs_DTCPRMTYP_STANDALONE:
		default:
			/* No parameter specific swapping required. */
			break;
		}
	}
	return swap;
}
int EXP_LVL5 CS_gxswpRd (struct cs_GeodeticTransform_* gx_def)
{
	int rtnValue;

	rtnValue = CS_gxswp (gx_def,FALSE);
	return rtnValue;
}
int EXP_LVL5 CS_gxswpWr (struct cs_GeodeticTransform_* gx_def)
{
	int rtnValue;

	rtnValue = CS_gxswp (gx_def,TRUE);
	return rtnValue;
}
/* Normalize the path name with the current platform.  Specifically,
   switch the directory separator character to what is appropriate
   for the current platform. */
void EXP_LVL3 CS_gxsep (struct cs_GeodeticTransform_* gx_def)
{
	short idx;
	short pathCount;
	struct csGeodeticXfromParmsFile_* fileParmsPtr;

	if (gx_def->methodCode == cs_DTCMTH_GFILE)
	{
		pathCount = gx_def->parameters.fileParameters.fileReferenceCount;
		for (idx = 0;idx < pathCount;idx += 1)
		{
			fileParmsPtr = &gx_def->parameters.fileParameters.fileNames [idx];
			CSrplDirSep (fileParmsPtr->fileName);
		}
	}
}
