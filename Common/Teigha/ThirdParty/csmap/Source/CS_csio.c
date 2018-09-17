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
**	strm = CS_csopn (mode);
**
**	char *mode;					mode of the open, cs_map.h ala _STRM_???
**	csFILE *strm;				returns a stream pointer to the open file.
**
**	Will return NULL if a problem was encountered.
**
**	File is positioned past the magic number on the front.
**********************************************************************/
csFILE* EXP_LVL3 CS_csopn (Const char *mode)
{
	return CS_csFileOpen(mode);
}

/**********************************************************************
**	void CS_csDictCls (csFILE* stream);
**
**	csFILE *strm;			stream to be closed.
**
** The stream is closed.
**********************************************************************/
void CS_csDictCls (csFILE* stream)
{
	CS_fclose (stream);
}

/**********************************************************************
**	flag = CS_csrd (strm,cs_def,crypt);
**
**	csFILE *strm;				file/device from which the possibly
**								enrypted coordinate system definition
**								is to be read.
**	struct cs_Csdef_ *cs_def;	decoded coordinate system definition is
**								returned here.
**	int *crypt;					returns TRUE here if the entry was
**								encrypted; else returns FALSE.
**	int flag;					returns +1 for successful read, 0 for
**								EOF, -1 for error.
**
**********************************************************************/
int EXP_LVL3 CS_csrd (csFILE *strm,struct cs_Csdef_ *cs_def, int *crypt)
{
	return CS_csRead(strm, cs_def, crypt);
}

/**********************************************************************
**	st = CS_cswr (strm,cs_def,crypt);
**
**	csFILE *strm;				file/device to which the possibly
**								enrypted coordinate system definition
**								is written.
**	struct cs_Csdef_ *cs_def;	the unencrypted coordinate system
**								definition which is to be written.
**	int crypt;					TRUE says that the definition is to
**								be encoded before writting, FALSE says
**								no encoding.
**	int st;						returns FALSE if the write was
**								successfully completed, else returns
**								TRUE.
**
**********************************************************************/
int EXP_LVL3 CS_cswr (csFILE *strm, Const struct cs_Csdef_ *cs_def, int crypt)
{
	return CS_csWrite(strm, cs_def, crypt);
}

/**********************************************************************
**	st = CS_csdel (csdef);
**
**	struct cs_Csdef_ *csdef;	a pointer to the coordinate system
**								definition structure which is to be
**								deleted from the Coordinate System
**								Dictionary.
**	int st;						returns zero if the delete was completed
**								successfully, else returns -1.
**
**	This function will modify the contents of the key_nm,
**	forcing same to adhere to the Coordinate System naming
**	convention of all upper case letters.
**********************************************************************/
int EXP_LVL3 CS_csdel (struct cs_Csdef_ *csdef)
{
	extern char *cs_CsKeyNames;
	int result = CS_csDelete(csdef);

	if (!result)
	{
		/* cs_CsKeyNames is a list of coordinate system key names maintained in
		memory to facilitate the rapid validation of a coordinate system key
		name.  Once our deletion of this coordinate system has been completed,
		thatlist will not longer be valid.  Thus, we invalidate that list,
		forcing a regeneration next time it is needed. */
		CS_free (cs_CsKeyNames);
		cs_CsKeyNames = NULL;
	}

	return result;
}


/**********************************************************************
**	flag = CS_csupd (csdef,crypt);
**
**	struct cs_Csdef_ *csdef;	a pointer to the coordinate system
**								definition structure which is to be
**								written to the Coordinate System
**								Dictionary.
**	int crypt;					if TRUE, the coordinate system is
**								encrypted before being written to the
**								dictionary.
**	int flag;					returns +1 if a coordinate system of
**								the given name already existed and was
**								updated.  Returns 0 if the coordinate
**								system had to be added to the
**								dictionary.  Returns -1 if an error
**								occurred during the update.
**
**	If the Coordinate System Dictionary does not already contain
**	an entry with the indicated key name, the entry is added.
**
**	This function will modify the key_nm element of the cs_Csdef_
**	structure provided, forcing all lower case letters to upper
**	case.
**********************************************************************/

