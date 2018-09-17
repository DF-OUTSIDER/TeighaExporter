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
**    Calling Sequence:	strm = CS_elopn (mode);
**
**	int mode;					mode of the file open, ala fcntl.h
**	csFILE *strm;				returns a file descriptor to the open file.
**
**	Will return NULL if a problem was encountered.
**
**	File is positioned past the magic number on the front.
**********************************************************************/
csFILE * EXP_LVL3 CS_elopn (Const char *mode)
{
	return CS_elFileOpen(mode);
}

/**********************************************************************
**	void CS_elDictCls (csFILE* stream);
**
**	csFILE *strm;			stream to be closed.
**
** The stream is closed.
**********************************************************************/
void CS_elDictCls (csFILE* stream)
{
	CS_fclose (stream);
}

/**********************************************************************
**	flag = CS_elrd (strm,el_def,crypt);
**
**	csFILE strm;				file/device from which the possibly enrypted
**								ellipsoid definition is to be read.
**	struct cs_Eldef_ *el_def;	decoded ellipsoid definition is returned here.
**	int *crypt;					if the entry read was encrypted, a TRUE
**								value is returned here, else this variable is
**								set to FALSE.
**	int flag;					returns +1 for successful read, 0 for EOF,
**								-1 for error.
**
**********************************************************************/
int EXP_LVL3 CS_elrd (csFILE *strm,struct cs_Eldef_ *el_def,int *crypt)
{
	return CS_elRead(strm, el_def, crypt);
}

/**********************************************************************
**	st = CS_elwr (strm,el_def,crypt);
**
**	csFILE *strm;				file/device to which the possibly enrypted
**								ellipsoid definition is to be written.
**	struct cs_Eldef_ *el_def;	the unencrypted Ellipsoid definition which is
**								to be written.
**	int crypt;					TRUE says that the definition is to be encoded
**								before writting, FALSE says no encoding.
**	int st;						returns FALSE if write was completed successfully,
**								else returns TRUE.
**
**********************************************************************/
int EXP_LVL3 CS_elwr (csFILE *strm,Const struct cs_Eldef_ *el_def,int crypt)
{
	return CS_elWrite(strm, el_def, crypt);
}

/**********************************************************************
**	st = CS_eldel (eldef);
**
**	struct cs_Eldef_ *eldef;	a pointer to the ellipsoid definition
**								structure which is to be deleted from the
**								Ellipsoid Dictionary.
**	int st;						returns a zero if the delete was successfully
**								completed, else returns -1.
**
**********************************************************************/
int EXP_LVL3 CS_eldel (struct cs_Eldef_ *eldef)
{
	extern char *cs_ElKeyNames;

	int deletionStatus = CS_elDelete(eldef);
	if (deletionStatus)
		return deletionStatus;

	/*	cs_ElKeyNames is a memory array which contains all of the existing ellipsoid key names.
		Since we're going to change this, we free up the in memory list to
		force a regeneration of same next time its use is required.  This
		regeneration will not have this name in it if our addition completes
		successfully. */
	CS_free(cs_ElKeyNames);
	cs_ElKeyNames = NULL;

	return deletionStatus;
}

