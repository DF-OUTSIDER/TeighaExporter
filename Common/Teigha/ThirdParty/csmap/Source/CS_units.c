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
 Hook function to support the use of user defined units table.

 To activate, set the global CS_usrUnitPtr variable to point to your
 function.  To deactivate, set CS_usrUnitPtr to NULL.
 
 CS_unitlu calls the (*CS_usrUnitPtr) function after it checks the type
 argument for validitiy, but before it does anything else.  Thus, user
 defined unit names need not adhere to any CS-MAP convention regarding
 names and user defined units can be used to override names used by
 CS-MAP.

 (*CS_usrUnitsPtr) should return:
 < 0.0 (any negative value) to indicate an error of sorts, in which case
       the error condition must have already been reported to CS_erpt.
   0.0 to indicate that normal CS_unitlu processing is to be performed.
 > 0.0 the actual value which is to be returned by CS_unitlu to its
       calling function.
**********************************************************************/
extern double (*CS_usrUnitPtr) (short type,Const char *unitName);

/**********************************************************************
**	value = CS_unitlu (type,name);
**
**	short type;					code value indicating the type of unit being
**								searched for.
**	char *name;					the name of the unit as a null terminated string.
**	double value;				returns the value associated with that specific
**								unit, else zero.
**
**	This function returns the value necessary to convert
**	various external units to standard internal units.
**	For example, the standard internal unit of length is
**	the meter.  Therefore, when called with the name "FOOT",
**	this, function will return "0.3048006..." which is the
**	value you must multiply feet by to get meters.  Using this
**	function, you can convert from any supported unit of
**	length to any other.
**
**	Formerly, the unit table supported only units of length.
**	Currently, the unit table supports any number of unit
**	types, length and angular measure being implemented.
**	The first argument to this function indicates the type
**	unit to be searched for.
**
**	Please use this function for all unit look ups.  The
**	structure of the table and the search technique is
**	subject to change as the size of the table grows.
**
**********************************************************************/

