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

/**********************************************************************
**	count = CS_csgrp (grp_key,grp_list);
**
**	char *grp_key;				the specific group key for which a list is to be
**								returned.
**	struct cs_Csgrplst_ *grp_list;
**								a pointer to a linked list of malloc'ed cs_Csgrplst_
**								structures is returned here.
**	int count;					the number of coordinate systems in the returned list
**								is returned.  -1 is returned for an invalid group key.
**********************************************************************/

int EXP_LVL3 CS_csgrp (Const char *grp_key,struct cs_Csgrplst_ **grp_list)
{
	extern struct cs_Prjtab_ cs_Prjtab [];
	extern struct cs_Grptbl_ cs_CsGrptbl [];
	extern char csErrnam [];

	int st;
	int count;
	int crypt;

	csFILE *strm;
	struct cs_Csgrplst_ *lp;
	struct cs_Grptbl_ *tp;
	struct cs_Prjtab_ *pp;

	struct cs_Csdef_ cs_def;

	strm = NULL;
	lp = NULL;					/* initialization to keep gcc happy */
	*grp_list = NULL;
	count = 0;

	/* Verify that the group name is one that we know about. */
	for (tp = cs_CsGrptbl;tp->group [0] != '\0';tp += 1)
	{
		if (!CS_stricmp (tp->group,grp_key)) break;
	}
	if (tp->group [0] == '\0')
	{
		CS_stncp (csErrnam,grp_key,MAXPATH);
		CS_erpt (cs_CSGRP_INVKEY);
		goto error;
	}

	/* Generate the list. */
	strm = CS_csopn (_STRM_BINRD);
	if (strm == NULL) goto error;
	while ((st = CS_csrd (strm,&cs_def,&crypt)) == 1)
	{
		if (CS_stricmp (cs_def.group,grp_key)) continue;

		/* Add the new coodinate system to the list. */
		if (*grp_list == NULL)
		{
			*grp_list = CS_malc (sizeof (struct cs_Csgrplst_));
			lp = *grp_list;
		}
		else
		{
			lp->next = CS_malc (sizeof (struct cs_Csgrplst_));		/*lint !e613   possible use of null pointer (incorrect diagnostic) */
			lp = lp->next;											/*lint !e613   possible use of null pointer (incorrect diagnostic) */
		}
		if (lp == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}
		lp->next = NULL;

		/* Locate in the projection table. */
		for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
		{
			if (!CS_stricmp (pp->key_nm,cs_def.prj_knm)) break;
		}
		if (*pp->key_nm == '\0' || pp->setup == NULL)
		{
			CS_stncp (csErrnam,cs_def.prj_knm,MAXPATH);
			CS_erpt (cs_UNKWN_PROJ);
			goto error;
		}

		CS_stncp (lp->key_nm,cs_def.key_nm,sizeof (lp->key_nm));
		CS_stncp (lp->descr,cs_def.desc_nm,sizeof (lp->descr));
		CS_stncp (lp->source,cs_def.source,sizeof (lp->source));
		CS_stncp (lp->unit,cs_def.unit,sizeof (lp->unit));

		/* Units are defaultable. This is why we needed the projection
		   table look up. */
		if ((pp->flags & cs_PRJFLG_GEOGR) != 0)
		{
			CSdfltpro (cs_DFLTSW_AU,lp->unit,sizeof (lp->unit));
		}
		else
		{
			CSdfltpro (cs_DFLTSW_LU,lp->unit,sizeof (lp->unit));
		}

		/* Datums and ellipsoids are defaultable as well. */
		if (cs_def.dat_knm [0] != '\0')
		{
			CS_stcpy (lp->ref_typ,"Datum");
			CS_stncp (lp->ref_to,cs_def.dat_knm,sizeof (lp->ref_to));
			CSdfltpro (cs_DFLTSW_DT,lp->ref_to,sizeof (lp->ref_to));
		} 
		else
		{
			CS_stcpy (lp->ref_typ,"Ellipsoid");
			CS_stncp (lp->ref_to,cs_def.elp_knm,sizeof (lp->ref_to));
			CSdfltpro (cs_DFLTSW_EL,lp->ref_to,sizeof (lp->ref_to));
		}
		count += 1;
	}
	if (st != 0) goto error;
	CS_csDictCls (strm);
	return (count);

error:
	if (*grp_list != NULL)
	{
		CS_csgrpf (*grp_list);
		*grp_list = NULL;
	}
	if (strm != NULL) CS_csDictCls (strm);
	return (-1);
}

void EXP_LVL3 CS_csgrpf (struct cs_Csgrplst_ *grp_list)
{
	struct cs_Csgrplst_ *lp;

	while (grp_list != NULL)
	{
		lp = grp_list;
		grp_list = lp->next;
		CS_free (lp);
	}
	return;
}