/**********************************************************************
**	flag = CS_elupd (eldef,crypt);
**
**	struct cs_Eldef_ *eldef;	a pointer to the ellipsoid definition structure which
**								is to be written to the Ellipsoid Dictionary.
**	int crypt;					if TRUE, the ellipsoid entry is encrypted
**								before it is written.
**	int flag;					returns TRUE if the indicated ellipsoid previously
**								existed and was simply updated, returns FALSE if the
**								ellipsoid had to be added as a new ellipsoid, returns
**								-1 if the update process failed.
**
**	If the Ellipsoid Dictionary does not already contain an entry
**	with the indicated key name, the entry is added.
**********************************************************************/
int EXP_LVL3 CS_elupd (struct cs_Eldef_ *eldef,int crypt)
{
	extern char csErrnam [];

	extern char *cs_ElKeyNames;

	extern double cs_Zero;			/* 0.0 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_ERadMin;		/* 1.0 */
	extern double cs_ERadMax;		/* 100 million */
	extern double cs_PRadMin;		/* 0.75 */
	extern double cs_PRadMax;		/* 100 million */
	extern double cs_EccentMax;		/* 0.2 */

	double my_flat;
	double my_ecent;

	int updateStatus = -1;
 
	/* Check the definition for basic validity. */
	if (eldef->e_rad < cs_ERadMin || eldef->e_rad > cs_ERadMax ||
		eldef->p_rad < cs_PRadMin || eldef->p_rad > cs_PRadMax)
	{
		CS_stncp (csErrnam,eldef->key_nm,MAXPATH);
		CS_erpt (cs_ELDEF_INV);

		return -1;
	}

	/* Check/Calculate the flattening and/or the eccentricity.
	   If the provided values are zero or less, we assume that
	   the calling module wants us to calculate the values.

	   Note, at this point, we know that neither e_rad or p_rad
	   is zero. */
	my_flat = cs_One - (eldef->p_rad / eldef->e_rad);
	if (my_flat < 0.0 || my_flat > 0.0040)
	{
		CS_stncp (csErrnam,eldef->key_nm,MAXPATH);
		CS_erpt (cs_ELDEF_INV);

		return -1;
	}
	else if (my_flat < 1.0e-07)
	{
		eldef->p_rad = eldef->e_rad;
		eldef->flat = cs_Zero;
		eldef->ecent = cs_Zero;
	}
	else
	{
		my_ecent = sqrt (cs_Two * my_flat - (my_flat * my_flat));

		if (eldef->flat  <= 0.0) eldef->flat  = my_flat;
		if (eldef->ecent <= 0.0) eldef->ecent = my_ecent;

		if (fabs (my_flat - eldef->flat) > 1.0E-08 ||
			fabs (my_ecent - eldef->ecent) > 1.0E-08 ||
			my_ecent > cs_EccentMax)
		{
			CS_stncp (csErrnam,eldef->key_nm,MAXPATH);
			CS_erpt (cs_ELDEF_INV);
			
			return -1;
		}
	}


	updateStatus = CS_elUpdate(eldef, crypt);
	if (updateStatus < 0)
		return updateStatus;

	if (0 == updateStatus)
	{
		CS_free(cs_ElKeyNames);
		cs_ElKeyNames = NULL;
	}

	return updateStatus;
}

/**********************************************************************
**	cmp_val = CS_elcmp (pp,qq);
**
**	struct cs_Eldef_ *pp;
**	struct cs_Eldef_ *qq;		The two ellipsoid definition structures which are
**								to be compared.
**	int cmp_val;				return is negative if pp follows qq in the
**								collating sequence, positive if pp preceeds
**								qq, and zero if they are equivalent.
**********************************************************************/
int EXP_LVL7 CS_elcmp (Const struct cs_Eldef_ *pp,Const struct cs_Eldef_ *qq)
{
	cs_Register unsigned char key;
	cs_Register unsigned char *cp;

	int st;

	unsigned char *cpe;

	char pp_key [24];
	char qq_key [24];

	/* If the entries are encoded, we must decode before the
	   comparision is made. */

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
	   and binary search purposes, we only lookl at the key name. */

	st = CS_stricmp (pp_key,qq_key);

	return (st);
}

/**********************************************************************
**	el_ptr = CS_eldef (el_nam);
**
**	char *el_nam;				key name of the ellipsoid definition to be
**								fetched.
**	struct cs_Eldef_ *el_ptr;	returns a pointer to a malloc'ed ellipsoid definition
**								structure.
**
**	This function only fetches the ellipsoid definition as recorded in
**	the ellipsoid dictionary.  This is only a part of a datum definition.
**	CS_dtdef will fetch a datum definition, CS_dtloc fetches an entire
**	datum definition which includes both the datum definition and
**	the ellipsoid parameters.
**********************************************************************/