double EXP_LVL1 CS_unitlu (short type,Const char *name)
{
	extern double cs_Zero;
	extern int cs_Error;
	extern char csErrnam [];
	extern struct cs_Unittab_ cs_Unittab [];

	cs_Register struct cs_Unittab_ Huge *tp;

	Const char *ccp;
	char *cp;
	double usrValue;

	char my_name [cs_KEYNM_DEF];

	/* Check the unit type provided. */

	if (type != cs_UTYP_LEN && type != cs_UTYP_ANG)
	{
		CS_erpt (cs_INV_UTYP);
		return (-cs_Error);
	}

	/* Give the application a chance to do its own thing. */
	if (CS_usrUnitPtr != NULL)
	{
		usrValue = (*CS_usrUnitPtr)(type,name);
		if (usrValue < 0.0) return cs_Zero;
		if (usrValue > 0.0) return usrValue;
	}

	/* Get a copy of the name which I can modify.  We strip
	   leading and trailing blanks, and also any default
	   wrapping characters. */

	ccp = name;
	while (*ccp == ' ') ccp += 1;
	cp = CS_stncp (my_name,ccp,sizeof (my_name));
	if (my_name [0] == '\0') goto error;
	while (*(cp - 1) == ' ') cp -= 1;
	*cp = '\0';
	cp -= 1;

	if ((*my_name == cs_DFLT_IDNTBEG && *cp == cs_DFLT_IDNTEND) ||
		(*my_name == cs_DFLT_REPLBEG && *cp == cs_DFLT_REPLEND))
	{
		*cp = '\0';
		CS_stcpy (my_name,&my_name [1]);
	}

	/* Look for a match in the type and complete name portion
	   of the table. */

	for (tp = cs_Unittab;tp->type != cs_UTYP_END;tp++)
	{
		if (tp->type == type &&
			!CS_stricmp (my_name,tp->name))
		{
			return (tp->factor);
		}
	}

	/* If we're still here, we didn't find it.  Look for the
	   abbreviation. */

	for (tp = cs_Unittab;tp->type != cs_UTYP_END;tp++)
	{
		if (tp->type == type &&
		    !CS_stricmp (my_name,tp->abrv))
		{
			return (tp->factor);
		}
	}

	/* It's just not there.  Must have an invalid name. */

error:
	(void)strcpy (csErrnam,name);
	CS_erpt (cs_INV_UNIT);
	return (cs_Zero);
}
int EXP_LVL3 CS_unitAdd (struct cs_Unittab_ *unitPtr)
{
	extern char csErrnam [];
	extern struct cs_Unittab_ cs_Unittab [];

	int status = 0;
	struct cs_Unittab_ *tabPtr;

	/* Prepare for possible error. */
	CS_stncp (csErrnam,unitPtr->name,MAXPATH);

	/* Verify the validity of the provided structure. */
	if (unitPtr->type != cs_UTYP_LEN && unitPtr->type != cs_UTYP_ANG)
	{
		CS_erpt (cs_UADD_TYPE);
		status = -1;
	}
	else
	{
		/* Verify that a unit with this name does not alreay exist. */
		for (tabPtr = cs_Unittab;tabPtr->type != cs_UTYP_END;tabPtr++)
		{
			if (tabPtr->type == unitPtr->type && !CS_stricmp (unitPtr->name,tabPtr->name))
			{
				break;
			}
		}
		if (tabPtr->type != cs_UTYP_END)
		{
			CS_erpt (cs_UADD_DUP);
			status = -1;
		}
		else
		{
			/* Locate the first unused unit slot in the table. */
			for (tabPtr = cs_Unittab;tabPtr->type != cs_UTYP_END;tabPtr++)
			{
				if (tabPtr->type == cs_UTYP_OFF) break;
			}
			if (tabPtr->type != cs_UTYP_OFF)
			{
				CS_erpt (cs_UADD_FULL);
				status = -1;
			}
			else
			{
				/* Copy the new definition into the static table. */
				tabPtr->type = unitPtr->type;
				CS_stncp (tabPtr->name,unitPtr->name,sizeof (tabPtr->name));
				CS_stncp (tabPtr->pluralName,unitPtr->pluralName,sizeof (tabPtr->pluralName));
				CS_stncp (tabPtr->abrv,unitPtr->abrv,sizeof (tabPtr->abrv));
				tabPtr->system = unitPtr->system;
				tabPtr->factor = unitPtr->factor;
			}
		}
	}
	return status;
}
int EXP_LVL3 CS_unitDel (short type,Const char *name)
{
	extern char csErrnam [];
	extern struct cs_Unittab_ cs_Unittab [];

	int status = 0;
	struct cs_Unittab_ *tabPtr;

	/* Prepare for possible error. */
	CS_stncp (csErrnam,name,MAXPATH);

	/* Locate the name provided. */
	for (tabPtr = cs_Unittab;tabPtr->type != cs_UTYP_END;tabPtr++)
	{
		if (tabPtr->type == type && !CS_stricmp (tabPtr->name,name))
		{
			break;
		}
	}
	if (tabPtr->type == cs_UTYP_END)
	{
		CS_erpt (cs_UDEL_NONE);
		status = -1;
	}
	else
	{
		/* Mark it as available.  This, essentially, also deletes it. */
		tabPtr->type = cs_UTYP_OFF;
	}
	return status;
}
Const char* EXP_LVL3 CS_unitluByFactor (short type,double factor)
{
	extern struct cs_Unittab_ cs_Unittab [];

	double delta;
	double bestSoFar = 1.0;
	Const char* unitNamePtr = 0;
	struct cs_Unittab_ *tabPtr;

	for (tabPtr = cs_Unittab;tabPtr->type != cs_UTYP_END;tabPtr++)
	{
		if (tabPtr->type == type)
		{
			delta = fabs (tabPtr->factor - factor);
			if (delta < bestSoFar)			
			{
				unitNamePtr = tabPtr->name;
				bestSoFar = delta;
			}
		}
	}
    return unitNamePtr;
}
