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

/*        * * * * *  R E M A R K S  * * * * * *

	Includes lots of normal C runtime type stuff which is not supported
	by Windows CE.  At least, they say its not supported, I can't
	really believe some of these are not there.
*/
/*lint -esym(715,buffer,mode,size,stream) */      /* Unreferenced parameters */

#include "cs_map.h"
#include <ctype.h>
#include <wctype.h>
#include <sys/stat.h>
#if defined(__APPLE__)
#include <unistd.h>
#endif
#if _RUN_TIME < _rt_UNIXPCC
#	include <io.h>
#	include <float.h>
#else
#	define _stat stat
#endif

/* For some strange reason, isnan() sems to be controversial. */

#if (_RUN_TIME == _rt_MSDOTNET || _RUN_TIME == _rt_MSWIN64) || _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
int EXP_LVL3 CS_isnan (double xxx)
{
	return _isnan (xxx);
}
#else
int CS_isnan (double xxx)
{
	return isnan (xxx);
}
#endif

/* Having that as a resource, we code some very useful functions which the
   the ANSI committee dumped. */
Const char * EXP_LVL3 CS_ecvt (double value,int count,int *dec,int *sign)
{
	static char result [24];

	int ii;
	char *cp;
	char myResult [64];


	/* Deal with the 'tough' stuff first. */
	if (value >= 0.0)
	{
		*sign = 0;
	}
	else
	{
		value = -value;
		*sign = 1;
	}

	/* We need to handle zero specially, to avoid sinularity problems. */
	if (value < 9.9E-38)
	{
		for (ii = 0;ii < count;ii += 1) result [ii] = '0';
		result [ii] = '\0';
		cp = result;
		*dec = 0;
	}
	else
	{
		/* This is pretty hoeky, but it works. */
		sprintf (myResult,"%.*E",count - 1,value);
		cp = strchr (myResult,'E');
		if (cp != NULL)
		{
			*cp++ = '\0';
			*dec = atoi (cp) + 1;
			result [0] = myResult [0];
			strcpy (&result [1],&myResult [2]);					/*lint !e419 */
		}
		else
		{
			strcpy (result,"NaN");
			*dec = -99;
		}
	}
	return result;
}
int EXP_LVL6 CS_stricmp (Const char* cp1,Const char *cp2)
{
	char cc1, cc2;
	int result;

	cc1 = cc2 = '\0';
	result = 0;
	while (result == 0)
	{
		cc1 = *cp1++;
		cc2 = *cp2++;
		if (CS_isupper (cc1)) cc1 = (char)CS_tolower (cc1); 
		if (CS_isupper (cc2)) cc2 = (char)CS_tolower (cc2);
		result = cc1 - cc2;
		if (cc1 == '\0' || cc2 == '\0') break;
	}
	if (cc1 != '\0' || cc2 != '\0')
	{
		result = cc1 - cc2;
	}
	return (result);
}
int EXP_LVL3 CS_wcsicmp (Const wchar_t* cp1,Const wchar_t *cp2)
{
	wint_t wc1, wc2;
	int result;

	wc1 = wc2 = L'\0';
	result = 0;
	while (result == 0)
	{
		wc1 = *cp1++;
		wc2 = *cp2++;
		if (iswupper (wc1)) wc1 = (wint_t)towlower (wc1); 
		if (iswupper (wc2)) wc2 = (wint_t)towlower (wc2);
		result = (int)(wc1 - wc2);
		if (wc1 == '\0' || wc2 == '\0') break;
	}
	if (wc1 != '\0' || wc2 != '\0')
	{
		result = (int)(wc1 - wc2);
	}
	return (result);
}
int EXP_LVL9 CS_access (Const char *path,int mode)
{
	/* While it is required for a most any non-trivial
	   application, there is no ANSI way to implement this
	   function.  We provide a brute force approach which is
	   ANSI compliant, but has serious undesirable side
	   effects.  An example of such a side effect is that
	   the testing to see if a file exists will disturb the
	   last access time.

	   Anyway, you can pick the implementation that works
	   for you by adjusting the comments on the code
	   below. */

#define cs_CODE_CHOICE 0
#if cs_CODE_CHOICE == 0

	/* Use this one if your run-time library supports access.  You may
	   prefer to use _access, as when using the Microsoft compiler, you
	   will not need to link "oldnames.lib" in order to get a successful
	   link. */
#	if _RUN_TIME == _rt_MSDOTNET || _RUN_TIME == _rt_MSWIN64
		return _access (path,mode);
#	else
		return access (path,mode);
#	endif

#elif cs_CODE_CHOICE == 1

	#include <sys/stat.h>

	int st;
	int rtnValue;
	struct _stat statBufr;

	rtnValue = -1;
	st = _stat (path,&statBufr);
	if (st == 0)
	{
		/* Getting here implies that the file exists. */

		switch (mode) {
		case 0:
			rtnValue = 0;
			break;
		case 2:
			if ((statBufr.st_mode & (_S_IFREG | _S_IWRITE)) == (_S_IFREG | _S_IWRITE))
			{
				rtnValue = 0;
			} 
			beak;
		case 4:
			if ((statBufr.st_mode & (_S_IFREG | _S_IREAD)) == (_S_IFREG | _S_IREAD))
			{
				rtnValue = 0;
			} 
			break;
		case 6:
			if ((statBufr.st_mode & (_S_IFREG | _S_IWRITE | _S_IREAD)) == (_S_IFREG | S_IWRITE | _S_IREAD))
			{
				rtnValue = 0;
			} 
			break;
		default:
			rtnValue = -1;
			break;
		}
	}
	return rtnValue;

#elif cs_CODE_CHOICE == 2
	/* This version is ANSI compliant, but should be avoided at all costs,
	   Not only is it slow, it will screw up last access and modified
	   times of the files being tested which is very undesirable. */
	FILE *stream;
	int rtnValue = -1;

	switch (mode) {
	case 0:
		/* Does the file exist? */
		if ((stream = fopen (path,"r")) != NULL)
		{
			fclose (stream);
			rtnValue = 0;	
		}
		break;
	case 2:
		/* Does the file exist and is it writable? */
		if ((stream = fopen (path,"a")) != NULL)
		{
			fclose (stream);
			rtnValue = 0;	
		}
		break;
	case 4:
		/* Does the file exist and is it readable? */
		if ((stream = fopen (path,"r")) != NULL)
		{
			fclose (stream);
			rtnValue = 0;	
		}
		break;
	case 6:
		/* Does the file exists, and is it readbale and writable? */
		if ((stream = fopen (path,"r+")) != NULL)
		{
			fclose (stream);
			rtnValue = 0;	
		}
		break;
	default:
		rtnValue = -1;
		break;
	}
	return rtnValue;

#elif cs_CODE_CHOICE == 4
	/* This version will work under Windows, including CE. */
	int rtnValue;
	DWORD attributes;
	wchar_t wPath [MAX_PATH];

	rtnValue = -1;
	mbstowcs (wPath,path,MAX_PATH);
	attributes = GetFileAttributes (wPath);
	if (attributes != 0xFFFFFFFF)
	{
		switch (mode) {
		case 0:
			if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				rtnValue = 0;
			}
			break;
		case 2:
			if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
				(attributes & FILE_ATTRIBUTE_READONLY ) == 0)
			{
				rtnValue = 0;
			}
			break;
		case 4:
			if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				rtnValue = 0;
			}
			break;
		case 6:
			if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0 &&
				(attributes & FILE_ATTRIBUTE_READONLY ) == 0)
			{
				rtnValue = 0;
			}
			break;
		default:
			rtnValue = -1;
			break;
		}
	}
	return rtnValue;