struct cs_Eldef_ * EXP_LVL3 CS_eldef (Const char *el_nam)
{
	return CS_eldef2(el_nam, NULL);
}

/**********************************************************************
**	count = CS_eldefAll (pDefArray);
**
**	cs_Eldef_ **pDefArray[];	A pointer to an array of cs_Eldef_ instances.
**	
**	This function reads all available cs_Eldef_ definitions from all available
**	dictionary files. [pDefArray] will be set to a malloc'ed array
**	of cs_Eldef_ pointers where both, i.e. the array and the pointers contained,
**	will have to be CS_free'd() by the caller. The number
**	of pointers contained in the array is returned by [count].
**	That is, count will be >= 0 in case of success and a negative value
**	otherwise in which case [pDefArray] must not be examined by the caller.
**********************************************************************/
int EXP_LVL3 CS_eldefAll (struct cs_Eldef_ **pDefArray[])
{
	return CS_elDefinitionAll(pDefArray);
}

struct cs_Eldef_ * EXP_LVL3 CS_eldef2 (Const char *el_nam, char* pszDirPath)
{
	extern char csErrnam [];

	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_ERadMin;		/* 1.0 */
	extern double cs_ERadMax;		/* 100 million */
	extern double cs_PRadMin;		/*  0.75 */
	extern double cs_PRadMax;		/* 100 million */
	extern double cs_EccentMax;		/* 0.2 */

	double my_ecent;
	double my_flat;

	struct cs_Eldef_* el_ptr = NULL;
	int isUsrDef = FALSE;

	el_ptr = CS_elDefinition(el_nam, pszDirPath, &isUsrDef);
	if (NULL == el_ptr || TRUE == isUsrDef)
		return el_ptr;

	/* Check the ellipsoid definition for valitity. */
	if (el_ptr->e_rad < cs_ERadMin || el_ptr->e_rad > cs_ERadMax ||
		el_ptr->p_rad < cs_PRadMin || el_ptr->p_rad > cs_PRadMax)
	{
		CS_stncp (csErrnam, el_nam, MAXPATH);
		CS_erpt (cs_ELDEF_INV);

		goto error;
	}

	/* e_rad and p_rad are both greater than zero. */
	my_flat = cs_One - (el_ptr->p_rad / el_ptr->e_rad);
	if (my_flat < 0.0)
	{
		CS_stncp (csErrnam, el_nam, MAXPATH);
		CS_erpt (cs_ELDEF_INV);

		goto error;
	}
	else if (my_flat < 1.0E-07)
	{
		el_ptr->p_rad = el_ptr->e_rad;
		el_ptr->flat = cs_Zero;
		el_ptr->ecent = cs_Zero;
	}
	else
	{
		my_ecent = sqrt (cs_Two * my_flat - (my_flat * my_flat));
		if (my_ecent > cs_EccentMax ||
			fabs (my_ecent - el_ptr->ecent) > 1.0E-08 ||
			fabs (my_flat  - el_ptr->flat)   > 1.0E-08)
		{
			CS_stncp (csErrnam,el_nam,MAXPATH);
			CS_erpt (cs_ELDEF_INV);

			goto error;
		}
	}

	/* Return a pointer to the malloc'ed ellipsoid definition to the
	   user. */
	return (el_ptr);

error:
	if (el_ptr != NULL)			/*lint !e774  boolean expression always evaluates to true */
	{
		CS_free (el_ptr);
		el_ptr = NULL;
	}

	return NULL;
}

/**********************************************************************
**	CS_elfnm (new_name);
**
**	char *new_name;				the name of the ellipsoid dictionary file.
**
**	This function specifies the name only.  The directory,
**	and possibly drive, are specified using CS_altdr.
**********************************************************************/

void EXP_LVL1 CS_elfnm (Const char *new_name)
{
	extern char cs_Elname [];

	(void)CS_stncp (cs_Elname,new_name,cs_FNM_MAXLEN);
	return;
}

