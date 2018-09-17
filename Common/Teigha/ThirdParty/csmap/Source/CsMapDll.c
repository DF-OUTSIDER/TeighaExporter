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
 *       contr butors may be used to endorse or promote products derived
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

/******************************************************************************

	This module houses the Windows DllMain function typically required on
	all Windows DLL modules.  Perhaps there is something similar in the
	UNIX/Linux environment.
	
	Generally, the DllMain function is used to try to locate automatically
	the traditional DATA directory which is to be used.  Thus, the DLL is
	self initializing.  This desire is probably due to the primary reason
	for the development of this DLL module which is use in any of the
	various VBA applications available in Microsoft and other vendor
	Windows products.
	
******************************************************************************/

#include "windows.h"
#include "cs_map.h"

BSTR _stdcall GetCsErrorAsString (void);

extern char cs_Dir [MAXPATH];
extern char* cs_DirP;
extern csFILE* csDiagnostic;
extern char csErrmsg [];

extern char cs_DirsepC;
extern char cs_ExtsepC;
extern char cs_OptchrC;

extern int cs_Error;

char csDllModulePath [MAXPATH];
char csDllModuleName [MAXPATH];
char csDllModuleData [MAXPATH];
ulong32_t csDllModuleStatus;
char csDllModuleExplanation [MAXPATH];

/* We use the following static arrays to emulate the BStr object used in
   Mocrosoft Visual Basic and VBA. To avoid serious heap problems, we
   do not try to implement BStr's as common variables.  We simply
   convert our struct to wide characters, copy it into an array such
   as defined here starting the the [1] (i.e. second) element, put the
   string length in the [0] (i.e. first) element, and then return a
   pointer to the [1] (i.e. second element.
   
   This works fine as long as the calling code never tries to release the
   memory it thinks is a BStr.  Thus, all functions in this DLL module
   which return a BStr are declared to return the DStr ByRef.  Thus,
   VB and VBA will not try to relelease the memory of the static arrays
   we use.
   
   Of course, this technique means that the "reference pointer" to a
   BStr rerturned by a function in this module will remain valid only
   until a subsequent call to a module which uses the same static array.
   Not such a terrible price to pay for the simplicity this technique
   provides.  However, it will make multi-threaded development mmuch more
   difficult.
*/
static unsigned short csDllModuleExplanationBStr [256];
static unsigned short csDllModuleErrorMessage [256];