#else
	/* OK, we have no way to do this reasonably.  This distribution will
	   return -1 indicating that the file is not accessible.  Hopefully,
	   this will cause some sort of serious failure, maybe even with an
	   error messsage with the file name in it. */
	return 0;
#endif
}

/* The following is used to supplant the need for the stat and fstat functions
   which are not ANSI.  They are widely supported in C run-time libraries,
   so we have this function to perform the primary function of stat/fstat.

   That is, return the date/time of the last modification of a file.
   Returns zero if the file is not a regular file or does not exist. */
cs_Time_ EXP_LVL7 CS_fileModTime (Const char *filePath)
{
	int st;
	cs_Time_ rtnValue;
	struct _stat statBufr;

	rtnValue = 0;
	st = _stat (filePath,&statBufr);
	if (st == 0)
	{
		rtnValue = (cs_Time_)statBufr.st_mtime;
	}
	return rtnValue;
}
#ifdef CS_strnicmp
#	undef CS_strnicmp
#endif
int EXP_LVL9 CS_strnicmp (Const char* cp1,Const char *cp2,size_t count)
{
	char cc1, cc2;
	int result;
	int myCount;

	myCount = (int)count;
	result = 0;
	while ((myCount-- > 0) && (result == 0))
	{
		cc1 = *cp1++;
		cc2 = *cp2++;
		cc1 = (char)CS_tolower (cc1); 
		cc2 = (char)CS_tolower (cc2);
		result = cc1 - cc2;
		if (cc1 == '\0' || cc2 == '\0') break;
	}
	return (result);
}

