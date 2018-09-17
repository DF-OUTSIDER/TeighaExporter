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

/*lint -e766 */			/* bogus lint message, says cs_map.h is not referenced */

#include "cs_map.h"

#if defined (__MFC__)

#include "cs_mfc.h"
#include "cs_hlp.h"

static char csHelpFile [260] = "";
static int csHlpIsAvail = 0;

extern "C" char cs_Dir [];
extern "C" char *cs_DirP;

/**********************************************************************
**	status = CSwinhlp (wHndl,context);
**
**	HWND wHndl;					handle to the window which is requesting
**								help. Passed as first argument to WinHelp ().
**	int context;				context index for the specific item for which
**								help is requested.
**********************************************************************/

int EXP_LVL1 CS_setHelpPath (const char *path)
{
	CS_stncp (csHelpFile,path,sizeof (csHelpFile));
	if (CS_access (path,4) != 0) return FALSE;
	csHlpIsAvail = 1;
	return TRUE;
}
int EXP_LVL1 CS_isHlpAvailable ()
{
	if (csHlpIsAvail == 0)
	{
		if (csHelpFile [0] == '\0')
		{
			if (cs_DirP != NULL)
			{
				CS_stcpy (cs_DirP,"CS-MAP.hlp");
				if (CS_access (cs_Dir,4) == 0)
				{
					csHlpIsAvail = 1;
					CS_stcpy (csHelpFile,cs_Dir);
				}
			}
			else
			{
				csHlpIsAvail = -1;
			}
		}
		else if (CS_access (csHelpFile,4) == 0)
		{
			csHlpIsAvail = 1;
		}
		else
		{
			csHlpIsAvail = -1;
		}
	}
	return (csHlpIsAvail > 0);
}
int EXP_LVL3 CSwinhlp (void* hWnd,ulong32_t context)
{
	int rtnValue;
	TCHAR tcHlpFilePath [MAXPATH];

	rtnValue = 0;
	if (CS_isHlpAvailable ())
	{
		CSt_strNCpy (tcHlpFilePath,csHelpFile,tchrCount (tcHlpFilePath));
		rtnValue = (int)::WinHelp ((HWND)hWnd,tcHlpFilePath,HELP_CONTEXT,(DWORD)context);
	}
	return rtnValue;
}
#endif