char csModuleName [] = "CsMapDll.c";

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	BOOL ok;
	int status;
	DWORD dwStatus;

	char *cp;

	char chrTemp [MAXPATH];
	
	ok = FALSE;
	status = 0;

	cp = getenv ("CS_MAP_DIAGNOSTICS");
	if (cp != NULL)
	{
		csDiagnostic = fopen (cp,"a+");
		if (csDiagnostic != 0)
		{
			setvbuf (csDiagnostic,NULL,_IONBF,0);
		}
	}

	switch (ul_reason_for_call) {

	case DLL_PROCESS_ATTACH:
		csDllModuleStatus = -1;
		memset (csDllModulePath,'\0',sizeof (csDllModulePath));
		memset (csDllModuleName,'\0',sizeof (csDllModuleName));
		memset (csDllModuleData,'\0',sizeof (csDllModuleData));
		CS_stncp  (csDllModuleExplanation,"Dictionary location is as yet undetermined.",sizeof (csDllModuleExplanation));
	
		dwStatus = GetModuleFileName ((HMODULE)hModule,csDllModulePath,sizeof (csDllModulePath));
		if (dwStatus == FALSE)
		{
			csDllModulePath [0] = L'\0';
			csDllModuleStatus = GetLastError ();
			CS_stncp  (csDllModuleExplanation,"GetModuleName function failed to produce the DLL module path.",sizeof (csDllModuleExplanation));
		}
		else
		{
			/* We seem to have gotten the name/path of the DLL module.  We
			   capture this location as a directory path specification in
			   case we need to use it; this information is often useful. */
			CS_stncp (chrTemp,csDllModulePath,sizeof (chrTemp));
			cp = strrchr (chrTemp,cs_DirsepC);
			if (cp != NULL)
			{
				/* Capture the name of the module, then isolate the the path
				   to it. */
				if (*cp != '\0')	/* Defensive, this should always be true */
				{
					CS_stncp (csDllModuleName,(cp + 1),sizeof (csDllModuleName));
				}
				*cp = '\0';
				CS_stncp (csDllModulePath,chrTemp,sizeof (csDllModulePath));

				/* Now, finally, we see if any rational sub-directory of the
				   DLL module exists and is suitable for a Dictionary
				   directory.  Note, 14 is the length of the longest
				   sub-directory name (+2) we will try to use. */
				CS_stncp (csDllModuleData,csDllModulePath,sizeof (csDllModuleData));
				if (strlen (csDllModuleData) < (sizeof (csDllModuleData) - 14))
				{
					cp = csDllModuleData + strlen (csDllModuleData);
					*cp++ = cs_DirsepC;

					CS_stncp (cp,"Dictionaries",sizeof (chrTemp));
					status = CS_altdr (chrTemp);
					if (status == 0)
					{
						CS_stncp  (csDllModuleExplanation,"CsMapDll dictionary set to subdirectory of DLL home directory (Dictionaries).",sizeof (csDllModuleExplanation));
					}
					if (status != 0)
					{
						CS_stncp (cp,"Dictionary",sizeof (chrTemp));
						status = CS_altdr (chrTemp);
						if (status == 0)
						{
							CS_stncp  (csDllModuleExplanation,"CsMapDll dictionary set to subdirectory of DLL home directory (Dictionary).",sizeof (csDllModuleExplanation));
						}
					}
					if (status != 0)
					{
						CS_stncp (cp,"DATA",sizeof (chrTemp));
						status = CS_altdr (chrTemp);
						if (status == 0)
						{
							CS_stncp  (csDllModuleExplanation,"CsMapDll dictionary set to subdirectory of DLL home directory (DATA).",sizeof (csDllModuleExplanation));
						}
					}
					if (status != 0)
					{
						CS_stncp (cp,"Data",sizeof (chrTemp));
						status = CS_altdr (chrTemp);
						if (status == 0)
						{
							CS_stncp  (csDllModuleExplanation,"CsMapDll dictionary set to subdirectory of DLL home directory (Data).",sizeof (csDllModuleExplanation));
						}
					}
				}
			}
		}

		/* If the environmental variable is set, we use it in preference over
		   all other possibilities.  Thus, the end user can override the
		   hard coded alternatives very easily.  The user can also switch
		   the dictionary directory easily for testing or other purposes. */
		status = CS_altdr (NULL);
		if (status == 0)
		{
			CS_stncp (csDllModuleData,cs_Dir,sizeof (csDllModuleData));	
			CS_stncp  (csDllModuleExplanation,"CsMapDll dictionary directory obtained from environmental variable \"CS_MAP_DIR\".",sizeof (csDllModuleExplanation));
		}
		if (csDiagnostic != 0)
		{
			fprintf (csDiagnostic,"Status = %d at line %d in %s.\n",status,__LINE__,csModuleName);
			fprintf (csDiagnostic,"csDllModuleData = %s.\n",csDllModuleData);
		}
		if (status != 0)
		{
			csDllModuleStatus = -cs_Error;
		}

		/* In all cases, we indicate that the DLL has been successfully loaded. */
		status = TRUE;
		break;

	case DLL_THREAD_ATTACH:
		/* Currently, the DLL is valid in single threaded appolications only.
		   When this changes, we may need to do something here.  We could
		   set status to FALSE here as a protection against running in a
		   multithreaded environment, but I consider that to be a bit to
		   nannyish for me. */
		status = TRUE;
		break;

	case DLL_THREAD_DETACH:
		/* Can't think of anything we would need to do here. Since we're
		   very good at doing nothing, we'll make this the default for
		   the switch. */
	default:
		status = TRUE;
		break;
	
	case DLL_PROCESS_DETACH:
		/* We will be exiting.  The OS should do all the necessary
		   clean up, but lets try to be good citizens and take care
		   of our own trash.
		   
		   We get heavy of the dianostics here as is we screw up memory in the
		   use of this DLL (which is quite likely since differeing host
		   languages are likely), the CS_recvr function mght fail.  In
		   these cases, we want an easy way to determine that this indeed
		   was the cae. */
		if (csDiagnostic != 0)
		{
			fprintf (csDiagnostic,"Status = %d at line %d.\n",status,__LINE__);
			fflush (csDiagnostic);
		}
		CS_recvr ();
		if (csDiagnostic != 0)
		{
			fprintf (csDiagnostic,"Status = %d at line %d.\n",status,__LINE__);
			fflush (csDiagnostic);
		}
		break;
	}

	ok = (status == TRUE);
	if (!ok && csDiagnostic != 0)
	{
		/* Many hosts report a DLL load failure with very obscure messages,
		   some with none at all.  We want to be able to easily detect a
		   failure to load the DLL due to a coding screw up in the DLL. */
		fprintf (csDiagnostic,"Status = %d at line %d.\n",status,__LINE__);
	}
	return ok;
}
BSTR _stdcall GetCsErrorAsString (void)
{
	BSTR rtnValue;
	char* chrPtr;

	rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
	chrPtr = (char*)(rtnValue);
	CS_stncp (chrPtr,csErrmsg,cs_ERRMSG_SIZE-1);
	return rtnValue;
}
/* The following is a VBA special version of a standard CS-MAP librray
   function.  Returning strings (i.e. null terminated character arrays)
   is a biot trick in the VBA environment. */