/* This one is ANSI, but not supported by Windows CE.
   Supposedly, we're given a constant string, but we're
   suppoed to return a non-const pointer to a charcater
   in a const string.  This is ANSI? */
#ifdef CS_strrchr
#	undef CS_strrchr
#endif
char *EXP_LVL3 CS_strrchr (Const char *cPtr, int chr)
{
	int length;
	char *result = NULL;

	length = (int)strlen (cPtr) + 1;
	while (--length > 0)
	{
		if (*(cPtr + length) == chr)
		{
			result = (char *)(cPtr + length);
			break;
		}
	}
	return result;
}

/* For some reason, I thought atof was not supported in Windows CE.
   Thus, the  following.  Turns out that I was wrong, in Win CE 3.0
   anyway. */
#ifdef atof
#	undef atof
#endif
double EXP_LVL1 CS_ansiAtof (Const char *string)
{
	return atof (string);
}

#ifdef CS_getenv
#	undef CS_getenv
#endif
char * EXP_LVL9 CS_getenv (Const char *varName)
{
	return getenv (varName);
}

#ifdef CS_remove
#	undef CS_remove
#endif
int EXP_LVL9 CS_remove (Const char *path)
{
	/* A hold over from previously supported environments (WIN CE).  This
	   is no longer necessary, but is retained in lieu of changing a lot of
	   other code. */
	return remove (path);
}

/**********************************************************************
**	st = CS_rename (old,new_name);
**
**	Const char *old;			the current name of the file to be renamed.
**	Const char *new_name;		the new name to be given to the file.
**	int st;						returns zero if successful, else -1.
**
**	Isolates the differences between operating systems with
**	regard to this function.
**********************************************************************/
#ifdef CS_rename
#	undef CS_rename
#endif
int EXP_LVL9 CS_rename (Const char *old,Const char *new_name)
{
	extern char csErrnam [];

	int st;

	st = rename (old,new_name);
	if (st != 0)
	{
		(void)strcpy (csErrnam,old);
		CS_erpt (cs_RENAME);
		st = -1;
	}
	return (st);
}

#ifdef CS_strtod
#	undef CS_strtod
#endif
double EXP_LVL9 CS_strtod (Const char *ptr,char **endPtr)
{
	/* A hold over from previously supported environments (WIN CE).  This
	   is no longer necessary, but is retained in lieu of changing a lot of
	   other code. */
	return strtod (ptr,endPtr);
}

