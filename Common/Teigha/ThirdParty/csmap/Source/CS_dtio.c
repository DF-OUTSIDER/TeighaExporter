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

/**********************************************************************
**	strm = CS_dtopn (mode);
**
**	char *mode;					mode of the file open, ala stdio.h
**	csFILE *strm;				returns a file descriptor to the open file.
**
**	Will return NULL if a problem was encountered.
**
**	File is positioned past the magic number on the front.
**********************************************************************/

csFILE * EXP_LVL3 CS_dtopn (Const char *mode)
{
	return CS_dtFileOpen(mode);
}

/**********************************************************************
**	void CS_dtDictCls (csFILE *stream);
**
**	csFILE *stream;				stream to be closed
**
**	Stream is closed.
**********************************************************************/

void CS_dtDictCls (csFILE* stream)
{
	CS_fclose (stream);
}

/**********************************************************************
**    Calling Sequence:	flag = CS_dtrd (strm,dt_def,crypt);
**
**	csFILE *strm;				file/device from which the possibly
**								enrypted datum definition is to be read.
**	struct CS_Dtdef_ *dt_def;	decoded datum definition is returned here.
**	int *crypt;					returns TRUE if the entry was encrypted,
**								else returns FALSE.
**	int flag;					returns +1 for successful read, 0 for EOF,
**								-1 for error.
**********************************************************************/
int EXP_LVL3 CS_dtrd (csFILE *strm,struct cs_Dtdef_ *dt_def,int *crypt)
{
	return CS_dtRead(strm, dt_def, crypt);
}

/**********************************************************************
**	st = CS_dtwr (strm,dt_def,crypt);
**
**	csFile *strm;				file/device to which the possibly enrypted
**								datum definition is written.
**	struct cs_Dtdef_ *dt_def;	the unencrypted datum definition which is
**								to be written.
**	int crypt;					TRUE says that the definition is to be encoded
**								before writting, FALSE says no encoding.
**	int st;						returns FALSE if write was completed
**								successfully, else returns TRUE.
**
**********************************************************************/
int EXP_LVL3 CS_dtwr (csFILE *strm,Const struct cs_Dtdef_ *dt_def,int crypt)
{
	return CS_dtWrite(strm, dt_def, crypt);
}

/**********************************************************************
**	st = CS_dtdel (dtdef);
**
**	struct cs_Dtdef_ *dtdef;	a pointer to the datum definition structure
**								which is to be deleted from the Datum Dictionary.
**	int st;						returns a zero if the delete was successfully
**								completed, else returns -1.
**
**	This function will modify the key_nm element of the
**	cs_Dtdef_ structure provided, forcing all lower case
**	characters to upper case.
**********************************************************************/
int EXP_LVL3 CS_dtdel (struct cs_Dtdef_ *dtdef)
{
	extern char *cs_DtKeyNames;
	int result = CS_dtDelete(dtdef);

	if (!result)
	{
		/* Looks like we will be deleting this datum definition. cs_DtKeyNames
		is a memory array which contains all of the existing datum key names.
		Since we're going to change this, we free up the in memory list to
		force a regeneration of same next time its use is required.  This
		regeneration will not have this name in it if our addition completes
		successfully. */

		CS_free (cs_DtKeyNames);
		cs_DtKeyNames = NULL;
	}

	return result;
}

/**********************************************************************
**	flag = CS_dtupd (dtdef,crypt);
**
**	struct cs_Dtdef_ *dtdef;		a pointer to the datum structure
**									which is to be written to the Datum
**									Dictionary.
**	int crypt;						if TRUE, the datum dictionary entry
**									is encrypted
**									before it is written.
**	int flag;						returns +1 if the indicated datum
**									previously existed and was simply
**									updated, returns 0 if the datum had
**									to be added as a new datum, returns
**									-1 if the update process failed.
**
**	If the Datum Dictionary does not already contain an entry
**	with the indicated key name, the entry is added.
**
**	This function will modify the key_nm element of the
**	provided cs_Dtdef_ structure, changing all lower case
**	characters to upper case.
**********************************************************************/
int EXP_LVL3 CS_dtupd (struct cs_Dtdef_ *dtdef,int crypt)
{
	extern char *cs_DtKeyNames;
	int updateStatus = CS_dtUpdate(dtdef, crypt);
	if (updateStatus < 0)
		return updateStatus;

	/*	cs_DtKeyNames is a memory array which contains all of the existing
		datum key names.  Since we're going to change this, we free up the
		in memory list to force a regeneration of same next time its use is
		required.  This regeneration will have the new name in it if our
		addition completes successfully. */
	if (0 == updateStatus)
	{
		CS_free(cs_DtKeyNames);
		cs_DtKeyNames = NULL;
	}

	return updateStatus;
}