/*
	The following defines the CS_cschk options we use.
*/
#define cs_CSCHK_OPTS (cs_CSCHK_DATUM | cs_CSCHK_ELLIPS | cs_CSCHK_REPORT)

int EXP_LVL3 CS_csupd (struct cs_Csdef_ *csdef,int crypt)
{
	extern char *cs_CsKeyNames;
	extern struct cs_Prjtab_ cs_Prjtab [];	/* Projection Table */
	struct cs_Prjtab_ *pp;
	extern char csErrnam [];
	extern double cs_Two_pi;				/* 6.28..... */
	extern double cs_One;					/* 1.0 */
	extern double cs_Zero;					/* 0.0 */
	int updateStatus = -1;

	struct cs_Datum_ *dt_ptr = NULL;
	struct cs_Eldef_ *el_ptr = NULL;

	int err_list [4];
	int err_cnt;

	double e_rad;

	/* Verify that the definition is valid. */
	err_cnt = CS_cschk (csdef,cs_CSCHK_OPTS,err_list,sizeof (err_list)/sizeof (int));
	if (err_cnt != 0) goto error;

	/* CS_cschk will have verified the unit, but until we clean some
	   things up in release 8, we'll need to do some stuff with the
	   unit ourselves, and we need a pointer to the projection table
	   entry in order to do that. */
	for (pp = cs_Prjtab;pp->setup != NULL;pp += 1)
	{
		if (!CS_stricmp (csdef->prj_knm,pp->key_nm))
		{
			break;
		}
	}
	if (pp->setup == NULL)
	{
		/* This should never happen any more. */

		CS_stncp (csErrnam,csdef->prj_knm,MAXPATH);
		CS_erpt (cs_UNKWN_PROJ);
		goto error;
	}

	/* Make sure that the scale factors in the definition are
	   properly updated to reflect the unit of the coordinate
	   system. */
	if (pp->code != cs_PRJCOD_UNITY)
	{
		/* Here for normal cartesian coordinate systems.
		   Get the linear unit specification and make sure
		   that it is valid.  Unit scale is the factor
		   which is used to convert scalars, such as text
		   height, from system coordinates to meters by
		   multiplication; or from meters to the system
		   unit by division. */
		csdef->unit_scl = CS_unitlu (cs_UTYP_LEN,csdef->unit);

		/* This should never happen any more. */
		if (csdef->unit_scl == 0.0) goto error;

		/* Now recompute the scale of the definition.  This
		   scale factor converts system units to meters by
		   division; or meters to system units by multiplication.
		   This factor also includes the mapping scale factor.
		   
		   NOTE ALSO, that this no longer includes the scale
		   reduction factor.  Each individual projection which
		   requires a scale reduction factor is responsible for
		   handling it separately.  (This didn't used to be
		   the case.) */
		csdef->scale = cs_One / (csdef->unit_scl * csdef->map_scl);
	}
	else
	{
		/* Here for lat/long coordinate systems.  Again the
		   unit scale is used to convert scalars, such as text
		   height, from system units to meters by multiplication;
		   or from meters to system units by division.  This
		   depends upon the datum definition.  That is, we
		   use the length of a unit of longitude at the equator
		   as the conversion factor from degrees to meters
		   and vice versa.
		   
		   Get the angular system unit and make sure it is
		   valid. */
		csdef->unit_scl = CS_unitlu (cs_UTYP_ANG,csdef->unit);

		/* This should never happen any more. */
		if (csdef->unit_scl == 0.0) goto error;

		/* The basic system sacle factor is simply that of the
		   unit.  I.e., the amount we modify degrees to produce
		   the desired unit.  We do this calculation now. */
		csdef->scale = cs_One / csdef->unit_scl;

		/* Adjust the unit_scl by the radius of the ellipsoid
		   which is referenced.  Of course, we must access the
		   datum definition to get the equatorial radius. Since
		   we've already checked this, it should work without
		   problem. */
		e_rad = cs_Zero;
		if (csdef->dat_knm [0] != '\0')
		{
			dt_ptr = CS_dtloc (csdef->dat_knm);
			if (dt_ptr != NULL)
			{
				e_rad = dt_ptr->e_rad;
				CS_free (dt_ptr);
				dt_ptr = NULL;
			}
		}
		if (e_rad == 0.0 && csdef->elp_knm [0] != '\0')
		{
			el_ptr = CS_eldef (csdef->elp_knm);
			if (el_ptr != NULL)
			{
				e_rad = el_ptr->e_rad;
				CS_free (el_ptr);
				el_ptr = NULL;
			}
		}
		if (e_rad == 0.0)
		{
			e_rad = 6378206.4;
		}
		csdef->unit_scl *= cs_Two_pi * e_rad / 360.0;
	}

	updateStatus = CS_csUpdate(csdef, crypt);
	if (updateStatus < 0)
		return updateStatus;

	if (0 == updateStatus) //the definition got added
	{
		CS_free(cs_CsKeyNames);
		cs_CsKeyNames = NULL;
	}

	return updateStatus;

error:
	
	if (dt_ptr != NULL) CS_free (dt_ptr);	/*lint !e774 */  /* redundant, but defensively desireable */
	if (el_ptr != NULL) CS_free (el_ptr);	/*lint !e774 */  /* redundant, but defensively desireable */
	
	return (-1);
}
/**********************************************************************
**	cmp_val = CS_cscmp (pp,qq);
**
**	struct cs_Csdef_ *pp;
**	struct cs_Csdef_ *qq;		The two coordinate systems which
**								are to be compared.
**	int cmp_val;				return is negative if pp follows qq in the
**								collating sequence, positive if pp preceeds
**								qq, and zero if they are equivalent.
**********************************************************************/
int EXP_LVL7 CS_cscmp (Const struct cs_Csdef_ *pp,Const struct cs_Csdef_ *qq)
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
	st = CS_stricmp (pp_key,qq_key);
	return (st);
}

