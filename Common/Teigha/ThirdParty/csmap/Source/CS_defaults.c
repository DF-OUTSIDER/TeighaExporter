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
**	previous = CS_dtdflt  (dflt_dt);
**	previous = CS_eldflt  (dflt_el);
**	previous = CS_undflt  (dflt_un);
**
**	char *dflt_dt;				the name of the datum which is to become the
**								current default datum.
**	char *dflt_el;				the name of the ellipsoid which is to become the
**								current default ellipsoid.
**	char *dflt_un;				the name of the unit which is to become the
**								current default unit.
**	char *previous;				returns a pointer to a static memory array
**								which contains the previous default value.
**								NULL is returned if the provided argument
**								was invalid.
**
**
**	All functions return the NULL pointer if the provided
**	argument was invalid.
**
**	The static memory array to which a pointer is returned is
**	shared by all of these functions.  Thus, value contained
**	in the array pointed to by the return value remains valid
**	until the next call to one of these functions.
**
**	Calling the appropriate function with a NULL pointer, simply
**	returns the current value.  Calling the appropriate function
**	with a pointer to a null string disables the specific default
**	feature.
**********************************************************************/

static char previous [24];

char * EXP_LVL1 CS_dtdflt (Const char *dflt_dt)
{
	extern char csDtDflt [];

	struct cs_Dtdef_ *dtp;

	CS_stncp (previous,csDtDflt,sizeof (previous));

	if (dflt_dt == NULL)
	{
		return (previous);
	}
	if (*dflt_dt == '\0')
	{
		csDtDflt [0] = '\0';
		return (previous);
	}

	/* Verify the value provided is a valid datum name. */

	dtp = CS_dtdef (dflt_dt);
	if (dtp == NULL)
	{
		return (NULL);
	}
	CS_free (dtp);
	CS_stncp (csDtDflt,dflt_dt,cs_KEYNM_DEF);
	return (previous);
}

char * EXP_LVL1 CS_eldflt (Const char *dflt_el)

{
	extern char csElDflt [];

	struct cs_Eldef_ *elp;

	CS_stncp (previous,csElDflt,sizeof (previous));

	if (dflt_el == NULL)
	{
		return (previous);
	}
	if (*dflt_el == '\0')
	{
		csElDflt [0] = '\0';
		return (previous);
	}
	elp = CS_eldef (dflt_el);
	if (elp == NULL)
	{
		return (NULL);
	}
	CS_free (elp);
	CS_stncp (csElDflt,dflt_el,cs_KEYNM_DEF);
	return (previous);
}

char * EXP_LVL1 CS_ludflt (Const char *dflt_lu)

{
	extern char csLuDflt [];

	double unit_scl;

	CS_stncp (previous,csLuDflt,sizeof (previous));

	if (dflt_lu == NULL)
	{
		return (previous);
	}
	if (*dflt_lu == '\0')
	{
		csLuDflt [0] = '\0';
		return (previous);
	}

	unit_scl = CS_unitlu (cs_UTYP_LEN,dflt_lu);
	if (unit_scl == 0.0)
	{
		return (NULL);
	}
	CS_stncp (csLuDflt,dflt_lu,cs_KEYNM_DEF);
	return (previous);
}

char * EXP_LVL1 CS_audflt (Const char *dflt_au)

{
	extern char csAuDflt [];

	double unit_scl;

	CS_stncp (previous,csAuDflt,sizeof (previous));

	if (dflt_au == NULL)
	{
		return (previous);
	}
	if (*dflt_au == '\0')
	{
		csAuDflt [0] = '\0';
		return (previous);
	}

	unit_scl = CS_unitlu (cs_UTYP_ANG,dflt_au);
	if (unit_scl == 0.0)
	{
		return (NULL);
	}
	CS_stncp (csAuDflt,dflt_au,cs_KEYNM_DEF);
	return (previous);
}

int EXP_LVL9 CSdfltpro (int type,char *name,int size)
{
	extern char csDtDflt [];
	extern char csElDflt [];
	extern char csLuDflt [];
	extern char csAuDflt [];

	int status;

	char *cp;
	char *dflt;

	char ctemp [cs_KEYNM_DEF];

	status = FALSE;

	/* Trim leading ans trailing spaces from the name. */

	cp = name;
	while (*cp == ' ') cp += 1;
	if (*cp == '\0') return (status);

	cp = CS_stncp (ctemp,cp,sizeof (ctemp));
	while (*(cp - 1) == ' ') cp -= 1;
	*cp = '\0';
	cp -= 1;

	/* Is the name a defaultable reference. */

	if (ctemp [0] != cs_DFLT_IDNTBEG || *cp != cs_DFLT_IDNTEND)
	{
		return (status);
	}

	/* Select the appropriate default pointer. */

	switch (type) {

	case cs_DFLTSW_DT:  dflt = csDtDflt; break;
	case cs_DFLTSW_EL:  dflt = csElDflt; break;
	case cs_DFLTSW_LU:  dflt = csLuDflt; break;
	case cs_DFLTSW_AU:  dflt = csAuDflt; break;
	default:	    dflt = NULL;
	}

	if (dflt == NULL) return (status);
	if (*dflt == '\0') return (status);

	/* Here if the specific default feature is active.
	   Isolate the base name, without any default wrapping
	   characters, and replace with the replaced
	   delimiters. */

	*cp = '\0';
	CS_stncp (ctemp,(ctemp + 1),sizeof (ctemp));
	status = TRUE;
	*name = cs_DFLT_REPLBEG;
	cp = CS_stncp ((name + 1),dflt,(size - 2));
	*cp++ = cs_DFLT_REPLEND;
	*cp = '\0';
	return (status);
}