#ifdef CS_strtol
#	undef CS_strtol
#endif
long32_t EXP_LVL9 CS_strtol (Const char *ptr,char **endPtr,int base)
{
	/* A hold over from previously supported environments (WIN CE).  This
	   is no longer necessary, but is retained in lieu of changing a lot of
	   other code. */
	return (long32_t)strtol (ptr,endPtr,base);
}

#ifdef CS_strtoul
#	undef CS_strtoul
#endif
ulong32_t EXP_LVL9 CS_strtoul (Const char *ptr,char **endPtr,int base)
{
	/* A hold over from previously supported environments (WIN CE).  This
	   is no longer necessary, but is retained in lieu of changing a lot of
	   other code. */
	return (ulong32_t)strtoul (ptr,endPtr,base);
}

cs_Time_ EXP_LVL9 CS_time (cs_Time_ *ptr)
{
	cs_Time_ rtnValue;

//??// Do we need to make a 64 bit version for LINUX?
#if _RUN_TIME == _rt_MSWIN64
	time_t time64 = time ((time_t*)0);
	rtnValue = (__time32_t)(time64);
	if (ptr != 0)
	{
		*ptr = rtnValue;
	}
#else
	time_t time32 = time ((time_t*)0);
	rtnValue = time32;
	if (ptr != 0)
	{
		*ptr = rtnValue;
	}
#endif
	return rtnValue;
}
#ifdef CS_bsearch
#	undef CS_bsearch
#endif
void* CS_bsearch (Const void *key,Const void *base,size_t num,size_t width,int (*compare )(const void *elem1,const void *elem2))
{
	char  *kmin, *probe;
	size_t index;
	int cmpStatus;

	kmin = (char *) base;
	while (num > 0)
	{
		index = num >> 1;
		probe = kmin + index * width;
		cmpStatus = (*compare)(key, probe);
		if (cmpStatus > 0)
		{
			kmin = probe + width;
			num = num - index - 1;
		}
		else if (cmpStatus < 0)
		{
			num = (size_t)index;
		}
		else
		{
			return (probe);
		}
	}
	return(0);
}
/* These are not define in CE, for some reason.  What a pain.  Note that
   we do not use the other 'type' functions in CS-MAP.  I don't think
   we use all of these, but these are the easy ones. */
int EXP_LVL3 CS_isalpha (int chr)
{
	return (('A' <= chr) && (chr <= 'Z')) || (('a' <= chr) && (chr <= 'z'));
}
int EXP_LVL3 CS_isupper (int chr)
{
	return ('A' <= chr) && (chr <= 'Z');
}
int EXP_LVL3 CS_islower (int chr)
{
	return ('a' <= chr) && (chr <= 'z');
}
int EXP_LVL3 CS_isdigit (int chr)
{
	return ('0' <= chr) && (chr <= '9');
}
int EXP_LVL3 CS_isxdigit (int chr)
{
	return (('0' <= chr) && (chr <= '9')) || (('a' <= chr) && (chr <= 'f'))  || (('A' <= chr) && (chr <= 'F'));
}
int EXP_LVL3 CS_isspace (int chr)
{
	return chr == ' ';
}
int EXP_LVL3 CS_isalnum (int chr)
{
	return CS_isdigit (chr) || CS_isalpha (chr);
}
int EXP_LVL3 CS_toupper (int chr)
{
	int rtnValue = chr;

	if (CS_islower (chr)) rtnValue -= ('a' - 'A');
	return rtnValue;
}
int EXP_LVL3 CS_tolower (int chr)
{
	int rtnValue = chr;

	if (CS_isupper (chr)) rtnValue += ('a' - 'A');
	return rtnValue;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
***                                                                         ***
***  The following are rewritten functions necessary for this stuff to      ***
***  work on WIN CE platforms.  Sort of related to ANSI, but WIN CE is not  ***
***  really ANSI.                                                           ***
***                                                                         ***
*******************************************************************************
*******************************************************************************
******************************************************************************/
int CS_setvbuf (csFILE *stream,char *buffer,int mode,size_t size )
{
	return 0;
}