/**********************************************************************
**	cmp_val = CS_dtcmp (pp,qq);
**
**	struct cs_Dtdef_ *pp;
**	struct cs_Dtdef_ *qq;		The two datum definition structures which arew
**								to be compared.
**	int cmp_val;				return is negative if pp follows qq in the
**								collating sequence, positive if pp preceeds
**								qq, and zero if they are equivalent.
**********************************************************************/

int EXP_LVL7 CS_dtcmp (Const struct cs_Dtdef_ *pp,Const struct cs_Dtdef_ *qq)
{
	cs_Register unsigned char key;
	cs_Register unsigned char *cp;

	int st;

	unsigned char *cpe;

	char pp_key [24];
	char qq_key [24];

	/* If the entries are encoded, we must decode before the
	   comparision is made.  Note, this whole encryption
	   scheme is designed to decode from the front, therefore
	   we need only deal with the first 24 characters. */

	(void)memcpy (pp_key,pp->key_nm,sizeof (pp_key));
	key = (unsigned char)pp->fill [0];
	if (key != '\0')
	{
		cp = (unsigned char *)pp_key;
		cpe = cp + sizeof (pp_key);
		while (cp < cpe)
		{
			key ^= *cp;
			*cp++ = key;
		}
	}
	(void)memcpy (qq_key,qq->key_nm,sizeof (qq_key));
	key = (unsigned char)qq->fill [0];
	if (key != '\0')
	{
		cp = (unsigned char *)qq_key;
		cpe = cp + sizeof (qq_key);
		while (cp < cpe)
		{
			key ^= *cp;
			*cp++ = key;
		}
	}

	/* OK, now we can compare the two structures.  For sorting
	   and binary search purposes, we only look at the key name. */

	st = CS_stricmp (pp_key,qq_key);

	return (st);
}

/**********************************************************************
**	dt_ptr = CS_dtdef (dat_nam);
**
**	char dat_nam;				the key name of the datum to be located.
**								I.e. "NAD27 for the North American Datum
**								of 1927.
**	struct cs_Dtdef_ *dt_ptr;	returns a pointer to a malloc'ed structure.
**********************************************************************/

struct cs_Dtdef_ * EXP_LVL3 CS_dtdef (Const char *dat_nam)
{
	return CS_dtdef2 (dat_nam, NULL);
}

struct cs_Dtdef_ * EXP_LVL3 CS_dtdef2 (Const char *dat_nam, char* pszDirPath)
{
	extern char csErrnam [];

	extern double cs_DelMax;		/* 5,000.0 */
	extern double cs_RotMax;		/* 15.0    */
	extern double cs_SclMax;		/* 200.0   */

	struct cs_Dtdef_* dtptr = NULL;
	int isUsrDef = FALSE;

	dtptr = CS_dtDefinition(dat_nam, pszDirPath, &isUsrDef);
	if (NULL == dtptr || TRUE == isUsrDef)
		return dtptr;

	/* Verify that the values are not completely bogus. */
	if (fabs (dtptr->delta_X) > cs_DelMax ||
		fabs (dtptr->delta_Y) > cs_DelMax ||
		fabs (dtptr->delta_Z) > cs_DelMax ||
		fabs (dtptr->rot_X) > cs_RotMax    ||
		fabs (dtptr->rot_Y) > cs_RotMax    ||
		fabs (dtptr->rot_Z) > cs_RotMax    ||
		fabs (dtptr->bwscale) > cs_SclMax)
	{
		CS_stncp (csErrnam,dat_nam,MAXPATH);
		CS_erpt (cs_DTDEF_INV);

		goto error;
	}

	/* Return the initialized datum structure to the user. */
	return (dtptr);

error:
	if (dtptr != NULL)			/*lint !e774  boolean always evaluates to true */
	{
		CS_free (dtptr);
		dtptr = NULL;
	}

	return NULL;
}

/**********************************************************************
**	count = CS_dtdefAll (pDefArray);
**
**	cs_Dtdef_ **pDefArray[];	A pointer to an array of cs_Dtdef_ instances.
**	
**	This function reads all available cs_Dtdef_ definitions from all available
**	dictionary files. [pDefArray] will be set to a malloc'ed array
**	of cs_Dtdef_ pointers where both, i.e. the array and the pointers contained,
**	will have to be CS_free'd() by the caller. The number
**	of pointers contained in the array is returned by [count].
**	That is, count will be >= 0 in case of success and a negative value
**	otherwise in which case [pDefArray] must not be examined by the caller.
**********************************************************************/
int EXP_LVL3 CS_dtdefAll (struct cs_Dtdef_ **pDefArray[])
{
	return CS_dtDefinitionAll(pDefArray);
}

/**********************************************************************
**	CS_dtfnm (new_name);
**
**	char *new_name;				the name of the datum dictionary file.
**
**	This function specifies the name only.  The directory,
**	and possibly drive, are specified using CS_altdr.
**********************************************************************/

void EXP_LVL1 CS_dtfnm (Const char *new_name)
{
	extern char cs_Dtname [];

	(void)CS_stncp (cs_Dtname,new_name,cs_FNM_MAXLEN);
	return;
}