BSTR _stdcall CS_ftoaVB (double value,long32_t frmt)
{
	BSTR rtnValue;
	char* chrPtr;
	long32_t status;

	char conversionResult [cs_ERRMSG_SIZE];

	/* CS_ftoa returns a long indicating how the value was actually formatted.
	   This is different from the format provided by the argument only in the
	   case where the size of the buffer precludes formatting exactly the
	   way specified.  Since our buffer here is huge, ansd we are returning a
	   BSTR which can accomodate very long strings, this is not a problem.
	   Therefore, we can safely ignore the return value.  We cap[ture just in
	   case someone is debugging. */
	status = CS_ftoa (conversionResult,sizeof (conversionResult),value,frmt);
	
	/* Now we need to arrange to return the result as a string.  I suspect
	   we could have done this through a parameter, but returning a BSTR
	   is rather easy and is something I think I know how to do. */
	rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
	chrPtr = (char*)(rtnValue);
	CS_stncp (chrPtr,conversionResult,cs_ERRMSG_SIZE-1);
	return rtnValue;
}
BSTR __stdcall CS_getEllipsoidOfVB (char *csKeyName)
{
	int status;
	BSTR rtnValue;
	char* chrPtr;
	char ellipsoidName [MAXPATH];
	
	status = CS_getEllipsoidOf (csKeyName,ellipsoidName,sizeof (ellipsoidName));
	if (status == 0)
	{
		rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
		chrPtr = (char*)(rtnValue);
		CS_stncp (chrPtr,csErrmsg,cs_ERRMSG_SIZE-1);
	}	
	else
	{
		rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
		chrPtr = (char*)(rtnValue);
		*chrPtr = '\0';
	}
	return rtnValue;
}
BSTR __stdcall CS_mgrsFromLlVB (double latLng [2],int prec)
{
	int status;
	BSTR rtnValue;
	char* chrPtr;
	char mgrsResult [MAXPATH];
	
	status = CS_mgrsFromLl (mgrsResult,latLng,prec);
	if (status == 0)
	{
		rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
		chrPtr = (char*)(rtnValue);
		CS_stncp (chrPtr,csErrmsg,cs_ERRMSG_SIZE-1);
	}	
	else
	{
		rtnValue = SysAllocStringLen (L"This is a test",cs_ERRMSG_SIZE);
		chrPtr = (char*)(rtnValue);
		*chrPtr = '\0';
	}
	return rtnValue;
}	
