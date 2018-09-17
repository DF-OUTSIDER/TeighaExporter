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

/*                       R E M A R K S 

	This module carries several stub functions which are used to
	insulate CS-MAP from system dependencies.
*/

#include "cs_map.h"

int EXP_LVL1 CS_protect (int newValue)
{
	extern short cs_Protect;

	int rtnValue;
	
	rtnValue = cs_Protect;
	cs_Protect = (short)newValue;
	return rtnValue;
}
int EXP_LVL1 CS_unique (int newValue)
{
	extern char cs_Unique;

	int rtnValue;
	
	rtnValue = cs_Unique;
	cs_Unique = (char)newValue;
	return rtnValue;
}

/* Establish definitions for all functions which the application programmer
   did not provide.  Application programmers are expected to make their
   own definitions in the "cs_ClientBeg.h" or "cs_ClientEnd.h" header
   files. */

void * EXP_LVL1 CS_malc (size_t blk_size)
{
#ifndef _csMalloc
#	define _csMalloc(_n) malloc(_n)
#endif
	return (_csMalloc (blk_size));
}

void * EXP_LVL1 CS_ralc (void *ptr,size_t new_size)
{
#ifndef _csRealloc
#	define _csRealloc(_m,_n) realloc(_m,_n)
#endif
	return (_csRealloc (ptr,new_size));
}

void * EXP_LVL1 CS_calc (size_t blk_count,size_t blk_size)
{
#ifndef _csCalloc
#	define _csCalloc(_n,_s) calloc(_n,_s)
#endif
	return (_csCalloc (blk_count,blk_size));
}

void EXP_LVL1 CS_free (void *ptr)
{
#ifndef _csFree
#	define _csFree(_m) {if (_m) {free(_m); _m = 0;} }
#endif

	void *lclPtr;

	lclPtr = ptr;
	_csFree (lclPtr);
	return;
}

/******************************************************************************
	The following function is, essentially, a case insensitive file
	open function.  It was written by a CS-MAP user who distributes
	product on a wide variety of platforms, specifically several
	different flavors of UNIX.  Thus, a case insensitive file open
	is very useful.

	NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE

	The CS-MAP authors have no way of testing this function or verifying
	its veracity.  Use this function at your own risk.
******************************************************************************/

#if _FILE_SYSTEM == _fs_UNIX
#include <dirent.h>
csFILE *CS_fopen(const char *filename, const char *mode)
{
	extern char cs_DirsepC;
	struct stat statbuf;
	csFILE *result = NULL;
	char *fixed_filename = (char*) filename;

	/* If we are attempting to read from the file, and the file passed
	 * in does not exist, look for a different file in the same directory
	 * that has the same filename with a different case.
	 */

	if ( (*mode == *_STRM_TXTRD) && stat(filename,&statbuf) )
	{
		/* Make a copy of the filename passed in, and separate it into
		 * directory and path components.
		 */

		char *last_path_sep;

		fixed_filename = (char*) malloc(strlen(filename)+1);
		strcpy(fixed_filename,filename);
		last_path_sep = strrchr(fixed_filename,cs_DirsepC);
		if ( last_path_sep )
		{
			/* Unless the directory is the current directory, we'd
			 * better make sure that at least the directory exists!
			 */
			char *filename_part = last_path_sep + 1;
			*last_path_sep = '\0';
			if ( ! stat(fixed_filename,&statbuf) )
			{
				/* Search the contents of the directory for a file which
				 * matches the one passed in, allowing for different
				 * case.
				 */
#if 1
				DIR *directory = opendir(fixed_filename);
				if ( directory )
				{
					struct dirent *entry;
					while ( (entry = readdir(directory)) != 0 )
					{
						if ( ! CS_stricmp(entry->d_name,filename_part) )
						{
							 /* We have a match!  It should be safe to assume that
							  * their lengths are the same, so we'll just copy it
							  * in-place.
							  */
							strcpy(filename_part,entry->d_name);
							break;
						}
					}
					closedir(directory);
				}
#else
				struct dirent *name_list = NULL;
				int num_matches = scandir( fixed_filename, name_list, NULL, NULL);
				if (num_matches > 0)
				{
					int ii;
					for (ii=0;ii < num_matches;ii++ )
					{
						if (!CS_stricmp (name_list[ii].d_name,filename_part))
						{
							 /* We have a match!  It should be safe to assume that
							  * their lengths are the same, so we'll just copy it
							  * in-place.
							  */
							strcpy (filename_part,name_list[ii].d_name);
							break;
						}
					}
					free (name_list);
				}
#endif
				/* Replace the directory/path separator */
				*last_path_sep = cs_DirsepC;
			}
		}
	}
	result = (csFILE*) fopen(fixed_filename,mode);
	if ( filename != fixed_filename )
	{
		free(fixed_filename);
	}
	return result;
}
#endif

#if _RUN_TIME == _rt_MOSXUNIX
/******************************************************************************
	Apple doesn't support findfirst or ftw.  So, the following function
	has been written to support use of CS-MAP on the Apple.  It should be
	noted, that the ftw functionality is no longer a part of the standard
	library.  

    Note: ftw is only used in the CS_swpal function in the CS_rlsUpdt module.
    The CS_swpal function is only used in the CStestS.c  function of the CS_Test
    (console test) module.  Since it is not used in the library portion of the product,
    It should be moved to be a function in the CStestSupport.c source  module,
    With a prototype added to the cs_Test.h module.

    Alternatively, the ‘S’ test of the CS_Test module could be removed and
    all references to CS_ftw could then be removed from the product.
	
    CS_rlsUpdt is used to update
	older dictionary files to a newer format.  Since the dictionary files
	have not changed in many years, it's use is rather limited also.

	NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE   NOTE

	The CS-MAP authors has no way of testing this function or verifying
	its veracity.  Use this function at your own risk.
*****************************************************************************/

int CS_ftw (const char *path,int (*fn)(const char*,const struct stat*,int),int depth)
{
		/* Apple doesn't include an "ftw" implementation, and the
		 * only one I could find is subject to the GNU license,
		 * so we'll go for a different implementation. [GEH 2000/11/30]
		 * 
		 * This is a bit of a farce, really; the "depth" parameter is completely
		 * ignored.  It would take very little work to make it call CS_ftw()
		 * recursively.
		 */
	extern char cs_DirsepC;
	extern char *cs_DirP;
	extern char cs_ExtsepC;

	char *cp;
	char ctemp[MAXPATH];
	DIR *directory = NULL;
	int st = 0;

	cp = CS_stcpy(ctemp,path);

	directory = opendir(ctemp);
	if ( directory )
	{
		struct dirent *entry;
		*(cp - 1) = cs_DirsepC;
		while ( entry = readdir(directory) )
		{
			struct stat stat_buf; 
			CS_stcpy(cp,entry->d_name);

			st = stat (ctemp,&stat_buf);
			if (st == 0)
			{
				if ( S_ISDIR(stat_buf.st_mode) )  /* Should really be smarter here */
				{
					st = (*fn) (ctemp,&stat_buf,FTW_D);
				}
				else
				{
					st = (*fn) (ctemp,&stat_buf,FTW_F);
				}
				if (st != 0)
				{
					break;
				}
			}
		}
		closedir(directory);
	}
	return st;
}
#endif
