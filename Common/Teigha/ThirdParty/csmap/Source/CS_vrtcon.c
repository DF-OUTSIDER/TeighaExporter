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

/*
	These are static global variables, represent a possible multi-threading
	problem.
*/
struct csVertconUS_* csVertconUS = NULL;
int csVertconUSCnt = 0;

/**********************************************************************
**	st = CSvrtconInit ();
**
**	int st;						returns 0 if the initialization proceeded to
**								completion successfully.  Otherwise,
**								returns -1.
**********************************************************************/
int EXP_LVL7 CSvrtconInit (void)
{
	extern char *cs_DirP;
	extern char cs_Dir [];
	extern char cs_VertconName [];

	/* See if we already have a csVertconUS object.
	   Note, we can have such even if the count is zero. */
	if (csVertconUS == NULL)
	{
		/* Nope, we best create one. */
		CS_stcpy (cs_DirP,cs_VertconName);
		csVertconUS = CSnewVertconUS (cs_Dir);
		if (csVertconUS == NULL)
		{
			goto error;
		}
		csVertconUSCnt = 0;
	}
	csVertconUSCnt += 1;
	return 0;

error:
	if (csVertconUS != NULL)
	{
		CSdeleteVertconUS (csVertconUS);
		csVertconUS = NULL;
	}
	csVertconUSCnt = 0;
	return -1;
}
/**********************************************************************
**	CSvertconCls (void);
**
**	NOTE:  This function simply releases the resources, it doesn't
**	delete them.  This is for performance reasons.  This will cause a
**	memory leak message on some debuggers.  If this is undesirable,
**	replace CSreleaseNad27ToNad83 with CSdeleteNad27ToNad83 below, and
**	set the csNad27ToNad83 pointer to NULL.
**********************************************************************/
void EXP_LVL9 CSvrtconCls (void)
{
	csVertconUSCnt -= 1;
	if (csVertconUSCnt <= 0)
	{
		/* Alternate code selected. */
		CSdeleteVertconUS (csVertconUS);
		csVertconUS = NULL;
		csVertconUSCnt = 0;

		/**************************************************************
          Performance code below
		if (csVertconUS != NULL)
		{
			CSreleaseVertconUS (csVertconUS);
		}
		csVertconUSCnt = 0;
		**************************************************************/

		/**************************************************************
		  Alternative code, commented out.
		CSdeleteVertconUS (csVertconUS);
		csVertconUS = NULL;
		csVertconUSCnt = 0;
		**************************************************************/
	}
	return;
}
/**********************************************************************
**	status = CSvrtcon29To88 (deltaHgt,ll_83);
**
**	double *deltaHgt;			the calculated change in orthometric height
**								is returned here. 
**	double ll_83 [3];			the longitude ([0]) and the latitude ([1]) of
**								the point at which the change in orthometric
**								height is to be determined.
**	int status;					returns zero if conversion took place as expected,
**								+1 if conversion failed due to out of range
**								error, -1 for all other errors.
**
**	In the event of a +1 return value, deltaHgt is set to zero.
**********************************************************************/
int EXP_LVL7 CSvrtcon29To88 (double* deltaHgt,Const double ll_83 [3])
{
	extern double cs_Zero;

	int status;
	double result;

	result = cs_Zero;
	/* Make sure we have been initialized. */
	if (csVertconUS == NULL)
	{
		CS_erpt (cs_DTC_NO_SETUP);
		status = -1;
	}
	else
	{
		/* Note, this function returns the shift in millimeters. */
		status = CScalcVertconUS (csVertconUS,&result,ll_83);
		if (status == 0) result *= 0.001;
	}
	*deltaHgt = result;
	return status;
}