/**********************************************************************
**	cs_ptr = CS_csdef (cs_nam);
**
**	char *cs_nam;				key name of the coordinate system to be
**								set up.
**	struct cs_Csdef_ *cs_ptr;	returns a pointer to a malloc'ed and initialized
**								coordinate system definition structure; else
**								a NULL value for failure.
**********************************************************************/
struct cs_Csdef_ * EXP_LVL3 CS_csdef (Const char *cs_nam)
{
	return CS_csdef2 (cs_nam, NULL);
}

/**********************************************************************
**	cs_ptr = CS_csdef2 (cs_nam);
**
**	char *cs_nam;				key name of the coordinate system to be
**								set up.
**	struct cs_Csdef_ *cs_ptr;	returns a pointer to a malloc'ed and initialized
**								coordinate system definition structure; else
**								a NULL value for failure.
**********************************************************************/
struct cs_Csdef_ * EXP_LVL3 CS_csdef2 (Const char *cs_nam, char* pszDirPath)
{
	return CS_csDefinition(cs_nam, pszDirPath, NULL);
}

/**********************************************************************
**	count = CS_csdefAll (pDefArray);
**
**	cs_Csdef_ **pDefArray[];	A pointer to an array of cs_Csdef_ instances.
**	
**	This function reads all available cs_Csdef_ definitions from all available
**	dictionary files. [pDefArray] will be set to a malloc'ed array
**	of cs_Csdef_ pointers where both, i.e. the array and the pointers contained,
**	will have to be CS_free'd() by the caller. The number
**	of pointers contained in the array is returned by [count].
**	That is, count will be >= 0 in case of success and a negative value
**	otherwise in which case [pDefArray] must not be examined by the caller.
**********************************************************************/
int EXP_LVL3 CS_csdefAll (struct cs_Csdef_ **pDefArray[])
{
	return CS_csDefinitionAll(pDefArray);
}

/**********************************************************************
**	CS_csfnm (new_name);
**
**	char *new_name;				the name of the coordinate system dictionary file.
**
**	This function specifies the name only.  The directory,
**	and possibly drive, are specified using CS_altdr.
**********************************************************************/
void EXP_LVL1 CS_csfnm (Const char *new_name)

{
	extern char cs_Csname [];

	(void)CS_stncp (cs_Csname,new_name,cs_FNM_MAXLEN);
	return;
}

