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

/*lint -esym(534,CS_cswr,CS_dtwr,CS_elwr) */		/* Ignoring return value */

#include "cs_map.h"
#include "cs_Legacy.h"

/* Entire module skipped if this is an Embedded compile for project management
   convenience.  Don't think it likely that we'll need to compile dictionaries
   in the Embedded environment. */

#include <sys/types.h>
#include <sys/stat.h>
#if _RUN_TIME >= _rt_UNIXPCC
#if !defined(ANDROID) && !defined(__GNUC__)
#	include <ftw.h>
#endif
#else
#	include <io.h>
#endif

#ifndef FTW_F
#	define FTW_F 1
#endif

/* We need this ugly conditional compile here as _stat is defined in <sys/stat.h>.  If
   we include that before "cs_map.h", lots of stuff gets defined by windows headers without
   the benefit of the defines and selections made in this header.  Thus, while ugly, it is
   prettier than the alternative. */
#if _RUN_TIME != _rt_MSWIN64
	int			EXP_LVL9	CSswpatl (const char *fname,const struct _stat *stat_ptr,int parm);
#else
	int			EXP_LVL9	CSswpatl (const char *fname,const struct _stat32 *stat_ptr,int parm);
#endif

/*
	This structure is used to: 1) build a list of the files to
	be swapped, and 2) associate a temporary file name with
	each file as it is swapped.
*/

struct cs_SwpLst_
{
	struct cs_SwpLst_ *next;
	char real_nm [16];	/* File to be swapped. */
	char temp_nm [16];	/* Temporary file name. */
};
static struct cs_SwpLst_ *cs_SwpLst;

/**********************************************************************
**	status = CS_swpal ();
**
**	int status;					returns zero for successful completion, a value
**								of -1 indicates an error.
**
**	This function was designed specifically for testing use only.  It
**	should never be used in a true application environment.  It may be
**	used in a manufacturing or installation type environment.
**
**	Due to the nature of byte swapping, this function is also its
**	own inverse.
**
**	A -1 is returned for an error condition since the nature of any
**	such error will have already been reported to CS_erpt.  Should
**	a module need to know the exact nature of the problem, cs_Error
**	can be consulted.
**
**	If conditions are such that CS_bswap isn't swapping anything,
**	and this function is called, the result is a big no-op.  That is,
**	all of the files will be copied and renamed back to the original
**	name.  This behavior is NOT GUARANTEED, however.  Its just what
**	came out of the design.
**
**	The function swaps all files into a temporary copy of each file,
**	Only when all files have been swapped will it delete the originals
**	and rename the temporaries.  Thus, only if an error during the
**	rename process occurs will the system of files be inconsistent.
**	This does imply that you have sufficient disk space available,
**	which can be considerable if the Canadian National Transformation
**	data file is present.
**
**********************************************************************/

int EXP_LVL7 CS_swpal (void (*prog)(Const char *name))
{
	extern char cs_Dir [];
	extern char *cs_DirP;

	extern char cs_Csname [];
	extern char cs_Dtname [];
	extern char cs_Elname [];

	extern int cs_Error;

	int st;

	char *cp;		
	struct cs_SwpLst_ *sp, *sp1;

	char ctemp [MAXPATH];
	char tmp_name [MAXPATH];

	cs_SwpLst = NULL;
	
	/* Add the three basic files to the swap list.  Note, the
	   cs_Dir directory is assumde for all files, we deal only
	   with file names here. */
	
	cs_SwpLst = sp = CS_malc (sizeof (struct cs_SwpLst_));
	if (sp == NULL)
	{
		st = cs_NO_MEM;
		goto error;
	}
	sp->next = NULL;
	strcpy (sp->real_nm,cs_Csname);
	sp->temp_nm [0] = '\0';
	
	sp->next = CS_malc (sizeof (struct cs_SwpLst_));
	sp = sp->next;
	if (sp == NULL)
	{
		st = cs_NO_MEM;
		goto error;
	}
	sp->next = NULL;
	strcpy (sp->real_nm,cs_Dtname);
	sp->temp_nm [0] = '\0';
	
	sp->next = CS_malc (sizeof (struct cs_SwpLst_));
	sp = sp->next;
	if (sp == NULL)
	{
		st = cs_NO_MEM;
		goto error;
	}
	sp->next = NULL;
	strcpy (sp->real_nm,cs_Elname);
	sp->temp_nm [0] = '\0';
	
	/* Scan the directory for files with extensions that we
	   recognize, and add them to the list. */

	{
#if _RUN_TIME >= _rt_UNIXPCC

		*cs_DirP = '\0';
		cp = CS_stcpy (ctemp,cs_Dir);
		*(cp - 1) = '\0';
		st = CS_ftw (ctemp,CSswpatl,1);
		if (st != 0)
		{
			/* Here if something went wrong on the
		   	scan. */

			goto error;
		}

#elif _RUN_TIME == _rt_MSVC32 || _RUN_TIME == _rt_MSDOTNET || _RUN_TIME == _rt_BORLAND32

		/* NT is, conceptually, very similar to MS-DOS. The
		   function names and calling sequqences are slightly
		   different. */

		int done;
		intptr_t fnd_hdl;
		
		struct _finddata_t fnd_cb;
		struct _stat stat_buf; 
		
		*cs_DirP = '\0';
		cp = CS_stcpy (ctemp,cs_Dir);
		(void)CS_stcpy (cp,"*.*");
		fnd_hdl = _findfirst (ctemp,&fnd_cb);
		done = (fnd_hdl < 0);
		while (!done)
		{
			(void)CS_stcpy (cp,fnd_cb.name);
			st = _stat (ctemp,&stat_buf);
			if (st == 0)
			{
				st = CSswpatl (ctemp,&stat_buf,FTW_F);
				if (st != 0)
				{
					goto error;
				}
			}
			done = _findnext (fnd_hdl,&fnd_cb);
		}
#elif _RUN_TIME == _rt_MSWIN64
		int done;
		intptr_t fnd_hdl;
		
		struct _finddata_t fnd_cb;
		struct _stat32 stat_buf; 
		
		*cs_DirP = '\0';
		cp = CS_stcpy (ctemp,cs_Dir);
		(void)CS_stcpy (cp,"*.*");
		fnd_hdl = _findfirst (ctemp,&fnd_cb);
		done = (fnd_hdl < 0);
		while (!done)
		{
			(void)CS_stcpy (cp,fnd_cb.name);
			st = _stat32 (ctemp,&stat_buf);
			if (st == 0)
			{
				st = CSswpatl (ctemp,&stat_buf,FTW_F);
				if (st != 0)
				{
					goto error;
				}
			}
			done = _findnext (fnd_hdl,&fnd_cb);
		}
#else
		/* Under MS_DOS, we need to use the findfirst and
		   findnext functions as one cannot open a directory
		   file directly. */

		int done;
		struct find_t fnd_cb;
		struct _stat stat_buf;

		*cs_DirP = '\0';
		cp = CS_stcpy (ctemp,cs_Dir);
		(void)CS_stcpy (cp,"*.*");
		done = _dos_findfirst (ctemp,_A_NORMAL,&fnd_cb);
		while (!done)
		{
			(void)CS_stcpy (cp,fnd_cb.name);
			st = stat (ctemp,&stat_buf);
			if (st == 0)
			{
				st = CSswpatl (ctemp,&stat_buf,FTW_F);
				if (st != 0)
				{
					goto error;
				}
			}
			done = _dos_findnext (&fnd_cb);
		}
#endif
	}

	/* Swap all the files on the list into temporary files.  If any
	   of the Canadian guys are there, it will take some disk space. */

	st = 0;
	for (sp = cs_SwpLst,st = 0;
		 sp != NULL && st == 0;
		 sp = sp->next)
	{
		if (prog != NULL) (*prog)(sp->real_nm);
		cp = CS_swpfl (sp->real_nm);
		if (cp == NULL)
		{
			/* Oppps!!! "Something went wrong Charlie!"
			   We're supposed to return the CS-MAP error
			   code, so don't screw with st after this. */
			   
			st = cs_Error;
			break;
		}
		else
		{
			/* Save the temporary file name of the swapped
			   version of this file. */
			   
			strcpy (sp->temp_nm,cp);
		}
	}
	   
	/* If all of the swaps were successful, remove/rename to
	   cause all files to be swapped.  Otherwise, simply delete
	   all of the temporary files. */
	   
	for (sp = cs_SwpLst;sp != NULL;sp = sp->next)
	{
		/* Nothing to do if a temporary file didn't get
		   generated. */

		if (sp->temp_nm [0] != '\0')
		{
			/* Generate a full path to the temporary file. */
			
			*cs_DirP = '\0';
			cp = CS_stcpy (tmp_name,cs_Dir);
			strcpy (cp,sp->temp_nm);
			strcpy (cs_DirP,sp->real_nm);
			
			/* Are we renaming? or are we deleting? */
			
			if (st == 0)
			{
				remove (cs_Dir);					/*lint !e534   ignoring return value */

				/* We ignore errors at this point.
				   What would we do anyway??? */

				CS_rename (tmp_name,cs_Dir);		/*lint !e534   ignoring return value */
			}
			else
			{
				remove (tmp_name);					/*lint !e534   ignoring return value */
			}
		}
	}
error:
	/* Clean up in peparation for return to calling module. */
	
	sp = cs_SwpLst;
	while (sp != NULL)
	{
		sp1 = sp;
		sp = sp->next;
		CS_free (sp1);
	}

	return (st); 
}

/* This function adds a file to the "swap list" which is being built if the
   file is a cnadidate for byte swapping.  It returns FALSE if the directory
   scan is to continue, else TRUE if the directory scan should be terminated
   for some reason. (SWap - Add To List) */
#if _RUN_TIME != _rt_MSWIN64
	int EXP_LVL9 CSswpatl (const char *fname,const struct _stat *stat_ptr,int parm)
#else
	int	EXP_LVL9 CSswpatl (const char *fname,const struct _stat32 *stat_ptr,int parm)
#endif
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern struct cs_SwpLst_ *cs_SwpLst;

	char *cp;

	char my_path [MAXPATH];

	struct cs_SwpLst_ *sp;

	/* If the given file is not an ordinary file, we ignore the entry. */
	if (parm != FTW_F ||
		(stat_ptr != NULL && (stat_ptr->st_mode & S_IFMT) != S_IFREG)
	   )
	{
		/* Returning FALSE says continue the directory scan. */
		return (FALSE);
	}

	/* Verify that this file came from the specific directory we were told to
	   search.  This is necessary only for UNIX systems as the DOS
	   findfirst/findnext functions do not recurse sub-directories.  But we
	   do it anyway to check this code. */
	CS_stcpy (my_path,fname);

	/* Isolate the directory portion of the path name. */
	cp = strrchr (my_path,cs_DirsepC);

	/* If cp is NULL at this point, there is no directory separator, therefore
	   we must still be in the initial diirectory in which the search started.
	   Therefore, we should be OK.  This should never happen anyway, unless the
	   user has set cs_Dir to the null string attempting to access the current
	   working directory. */
	if (cp != NULL)
	{
		/* Terminate the directory portion of the path name we have been given.
		   Notice that we place this null immediately after the directory
		   termination character so that our result should match cs_Dir
		   exactly. */
		*(cp + 1) = '\0';

		/* See if we are still in the directory indicated by cs_Dir. */
		*cs_DirP = '\0';
		if (CS_stricmp (my_path,cs_Dir))
		{
			return (FALSE);
		}
	}
	CS_stcpy (my_path,fname);
	cp = strrchr (my_path,cs_ExtsepC);

	/* If there is no extension, is cannot be a file of interest to us. */
	if (cp == NULL)
	{
		return (FALSE);
	}
	cp += 1;

	/* If we have an extension that we recognize, add the file name to the
	   list. */
	if (!CS_stricmp (cp,cs_NADCON_LAS) ||
		!CS_stricmp (cp,cs_NADCON_LOS) ||
		!CS_stricmp (cp,cs_MULREG_EXT) ||
		!CS_stricmp (cp,cs_CANNT_DAC)  ||
		!CS_stricmp (cp,cs_CANNT_GSB))
	{
		if (cs_SwpLst == NULL)
		{
			cs_SwpLst = sp = CS_malc (sizeof (struct cs_SwpLst_));
		}
		else
		{
			sp = cs_SwpLst;
			while (sp->next != NULL) sp = sp->next;
			sp->next = CS_malc (sizeof (struct cs_SwpLst_));
			sp = sp->next;
		}
		if (sp == NULL) goto atl_error;
		sp->next = NULL;
		sp->temp_nm [0] = '\0';

		/* Isolate the file name. */
		cp = strrchr (my_path,cs_DirsepC);
		if (cp == NULL) cp = my_path;
		else		cp += 1;
		CS_stncp (sp->real_nm,cp,sizeof (sp->real_nm));
	}

	return (FALSE);
atl_error:
	return (cs_NO_MEM);
}
/**********************************************************************
**	new_name = CS_swpfl (org_name);
**
**	char *org_name;				name only of the file to be swapped.
**								Must reside in standard directory (cs_Dir).
**	char *new_name;				returns a pointer to a static memory array which
**								contains the temporary name assigned to the
**								newly created file; return NULL on error.
**
**	Designed primarily for use in testing.  This function uses
**	CS_bswap as is.  In testing mode, the cs_Bswap global variable
**	is often twiddled to force this function to do something on
**	an little endian system (i.e. Intel).
**
**	In a SUN environment, this function could be used in a mainline
**	to cause a file to be swapped to native big endian form.
**
**	The returned pointer is to a name only.  The file will always
**	reside in the cs_Dir directory.
**
**	How to swap the bytes is determined by the file name.  For example,
**	if the file name matches that of cs_Csname, it is swaped as a
**	coordinate system dictionary file.  If the name has a .LAS or
**	.LOS extension, it is swapped as a NADCON file.
**
**	This function requires that CS_bswap is setup to actually swap
**	stuff.  If this is not true, not only will the files not get
**	swapped, this function will probably bomb.  This function uses
**	CS_bswap to swap byte counts etc. which it needs swapped in order
**	to function properly. The assumption is, if CS_bswap isn't
**	swapping anything, why would you call this function in the
**	first place.
**
**	All string comparisons (file names, extensions, etc.) are
**	case insensitive.
**
**********************************************************************/

char * EXP_LVL7 CS_swpfl (Const char org_name [])
{
	extern char cs_Dir [];
	extern char csErrnam [];
	extern char *cs_DirP;
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char cs_Csname [];
	extern char cs_Dtname [];
	extern char cs_Elname [];
	
	static char rtn_name [32];	/* Pointer to this static array is
								   the normal return value. */

	int st;
	int ele_cnt;

	size_t rdCnt;
	size_t wrCnt;
	size_t my_size;
	size_t hdr_size;
	size_t rec_size;
	size_t res_size;					/* Residual size */

	long32_t tmp_l;

	char *cp;	
	char *bufr;
	const char* kCp;
	csFILE *inStrm;
	csFILE *outStrm;

	char in_name [MAXPATH];
	char out_name [MAXPATH];
	char hdr_frmt [64];
	char rec_frmt [256];
	char res_frmt [256];			/* Residual format */

	__ALIGNMENT__1					/* Required by some Sun compilers. */
	struct csGridHdrUS_ nad_hdr;
	__ALIGNMENT__2					/* Required by some Sun compilers. */
	struct csGridFileCa1Hdr_ cannt_hdr;
	__ALIGNMENT__3					/* Required by some Sun compilers. */
	struct csDmaMReg_ grfmreg;

	/* Prepare for an error. */
	inStrm = NULL;
	outStrm = NULL;
	bufr = NULL;
	in_name [0] = '\0';
	out_name [0] = '\0';

	/* Manufacture a full path for the input filename. */
	*cs_DirP = '\0';
	cp = CS_stcpy (in_name,cs_Dir);
	CS_stcpy (cp,org_name);
	
	/* Create a temporary name for the new file. */
	st = CS_tmpfn (out_name);
	if (st != 0) goto error;
	cp = strrchr (out_name,cs_DirsepC);
	if (cp == NULL)
	{
		CS_stncp (csErrnam,"CS_rlsUpdt:1",MAXPATH);
		st = cs_ISER;
		goto error;
	}
	strcpy (rtn_name,(cp + 1));
	
	/* Do whatever setup is required for the specific type of
	   file we have.  The code below expects hdr_size, rec_size,
	   hdr_frmt, and rec_frmt to be initialized. */
	kCp = strrchr (org_name,cs_ExtsepC);
	if (!CS_stricmp (org_name,cs_Csname))
	{
		/* Here for a coordinate system dictionary file. */
		strcpy (hdr_frmt,"l");		/* Magic Number */
		hdr_size = sizeof (cs_magic_t);
		strcpy (rec_frmt,cs_BSWP_CSDEF);
		rec_size = sizeof (struct cs_Csdef_);

		res_size = 0;
		res_frmt [0] = '\0';
	}
	else if (!CS_stricmp (org_name,cs_Dtname))
	{
		/* Here for a datum dictionary. */
		strcpy (hdr_frmt,"l");			/* Magic Number */
		hdr_size = sizeof (cs_magic_t);
		strcpy (rec_frmt,cs_BSWP_DTDEF);
		rec_size = sizeof (struct cs_Dtdef_);

		res_size = 0;
		res_frmt [0] = '\0';
	}
	else if (!CS_stricmp (org_name,cs_Elname))
	{
		/* Here for an ellipsoid dictionary. */
		strcpy (hdr_frmt,"l");			/* Magic Number */
		hdr_size = sizeof (long32_t);
		strcpy (rec_frmt,cs_BSWP_ELDEF);
		rec_size = sizeof (struct cs_Eldef_);

		res_size = 0;
		res_frmt [0] = '\0';
	}
	else if (kCp != NULL && (!CS_stricmp ((kCp + 1),cs_NADCON_LOS) ||
							 !CS_stricmp ((kCp + 1),cs_NADCON_LAS)
						   )
			)
	{
		/* Here for a NADCON file.  We have to read the header in order to
		   get the size of the individual records.  We might even have to swap
		   the ele_cnt to get the right number of records.  That is, the files
		   we are swapping may be formatted for the machine we are swapping
		   from. */
		strcpy (hdr_frmt,cs_BSWP_GridFileHdrUS);
		inStrm = CS_fopen (in_name,_STRM_BINRD);
		if (inStrm == NULL)
		{
			strcpy (csErrnam,org_name);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}
		rdCnt = CS_fread (&nad_hdr,1,sizeof (nad_hdr),inStrm);
		if (rdCnt != sizeof (nad_hdr))
		{
			CS_erpt (CS_ferror (inStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_fclose (inStrm);
		inStrm = NULL;

		/* We assume that an outrageous value for ele_cnt means
		   we are swapping to the current native format from a
		   foreign format. */
		if (nad_hdr.ele_cnt < 0L || nad_hdr.ele_cnt > 0x8000L)
		{
			CS_bswap (&nad_hdr,hdr_frmt);
		}
		sprintf (rec_frmt,"l%df",(int)nad_hdr.ele_cnt);
		hdr_size = rec_size = ((size_t)nad_hdr.ele_cnt * sizeof (float)) + sizeof (long32_t);
		res_size = 0;
		res_frmt [0] = '\0';
	}
	else if (kCp != NULL && !CS_stricmp ((kCp + 1),cs_MULREG_EXT))
	{
		/* Here for a Multiple Regression file.  This is different from all
		   the others.  We just read it in, swap it, and write it back out.
		   The following turns off the general conversion used below for all
		   other files. */
		hdr_frmt [0] = rec_frmt [0] = res_frmt [0] = '\0';
		hdr_size = rec_size = res_size = 0;

		/* Read in the file. */
		inStrm = CS_fopen (in_name,_STRM_BINRD);
		if (inStrm == NULL)
		{
			strcpy (csErrnam,org_name);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}
		rdCnt = CS_fread (&grfmreg,1,sizeof (grfmreg),inStrm);

		/* See if this file is in the right byte order for our current machine.
		   If not, we need to swap the file size element of the structure. */
		tmp_l = grfmreg.mr_size;
		if (tmp_l < 0L || tmp_l > 0x8000L)
		{
			CS_bswap (&tmp_l,"l");
		}
		my_size = (size_t)tmp_l;
		if (rdCnt != my_size)
		{
			CS_erpt (CS_ferror (inStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_fclose (inStrm);
		inStrm = NULL;

		/* Swap the data and write it back out. */
		CS_bswap (&grfmreg,cs_BSWP_DMAMREG);
		outStrm = CS_fopen (out_name,_STRM_BINWR);
		if (outStrm == NULL)
		{
			CS_erpt (cs_TMP_CRT);
			goto error;
		}
		wrCnt = CS_fwrite (&grfmreg,1,my_size,outStrm);			/*lint !e670 */
		if (wrCnt != my_size)
		{
			CS_erpt (CS_ferror (outStrm) ? cs_IOERR : cs_DISK_FULL);
			goto error;
		}
		CS_fclose (outStrm);
		outStrm = NULL;
	}
	else if (kCp != NULL && !CS_stricmp ((kCp + 1),cs_CANNT_DAC))
	{
		/* Here for a Version 1 Canadian National Transformation file.  Very
		   similar to a NADCON file, we'll skip the comments. */
		strcpy (hdr_frmt,cs_BSWP_GridFileCa1Hdr);
		inStrm = CS_fopen (in_name,_STRM_BINRD);
		if (inStrm == NULL)
		{
			strcpy (csErrnam,org_name);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}
		rdCnt = CS_fread (&cannt_hdr,1,sizeof (cannt_hdr),inStrm);
		if (rdCnt != sizeof (cannt_hdr))
		{
			CS_erpt (CS_ferror (inStrm) ? cs_IOERR : cs_INV_FILE);
			CS_fclose (inStrm);
			inStrm = NULL;
			goto error;
		}
		CS_fclose (inStrm);
		inStrm = NULL;

		/* The Canadians use a delta of 5 minutes.  We assume the record needs
		   to be swapped if the delta is not equal to a value in this range. */
		if (cannt_hdr.del_lat <  (1.0 / 60.0) ||
			cannt_hdr.del_lat > (30.0 / 60.0))
		{
			CS_bswap (&cannt_hdr,hdr_frmt);
		}
		ele_cnt = (int)(((cannt_hdr.max_lng - cannt_hdr.min_lng) / cannt_hdr.del_lng) + 1.0E-10) + 1;

		/* Canadian files contain doubles, and don't have the record ID that US
		   files have. */
		sprintf (rec_frmt,"%dd",ele_cnt * 2);
		hdr_size = sizeof (cannt_hdr);
		rec_size = 2 * ele_cnt * sizeof (double);

		/* Canadian files have an "END" record on the end of
		   them.  We need to account for this. */
		res_size = 16;
		CS_stcpy (res_frmt,"16c");
	}
	else
	{
		strcpy (csErrnam,org_name);
		CS_erpt (cs_SWP_TYPE);
		goto error;
	}
	
	/* The remainder is a generalized swap facility given four
	   parameters:
	   	hdr_size:	size, in bytes, of any header or magic number.
		hdr_frmt:	a CS_bswap formatting string for the header.
		rec_size:	size, in bytes, of the individual records.
		rec_frmt:	a CS_bswap formatting string for the records.
		res_size:	sizeof file residual.
		res_frmt:	format of the residual.
	*/

	if (rec_size != 0 || hdr_size != 0)
	{
		inStrm = CS_fopen (in_name,_STRM_BINRD);
		if (inStrm == NULL)
		{
			strcpy (csErrnam,in_name);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}

		outStrm = CS_fopen (out_name,_STRM_BINWR);
		if (outStrm == NULL)
		{
			CS_erpt (cs_TMP_CRT);
			goto error;
		}

		/* Swap the header of the file, often simply a magic number. */
		if (hdr_size != 0)
		{
			bufr = CS_malc (hdr_size);
			if (bufr == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}
			rdCnt = CS_fread (bufr,1,hdr_size,inStrm);
			if ((int)rdCnt != hdr_size)
			{
				CS_erpt (CS_ferror (inStrm) ? cs_IOERR : cs_INV_FILE);
				goto error;
			}
			CS_bswap (bufr,hdr_frmt);
			wrCnt = CS_fwrite (bufr,1,hdr_size,outStrm);
			if ((int)wrCnt != hdr_size)
			{
				CS_erpt (CS_ferror (outStrm) ? cs_IOERR : cs_DISK_FULL);
				goto error;
			}
			CS_free (bufr);
			bufr = NULL;
		}

		/* Swap the remainder of the file. */
		if (rec_size != 0)
		{
			bufr = CS_malc (rec_size);
			if (bufr == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}
			while ((int)(rdCnt = CS_fread (bufr,1,rec_size,inStrm)) == rec_size)
			{
				CS_bswap (bufr,rec_frmt);
				wrCnt = CS_fwrite (bufr,1,rdCnt,outStrm);
				if (wrCnt != rdCnt)
				{
					CS_erpt (CS_ferror (outStrm) ? cs_IOERR : cs_DISK_FULL);
					goto error;
				}
			}
			if ((int)rdCnt == res_size)
			{
				if (rdCnt > 0)
				{
					CS_bswap (bufr,res_frmt);
					wrCnt = CS_fwrite (bufr,1,rdCnt,outStrm);
					if (wrCnt != rdCnt)
					{
						CS_erpt (CS_ferror (outStrm) ? cs_IOERR : cs_DISK_FULL);
						goto error;
					}
				}
			}
			else
			{
				if (rdCnt > 0) CS_erpt (cs_INV_FILE);
				else		   CS_erpt (cs_IOERR);
				goto error;
			}
			CS_free (bufr);
			bufr = NULL;
		}
		CS_fclose (inStrm);
		CS_fclose (outStrm);
	}
	return (rtn_name);

error:
	if (inStrm != NULL) CS_fclose (inStrm);
	if (outStrm != NULL)
	{
		CS_fclose (outStrm);
		remove (out_name);						/*lint !e534   ignoring return value */
	}
	if (bufr != NULL) CS_free (bufr);
	return (NULL);
}

/**********************************************************************
**	status = CS_csrup (char *distrb,char *bkupnm);
**
**	char *update;				name of the distribution coordinate system
**								dictionary file, i.e. the latest distribution.
**	char *bkupnm;				name which is to be given to the old file instead
**								of deleteing it.
**	int status;					returns status value, see Remarks below.
**
**	If the distribution file argument has no directory information
**	in it, i.e. no directory separators, it is looked for first
**	in the directory indicated by cs_Dir.  If not found there, the
**	current directory is searched. If directory information is
**	present in the distrb argument, the file must be there.
**
**	A new coordinate system dictionary is created in a temporary
**	file in the directory indicated by cs_Dir. Thus, a failure
**	in this function (other than the final remove and rename)
**	has no effect on existsing files.
**
**	The normal return status is zero.  CS_csrup returns a status
**	greater than zero if definitions in the original file had
**	to be renamed.  The number of rename's is returned.
**	A negative return value indicates failure, and is the negative
**	of the CS-MAP error condition which caused the failure.  CS_erpt
**	is not called by this function.
**	
**********************************************************************/

/*
	Define a handy macro to use when updating from v6 to v7
	coordinate system definitions.  This is to make the code
	more readable/editable.  The parameters are:

	prm:  a parameter to "mask", e.g. ptr07->prj_prm1
	code: numeric projection code for CS_prjprm function
	ii:    numeric index of parameter, 1-based (1-24)

	The macro sets prm to zero if CS_prjprm() indicates that
	this parameter is not used for this projection
*/

/*lint -e773    macro parameters not enclosed in parenthesis, possible erroneous expansion */
#define MASKPRM(prm,code,ii) prm = (CS_prjprm(NULL,code,ii-1)) ? prm : 0.0
/*lint +e773 */

/*
	The code in this module is written in such a manner as to
	make it very easy to add a new release level to the scheme
	of things. So, for example, at this writting, we have
	the following defines set up to essentially dummy up
	release 07 as the current release.  When we add release 8,
	we will change what we do here to dummy up release 8, which
	will automatically undummy release 7.  That's the plan
	any way. Tune in next year to see if any of this works.
*/

#define cs_Csdef07_ cs_Csdef_			/* Current release is same as 8 */
#define cs_Csdef08_ cs_Csdef_			/* Current release is 8 */
#define cs_BSWP_CSDEF07 cs_BSWP_CSDEF	/* Current release is 7 */
#define cs_BSWP_CSDEF08 cs_BSWP_CSDEF	/* Current release is 7 */
#define csdefCUR csdef08				/* Current release is 8
										   It is the csdefCUR element
										   of the u union which we
										   actually write to the
										   new Coordinate System
										   Dictionary. */

struct csCsrup_
{
	short level;
	short crypt;
	union
	{
		struct cs_Csdef05_ csdef05;
		struct cs_Csdef06_ csdef06;
		struct cs_Csdef07_ csdef07;
		struct cs_Csdef08_ csdef08;
	} u;
};

__ALIGNMENT__1					/* For some versions of Sun compiler. */
static struct csCsrup_ csCsrup;
__ALIGNMENT__2					/* For some versions of Sun compiler. */
static struct cs_Csdef_ cs_Csdef;

int CScsrupReadOld (csFILE *oldStrm,struct csCsrup_ *csrup,int old_lvl);
int CScsrupRead05 (csFILE *oldStrm,struct csCsrup_ *csrup);
int CScsrupRead06 (csFILE *oldStrm,struct csCsrup_ *csrup);
int CScsrupRead07 (csFILE *oldStrm,struct csCsrup_ *csrup);
int CScsrupRead08 (csFILE *oldStrm,struct csCsrup_ *csrup);
int CScsrupTo06 (struct csCsrup_ *csrup);
int CScsrupTo07 (struct csCsrup_ *csrup);
int CScsrupTo08 (struct csCsrup_ *csrup);

static unsigned short EXP_LVL9 prjCodeFromName (Const char *prjName);
static double EXP_LVL9 CScsrupPNT (struct cs_Csdef_ *csdef);

int EXP_LVL3 CS_csrup (Const char *distrb,Const char *bkupnm)
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_Csname [];
	extern char csErrnam [];
	extern char cs_DirsepC;

	int st;
	int cmp;
	int upd_st;
	int old_st;
	int old_lvl;
	int upd_crypt;

	size_t rdCnt;
	size_t wrCnt;

	const char* kCp;
	csFILE *oldStrm;
	csFILE *tmpStrm;
	csFILE *updStrm;

	cs_magic_t magic;

	char tmp_fnm [MAXPATH];
	char ctemp [MAXPATH];

	/* Prepare for the possibility of an error of some sort. */
	oldStrm = NULL;
	tmpStrm = NULL;
	updStrm = NULL;
	tmp_fnm [0] = '\0';
	old_lvl = 0;

	/* Open up the distribution file if there is one. */
	if (distrb != NULL && distrb [0] != '\0')
	{
		kCp = strchr (distrb,cs_DirsepC);
		if (kCp == NULL)
		{
			/* No directory information. We try cs_Dir first. */
			CS_stcpy (cs_DirP,distrb);
			updStrm = CS_fopen (cs_Dir,_STRM_BINRD);
		}
		if (updStrm == NULL)
		{
			updStrm = CS_fopen (distrb,_STRM_BINRD);
			if (updStrm == NULL)
			{
				CS_stncp (csErrnam,distrb,MAXPATH);
				CS_erpt (cs_FL_OPEN);
				goto error;
			}
		}

		/* Read the magic number. */
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),updStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (CS_ferror (updStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");
		if (magic != cs_CSDEF_MAGIC)
		{
			CS_erpt (cs_CS_BAD_MAGIC);
			goto error;
		}
	}

	/* Open up the existing file and get it's magic number so we know what we
	   are expected to do. */
	strcpy (cs_DirP,cs_Csname);
	oldStrm = CS_fopen (cs_Dir,_STRM_BINRD);
	if (oldStrm != NULL)
	{
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),oldStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");

		/* Determine the release level of the old file.  We're 100%
		   ANSI now, so we can use a switch on a long. */
		switch (magic) {
		case cs_CSDEF_MAGIC05: old_lvl = 5; break;
		case cs_CSDEF_MAGIC06: old_lvl = 6; break;
		case cs_CSDEF_MAGIC07: old_lvl = 7; break;
		case cs_CSDEF_MAGIC:   old_lvl = 8; break;
		default:
			/* It isn't a coordinate system definition file. */
			CS_erpt (cs_CS_BAD_MAGIC);
			goto error;
		}
	}

	/* Now we have the lay of the land, see what we are supposed to do. */
	if (updStrm == NULL && oldStrm ==  NULL)
	{
		/* There are no files, we can't really do anything. */
		CS_erpt (cs_CS_NOT_FND);
		goto error;
	}

	/* One of three things to do. Regardless of which, we'll need a temporary
	   file.  Create it now.  We could use CS_tmpfn, but it calls CS_erpt, and
	   we want to avoid that in this module. */
	st = CS_tmpfn (tmp_fnm);
	if (st != 0) goto error;
	tmpStrm = CS_fopen (tmp_fnm,_STRM_BINWR);
	if (tmpStrm == NULL)
	{
		CS_erpt (cs_NWCS_WRIT);
		goto error;
	}
	magic = cs_CSDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite (&magic,1,sizeof (magic),tmpStrm);
	if (wrCnt != sizeof (magic))
	{
		CS_erpt (cs_NWCS_WRIT);
		goto error;
	}

	/* OK, we have someting to do, and a temporary file to do it
	   it with. Let's get to it. */
	if (updStrm != NULL && oldStrm == NULL)
	{
		/* We have a distribution file, but no old file. A simple file copy
		   will do it. */
		while ((rdCnt = CS_fread (&cs_Csdef,1,sizeof (cs_Csdef),updStrm)) == sizeof (cs_Csdef))
		{
			wrCnt = CS_fwrite (&cs_Csdef,1,rdCnt,tmpStrm);
			if (wrCnt != rdCnt)
			{
				CS_erpt (cs_NWCS_WRIT);
				goto error;
			}
		}
		if (rdCnt != 0)
		{
			CS_erpt (CS_ferror (updStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
	}
	else if (updStrm == NULL && oldStrm != NULL)
	{
		/* We have an old file, but no update file.  We simply reformat the old
		   file. */
		for (;;)
		{
			old_st = CScsrupReadOld (oldStrm,&csCsrup,old_lvl);
			if (old_st < 0) goto error;
			if (old_st == 0) break;
			CS_cswr (tmpStrm,&csCsrup.u.csdefCUR,csCsrup.crypt);
		}
	}
	else
	{
		/* Here if we have a merge to do. Prime the pump with one record from
		   each file. */
		upd_st = CS_csrd (updStrm,&cs_Csdef,&upd_crypt);
		if (upd_st < 0) goto error;
		if (upd_st == 0) cs_Csdef.key_nm [0] = '~';
		old_st = CScsrupReadOld (oldStrm,&csCsrup,old_lvl);
		if (old_st < 0) goto error;
		if (old_st == 0) cs_Csdef.key_nm [0] = '~';

		/* Loop, writing records in sorted order. If there are
		   duplicates, we write the update record. */
		while (upd_st > 0 || old_st > 0)
		{
			if      (upd_st == 0) cmp = 1;
			else if (old_st == 0) cmp = -1;
			else 	 cmp = CS_stricmp (cs_Csdef.key_nm,csCsrup.u.csdefCUR.key_nm);
			if (cmp == 0)
			{
				/* Write the distribution record, can the
				   the old record. */
				CS_cswr (tmpStrm,&cs_Csdef,upd_crypt);
				upd_st = CS_csrd (updStrm,&cs_Csdef,&upd_crypt);
				if (upd_st < 0) goto error;
				old_st = CScsrupReadOld (oldStrm,&csCsrup,old_lvl);
				if (old_st < 0) goto error;
			}
			else if (cmp < 0)
			{
				/* Write the distribution record. */
				CS_cswr (tmpStrm,&cs_Csdef,upd_crypt);
				upd_st = CS_csrd (updStrm,&cs_Csdef,&upd_crypt);
				if (upd_st < 0) goto error;
			}
			else
			{
				/* Write the old record. */
				CS_cswr (tmpStrm,&csCsrup.u.csdefCUR,csCsrup.crypt);
				old_st = CScsrupReadOld (oldStrm,&csCsrup,old_lvl);
				if (old_st < 0) goto error;
			}
		}
	}

	/* Clean up. */
	if (tmpStrm != NULL)		/*lint !e774   boolean always evaluates to TRUE */
	{
		CS_fclose (tmpStrm);
		tmpStrm = NULL;
	}
	if (oldStrm != NULL)		/*lint !e774   boolean always evaluates to TRUE */
	{
		CS_fclose (oldStrm);
		oldStrm = NULL;
	}
	if (updStrm != NULL)		/*lint !e774   boolean always evaluates to TRUE */
	{
		CS_fclose (updStrm);
		updStrm = NULL;
	}

	if (bkupnm != NULL && *bkupnm != '\0')
	{
		CS_stcpy (cs_DirP,bkupnm);
		CS_stncp (ctemp,cs_Dir,sizeof (ctemp));
		remove (ctemp);								/*lint !e534  ignoring return value */
		CS_stcpy (cs_DirP,cs_Csname);
		CS_rename (cs_Dir,ctemp);					/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	else
	{
		CS_stcpy (cs_DirP,cs_Csname);
		remove (cs_Dir);							/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	return ((st == 0) ? 0 : -1);

error:
	if (oldStrm != NULL) CS_fclose (oldStrm);
	if (tmpStrm != NULL) CS_fclose (tmpStrm);
	if (updStrm != NULL) CS_fclose (updStrm);
	if (tmp_fnm [0] != '\0')
	{
		remove (tmp_fnm);						/*lint !e534  ignoring return value */
	}
	return -1;
}
int CScsrupReadOld (csFILE *oldStrm,struct csCsrup_ *csrup,int old_lvl)
{
	extern char csErrnam [];

	int old_st;

	switch (old_lvl) {
	case  5: old_st = CScsrupRead05 (oldStrm,csrup); break;
	case  6: old_st = CScsrupRead06 (oldStrm,csrup); break;
	case  7: old_st = CScsrupRead07 (oldStrm,csrup); break;
	case  8: old_st = CScsrupRead08 (oldStrm,csrup); break;
	default:
		CS_stncp (csErrnam,"CS_rlsUpdt:2",MAXPATH);
		CS_erpt (cs_ISER);
		old_st = -1;
		break;
	}
	if (old_st > 0)
	{
		/* Bring the record up to date. */
		if (csrup->level == 5)
		{
			if (CScsrupTo06 (csrup) != 0) old_st = -1;
		}
		if (csrup->level == 6)
		{
			if (CScsrupTo07 (csrup) != 0) old_st = -1;
		}
		if (csrup->level == 7)
		{
			if (CScsrupTo08 (csrup) != 0) old_st = -1;
		}
	}
	return (old_st);
}

/*
	The following function reads a release 5 Coordinate System
	Directory.
*/
int CScsrupRead05 (csFILE *oldStrm,struct csCsrup_ *csrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Csdef05_ *ptr;

	ptr = &csrup->u.csdef05;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);		/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		csrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		csrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_CSDEF05);

	if (ptr->key_nm [0] < 'A' || ptr->key_nm [0] > 'Z')
	{
		CS_erpt (cs_INV_FILE);
		return (-1);
	}

	csrup->level = 5;
	return (1);
}

int CScsrupRead06 (csFILE *oldStrm,struct csCsrup_ *csrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Csdef06_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &csrup->u.csdef06;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		csrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		csrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_CSDEF06);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate in subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	csrup->level = 6;
	return (1);
}

int CScsrupRead07 (csFILE *oldStrm,struct csCsrup_ *csrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Csdef07_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &csrup->u.csdef07;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;					/*lint !e668 */
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		csrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		csrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_CSDEF07);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	csrup->level = 7;
	return (1);
}

int CScsrupRead08 (csFILE *oldStrm,struct csCsrup_ *csrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Csdef08_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &csrup->u.csdef08;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;					/*lint !e668 */
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		csrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		csrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_CSDEF08);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}

	csrup->level = 8;
	return (1);
}

/*
	The following function upgrades the provided csCsrup_
	structure from release five to release six.
*/
int CScsrupTo06 (struct csCsrup_ *csrup)
{
	extern double cs_Zero;

	struct cs_Csdef05_ *ptr05;
	struct cs_Csdef06_ *ptr06;

	ptr05 = (struct cs_Csdef05_ *)CS_malc (sizeof (struct cs_Csdef05_));
	if (ptr05 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr05,&csrup->u.csdef05,sizeof (struct cs_Csdef05_));

	ptr06 = &csrup->u.csdef06;
	memset (ptr06,'\0',sizeof (struct cs_Csdef06_));

	CS_stncp (ptr06->key_nm,ptr05->key_nm,sizeof (ptr06->key_nm));
	CS_stncp (ptr06->dat_knm,ptr05->dat_knm,sizeof (ptr06->dat_knm));
	CS_stncp (ptr06->prj_knm,ptr05->prj_knm,sizeof (ptr06->prj_knm));
	CS_stncp (ptr06->group,"USER",sizeof (ptr06->group));
	CS_stncp (ptr06->unit,ptr05->unit,sizeof (ptr06->unit));

	/* We leave fill set to zero until we know why this won't work. */

	ptr06->prj_prm1 = ptr05->prj_prm1;
	ptr06->prj_prm2 = ptr05->prj_prm2;
	ptr06->prj_prm3 = ptr05->prj_prm3;
	ptr06->prj_prm4 = ptr05->prj_prm4;
	ptr06->prj_prm5 = ptr05->prj_prm5;
	ptr06->prj_prm6 = ptr05->prj_prm6;
	ptr06->prj_prm7  = cs_Zero;
	ptr06->prj_prm8  = cs_Zero;
	ptr06->prj_prm9  = cs_Zero;
	ptr06->prj_prm10 = cs_Zero;
	ptr06->prj_prm11 = cs_Zero;
	ptr06->prj_prm12 = cs_Zero;
	ptr06->prj_prm13 = cs_Zero;
	ptr06->prj_prm14 = cs_Zero;
	ptr06->prj_prm15 = cs_Zero;
	ptr06->prj_prm16 = cs_Zero;
	ptr06->prj_prm17 = cs_Zero;
	ptr06->prj_prm18 = cs_Zero;
	ptr06->prj_prm19 = cs_Zero;
	ptr06->prj_prm20 = cs_Zero;
	ptr06->prj_prm21 = cs_Zero;
	ptr06->prj_prm22 = cs_Zero;
	ptr06->prj_prm23 = cs_Zero;
	ptr06->prj_prm24 = cs_Zero;

	ptr06->org_lng  = ptr05->org_lng;
	ptr06->org_lat  = ptr05->org_lat;
	ptr06->x_off    = ptr05->x_off;
	ptr06->y_off    = ptr05->y_off;
	ptr06->scl_red  = ptr05->scl_red;
	ptr06->unit_scl = ptr05->unit_scl;
	ptr06->map_scl  = ptr05->map_scl;
	ptr06->scale    = ptr05->scale;
	ptr06->zero [0] = ptr05->zero [0];
	ptr06->zero [1] = ptr05->zero [1];

	ptr06->hgt_xx   = cs_Zero;
	ptr06->hgt_yy   = cs_Zero;
	ptr06->hgt_zz   = cs_Zero;

	ptr06->ll_min [0] = cs_Zero;
	ptr06->ll_min [1] = cs_Zero;
	ptr06->ll_max [0] = cs_Zero;
	ptr06->ll_max [1] = cs_Zero;
	ptr06->xy_min [0] = cs_Zero;
	ptr06->xy_min [1] = cs_Zero;
	ptr06->xy_max [0] = cs_Zero;
	ptr06->xy_max [1] = cs_Zero;

	ptr06->protect = ptr05->protect;
	if (ptr06->protect == 0)
	{
		ptr06->protect = (short)((CS_time ((cs_Time_ *)0) - 1728000L) / 86400L);
	}
	ptr06->quad = 0;
	ptr06->order = 0;
	CS_stncp (ptr06->desc_nm,ptr05->desc_nm,sizeof (ptr06->desc_nm));

	/* Source will be set to the null string by the memset above. */

	CS_free (ptr05);
	csrup->level = 6;
	return (0);
}

int CScsrupTo07 (struct csCsrup_ *csrup)
{
	extern double cs_Zero;

	unsigned short usPrjCode;

	struct cs_Csdef06_ *ptr06;
	struct cs_Csdef07_ *ptr07;

	double Az;

	ptr06 = (struct cs_Csdef06_ *)CS_malc (sizeof (struct cs_Csdef06_));
	if (ptr06 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr06,&csrup->u.csdef06,sizeof (struct cs_Csdef06_));

	ptr07 = &csrup->u.csdef07;
	memset (ptr07,'\0',sizeof (struct cs_Csdef07_));

	CS_stncp (ptr07->key_nm,ptr06->key_nm,sizeof (ptr07->key_nm));
	CS_stncp (ptr07->dat_knm,ptr06->dat_knm,sizeof (ptr07->dat_knm));
	CS_stncp (ptr07->elp_knm,ptr06->elp_knm,sizeof (ptr07->elp_knm));
	CS_stncp (ptr07->prj_knm,ptr06->prj_knm,sizeof (ptr07->prj_knm));
	CS_stncp (ptr07->group,ptr06->group,sizeof (ptr07->group));
	CS_stncp (ptr07->locatn,ptr06->locatn,sizeof (ptr07->locatn));
	CS_stncp (ptr07->cntry_st,ptr06->cntry_st,sizeof (ptr07->cntry_st));
	CS_stncp (ptr07->unit,ptr06->unit,sizeof (ptr07->unit));

	/* We leave fill set to zero until we know why this won't work. */

	ptr07->prj_prm1  = ptr06->prj_prm1;
	ptr07->prj_prm2  = ptr06->prj_prm2;
	ptr07->prj_prm3  = ptr06->prj_prm3;
	ptr07->prj_prm4  = ptr06->prj_prm4;
	ptr07->prj_prm5  = ptr06->prj_prm5;
	ptr07->prj_prm6  = ptr06->prj_prm6;
	ptr07->prj_prm7  = ptr06->prj_prm7;
	ptr07->prj_prm8  = ptr06->prj_prm8;
	ptr07->prj_prm9  = ptr06->prj_prm9;
	ptr07->prj_prm10 = ptr06->prj_prm10;
	ptr07->prj_prm11 = ptr06->prj_prm11;
	ptr07->prj_prm12 = ptr06->prj_prm12;
	ptr07->prj_prm13 = ptr06->prj_prm13;
	ptr07->prj_prm14 = ptr06->prj_prm14;
	ptr07->prj_prm15 = ptr06->prj_prm15;
	ptr07->prj_prm16 = ptr06->prj_prm16;
	ptr07->prj_prm17 = ptr06->prj_prm17;
	ptr07->prj_prm18 = ptr06->prj_prm18;
	ptr07->prj_prm19 = ptr06->prj_prm19;
	ptr07->prj_prm20 = ptr06->prj_prm20;
	ptr07->prj_prm21 = ptr06->prj_prm21;
	ptr07->prj_prm22 = ptr06->prj_prm22;
	ptr07->prj_prm23 = ptr06->prj_prm23;
	ptr07->prj_prm24 = ptr06->prj_prm24;

	ptr07->org_lng   = ptr06->org_lng;
	ptr07->org_lat   = ptr06->org_lat;
	ptr07->x_off     = ptr06->x_off;
	ptr07->y_off     = ptr06->y_off;
	ptr07->scl_red   = ptr06->scl_red;
	ptr07->unit_scl  = ptr06->unit_scl;
	ptr07->map_scl   = ptr06->map_scl;
	ptr07->scale     = ptr06->scale;
	ptr07->zero [XX] = ptr06->zero [XX];
	ptr07->zero [YY] = ptr06->zero [YY];

	ptr07->hgt_lng   = cs_Zero;
	ptr07->hgt_lat   = cs_Zero;
	ptr07->hgt_zz    = cs_Zero;
	ptr07->geoid_sep = cs_Zero;

		/* Certain clients have written version 6
		   definitions without zeroing the following
		   out, leaving undefined stuff in these
		   variables. Therefore, we zero them out. */

	ptr07->ll_min [LNG] = cs_Zero;
	ptr07->ll_min [LAT] = cs_Zero;
	ptr07->ll_max [LNG] = cs_Zero;
	ptr07->ll_max [LAT] = cs_Zero;
	ptr07->xy_min [XX]  = cs_Zero;
	ptr07->xy_min [YY]  = cs_Zero;
	ptr07->xy_max [XX]  = cs_Zero;
	ptr07->xy_max [YY]  = cs_Zero;

	CS_stncp (ptr07->desc_nm,ptr06->desc_nm,sizeof (ptr07->desc_nm));
	CS_stncp (ptr07->source,ptr06->source,sizeof (ptr07->source));

	ptr07->quad = 1;
	ptr07->order = 0;
	ptr07->zones = 0;
	ptr07->protect = ptr06->protect;
	if (ptr07->protect == 0)
	{
		ptr07->protect = (short)((CS_time ((cs_Time_ *)0) - 1728000L) / 86400L);
	}
	ptr07->epsg_qd = 0;
	ptr07->srid = 0;

	/* We don't need the old definition anymore. */

	CS_free (ptr06);

	/* Adjust for the fact that we removed the alternate forms of
	   the AZMED, AZMEA, and STERO projections.  There is a single
	   form for all of these projections now. */

	if (!CS_stricmp (ptr07->prj_knm,"AZEDA"))
	{
		/* AZEDA is now AZMED, also, move quad. */

		CS_stncp (ptr07->prj_knm,"AZMED",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm2;
		/* North azimuth is already in the right place. */
		ptr07->prj_prm2 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"AZEDP"))
	{
		/* AZEDP is now AZMED, also, move quad,
		   calculate the azimuth. */

		CS_stncp (ptr07->prj_knm,"AZMED",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm3 = cs_Zero;
		Az = CScsrupPNT (ptr07);
		ptr07->prj_prm1 = Az;
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm2 = cs_Zero;
		ptr07->prj_prm3 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"AZEDO"))
	{
		CS_stncp (ptr07->prj_knm,"AZMED",sizeof (ptr07->prj_knm));
		Az = CScsrupPNT (ptr07);
		ptr07->prj_prm1 = Az;
		ptr07->prj_prm2 = cs_Zero;
		ptr07->quad = 1;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"AZEAA"))
	{
		CS_stncp (ptr07->prj_knm,"AZMEA",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm2;
		ptr07->prj_prm2 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"AZEAP"))
	{
		CS_stncp (ptr07->prj_knm,"AZMEA",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm3 = cs_Zero;
		Az = CScsrupPNT (ptr07);
		ptr07->prj_prm1 = Az;
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm2 = cs_Zero;
		ptr07->prj_prm3 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"STROA"))
	{
		CS_stncp (ptr07->prj_knm,"STERO",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm2;
		ptr07->prj_prm2 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"STROP"))
	{
		CS_stncp (ptr07->prj_knm,"STERO",sizeof (ptr07->prj_knm));
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm3 = cs_Zero;
		Az = CScsrupPNT (ptr07);
		ptr07->prj_prm1 = Az;
		ptr07->quad = (short)ptr07->prj_prm3;
		ptr07->prj_prm2 = cs_Zero;
		ptr07->prj_prm3 = cs_Zero;
	}
	else if (!CS_stricmp (ptr07->prj_knm,"SINUS") &&
			 ptr07->org_lng == 0.0 && ptr07->prj_prm1 != 0.0)
	{
		/* Sinusoidal has a parameter change. What used to be
		   parm1 is now org_lng. My source control system
		   says this happened prior to release 6, but
		   checking the old distribution files, it appears
		   that this took place in 7.0.

		   I have added the checking of the values as an
		   extra precaution. */

		ptr07->org_lng = ptr07->prj_prm1;
		ptr07->prj_prm1 = cs_Zero;
	}

	/* Some clients have added definitions to
	   dictionaries without seroing the structure.
	   Probaly because the documentation didn't
	   indicate that that was expected.  Here we
	   zero all unused parameters to overcome this. */

	usPrjCode = prjCodeFromName(ptr07->prj_knm);
	if (cs_PRJCOD_END == usPrjCode) return -1;

	MASKPRM(ptr07->prj_prm1,usPrjCode,1 );				/*lint !e778 */
	MASKPRM(ptr07->prj_prm2,usPrjCode,2 );
	MASKPRM(ptr07->prj_prm3,usPrjCode,3 );
	MASKPRM(ptr07->prj_prm4,usPrjCode,4 );
	MASKPRM(ptr07->prj_prm5,usPrjCode,5 );
	MASKPRM(ptr07->prj_prm6,usPrjCode,6 );
	MASKPRM(ptr07->prj_prm7,usPrjCode,7 );
	MASKPRM(ptr07->prj_prm8,usPrjCode,8 );
	MASKPRM(ptr07->prj_prm9,usPrjCode,9 );
	MASKPRM(ptr07->prj_prm10,usPrjCode,10);
	MASKPRM(ptr07->prj_prm11,usPrjCode,11);
	MASKPRM(ptr07->prj_prm12,usPrjCode,12);
	MASKPRM(ptr07->prj_prm13,usPrjCode,13);
	MASKPRM(ptr07->prj_prm14,usPrjCode,14);
	MASKPRM(ptr07->prj_prm15,usPrjCode,15);
	MASKPRM(ptr07->prj_prm16,usPrjCode,16);
	MASKPRM(ptr07->prj_prm17,usPrjCode,17);
	MASKPRM(ptr07->prj_prm18,usPrjCode,18);
	MASKPRM(ptr07->prj_prm19,usPrjCode,19);
	MASKPRM(ptr07->prj_prm20,usPrjCode,20);
	MASKPRM(ptr07->prj_prm21,usPrjCode,21);
	MASKPRM(ptr07->prj_prm22,usPrjCode,22);
	MASKPRM(ptr07->prj_prm23,usPrjCode,23);
	MASKPRM(ptr07->prj_prm24,usPrjCode,24);

	if (1 != ptr07->protect)
	{
		/* The "source" and "group" from previous revisions may be dirty */
		memset(ptr07->source, 0, sizeof(ptr07->source));
		memset(ptr07->group, 0, sizeof(ptr07->group));

		/* If the coordinate system specifies a datum, sometimes the
		   ellipsoid is left dirty. */
		if ('\0' != ptr07->dat_knm[0])
		{
			//Datum was specified, so zero out the ellipsoid
			//key name.
			memset(ptr07->elp_knm, 0, sizeof(ptr07->elp_knm));
		}

		/* The "location" and "cntry_st" may be dirty. */
		memset(ptr07->locatn, 0, sizeof(ptr07->locatn));
		memset(ptr07->cntry_st, 0, sizeof(ptr07->cntry_st));
	}	/* if it's a user-defined coordinate system */

	/* We now have a release 7 structure. */
	
	csrup->level = 7;
	return (0);
}

int CScsrupTo08 (struct csCsrup_ *csrup)
{
	extern double cs_MaxLatFz;

	struct cs_Csdef07_ *ptr07;
	struct cs_Csdef08_ *ptr08;

	long32_t tmpl;

	ptr07 = (struct cs_Csdef07_ *)CS_malc (sizeof (struct cs_Csdef07_));
	if (ptr07 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr07,&csrup->u.csdef07,sizeof (struct cs_Csdef07_));

	ptr08 = &csrup->u.csdef08;
	memset (ptr08,'\0',sizeof (struct cs_Csdef08_));

	CS_stncp (ptr08->key_nm,ptr07->key_nm,sizeof (ptr08->key_nm));
	CS_stncp (ptr08->dat_knm,ptr07->dat_knm,sizeof (ptr08->dat_knm));
	CS_stncp (ptr08->elp_knm,ptr07->elp_knm,sizeof (ptr08->elp_knm));
	CS_stncp (ptr08->prj_knm,ptr07->prj_knm,sizeof (ptr08->prj_knm));
	CS_stncp (ptr08->group,ptr07->group,sizeof (ptr08->group));
	CS_stncp (ptr08->locatn,ptr07->locatn,sizeof (ptr08->locatn));
	CS_stncp (ptr08->cntry_st,ptr07->cntry_st,sizeof (ptr08->cntry_st));
	CS_stncp (ptr08->unit,ptr07->unit,sizeof (ptr08->unit));

	/* We leave fill set to zero until we know why this won't work. */

	ptr08->prj_prm1  = ptr07->prj_prm1;
	ptr08->prj_prm2  = ptr07->prj_prm2;
	ptr08->prj_prm3  = ptr07->prj_prm3;
	ptr08->prj_prm4  = ptr07->prj_prm4;
	ptr08->prj_prm5  = ptr07->prj_prm5;
	ptr08->prj_prm6  = ptr07->prj_prm6;
	ptr08->prj_prm7  = ptr07->prj_prm7;
	ptr08->prj_prm8  = ptr07->prj_prm8;
	ptr08->prj_prm9  = ptr07->prj_prm9;
	ptr08->prj_prm10 = ptr07->prj_prm10;
	ptr08->prj_prm11 = ptr07->prj_prm11;
	ptr08->prj_prm12 = ptr07->prj_prm12;
	ptr08->prj_prm13 = ptr07->prj_prm13;
	ptr08->prj_prm14 = ptr07->prj_prm14;
	ptr08->prj_prm15 = ptr07->prj_prm15;
	ptr08->prj_prm16 = ptr07->prj_prm16;
	ptr08->prj_prm17 = ptr07->prj_prm17;
	ptr08->prj_prm18 = ptr07->prj_prm18;
	ptr08->prj_prm19 = ptr07->prj_prm19;
	ptr08->prj_prm20 = ptr07->prj_prm20;
	ptr08->prj_prm21 = ptr07->prj_prm21;
	ptr08->prj_prm22 = ptr07->prj_prm22;
	ptr08->prj_prm23 = ptr07->prj_prm23;
	ptr08->prj_prm24 = ptr07->prj_prm24;

	ptr08->org_lng   = ptr07->org_lng;
	ptr08->org_lat   = ptr07->org_lat;
	ptr08->x_off     = ptr07->x_off;
	ptr08->y_off     = ptr07->y_off;
	ptr08->scl_red   = ptr07->scl_red;
	ptr08->unit_scl  = ptr07->unit_scl;
	ptr08->map_scl   = ptr07->map_scl;
	ptr08->scale     = ptr07->scale;
	ptr08->zero [XX] = ptr07->zero [XX];
	ptr08->zero [YY] = ptr07->zero [YY];

	ptr08->hgt_lng   = ptr07->hgt_lng;
	ptr08->hgt_lat   = ptr07->hgt_lat;
	ptr08->hgt_zz    = ptr07->hgt_zz;
	ptr08->geoid_sep = ptr07->geoid_sep;

	ptr08->ll_min [LNG] = ptr07->ll_min [LNG];
	ptr08->ll_min [LAT] = ptr07->ll_min [LAT];
	ptr08->ll_max [LNG] = ptr07->ll_max [LNG];
	ptr08->ll_max [LAT] = ptr07->ll_max [LAT];
	ptr08->xy_min [XX]  = ptr07->xy_min [XX];
	ptr08->xy_min [YY]  = ptr07->xy_min [YY];
	ptr08->xy_max [XX]  = ptr07->xy_max [XX];
	ptr08->xy_max [YY]  = ptr07->xy_max [YY];

	CS_stncp (ptr08->desc_nm,ptr07->desc_nm,sizeof (ptr08->desc_nm));
	CS_stncp (ptr08->source,ptr07->source,sizeof (ptr08->source));

	ptr08->quad = ptr07->quad;
	ptr08->order = ptr07->order;
	ptr08->zones = ptr07->zones;
	if (ptr07->protect <= 1)
	{
		ptr08->protect = ptr07->protect;
	}
	else
	{
		tmpl = (ptr07->protect * 86400L) + 1728000L;
		ptr08->protect = (short)((tmpl - 630720000L) / 86400L);
	}
	ptr08->epsg_qd = ptr07->epsg_qd;
	ptr08->srid = ptr07->srid;

	/* We don't need the old definition anymore. */

	CS_free (ptr07);

	/* Adjust for the change in the key names of some projections. */

	if (!CS_stricmp (ptr08->prj_knm,"STERO"))
	{
		if (fabs (ptr08->org_lat) > cs_MaxLatFz)
		{
			strcpy (ptr08->prj_knm,"PSTERO");
		}
		else
		{
			strcpy (ptr08->prj_knm,"OSTEROUS");
		}
	}

	/* Adjust for the change in the key names of some units. */

	if (!CS_stricmp (ptr08->unit,"CHAIN"))
	{
		strcpy (ptr08->unit,"GunterChain");
	}

	/* I think that's it. */

	csrup->level = 8;
	return (0);
}

double EXP_LVL9 CScsrupPNT (struct cs_Csdef_ *csdef)
{
	extern double cs_Zero;
	extern double cs_Degree;

	struct cs_Datum_ *dt_ptr;
	struct cs_Eldef_ *el_ptr;

	double Az;
	double e_rad;
	double ecent;
	double dummy;

	double ll0 [2];
	double ll1 [2];

	ll0 [LNG] = csdef->org_lng * cs_Degree;
	ll0 [LAT] = csdef->org_lat * cs_Degree;
	ll1 [LNG] = csdef->prj_prm1 * cs_Degree;
	ll1 [LAT] = csdef->prj_prm2 * cs_Degree;

	e_rad = ecent = cs_Zero;
	if (csdef->dat_knm [0] != '\0')
	{
		dt_ptr = CS_dtloc (csdef->dat_knm);
		if (dt_ptr != NULL)
		{
			e_rad = dt_ptr->e_rad;
			ecent = dt_ptr->ecent;
			CS_free (dt_ptr);
		}
	}
	if (e_rad == 0.0 && csdef->elp_knm [0] != '\0')
	{
		el_ptr = CS_eldef (csdef->elp_knm);
		if (el_ptr != NULL)
		{
			e_rad = el_ptr->e_rad;
			ecent = el_ptr->ecent;
			CS_free (el_ptr);
		}
	}
	if (e_rad == 0.0)
	{
		/* Else just use Clarke 1866. */

		e_rad = 6378206.4;
		ecent = 0.082271854;
	}
	Az = CS_llazdd (e_rad,ecent*ecent,ll0,ll1,&dummy);
	return (Az);
}

/*
	Given a projection key name, return the projection
	code number.  Table look up required. */

static unsigned short EXP_LVL9 prjCodeFromName(Const char *kpPrjName)
{
	extern struct cs_Prjtab_ cs_Prjtab [];

	short sIndex;

	for (sIndex=0;cs_Prjtab[sIndex].key_nm[0] != '\0';sIndex++)
	{
		if (!CS_stricmp(kpPrjName,cs_Prjtab[sIndex].key_nm))
		{
			break;
		}
	}
	if (cs_Prjtab[sIndex].check == NULL)
	{
		return cs_PRJCOD_END;
	}
	return (cs_Prjtab[sIndex].code);
}
/**********************************************************************
**	status = CS_dtrup (char *distrb,char *bkupnm);
**
**	char *update;				name of the distribution datum dictionary file,
**								i.e. the latest distribution.
**	char *bkupnm;				name which is to be given to the old file instead
**								of deleteing it.
**	int status;					returns status value, see Remarks below.
**
**	If the distribution file argument has no directory information
**	in it, i.e. no directory separators, it is looked for first
**	in the directory indicated by cs_Dir.  If not found there, the
**	current directory is searched. If directory information is
**	present in the distrb argument, the file must be there.
**
**	A new datum dictionary is created in a temporary file in the
**	directory indicated by cs_Dir. Thus, a failure in this function
**	(other than the final remove and rename) has no effect on
**	existsing files.
**
**	The normal return status is zero. A negative return value
**	indicates failure.
**********************************************************************/

/*
	The following three defines enable us to switch to the
	next release relatively easily.  Note, the it is the
	element indicated by dtdefCUR which actually gets written
	when and old record is written to the new Datum Dictionary.

	Note that in many cases, the structure does not change, but
	the use of the contents change. In this case, we have a
	new magic number, we need to do some updating, but the
	actual structure remains the same. In such cases, we
	use a define to "make believe" we have a new structure.
*/

#define cs_Dtdef07_ cs_Dtdef_			/* 7 and 8 structures are
										   identical */
#define cs_Dtdef08_ cs_Dtdef_	
#define cs_BSWP_DTDEF07 cs_BSWP_DTDEF
#define cs_BSWP_DTDEF08 cs_BSWP_DTDEF
#define dtdefCUR dtdef08

struct csDtrup_
{
	short level;
	short crypt;
	union
	{
		struct cs_Dtdef05_ dtdef05;
		struct cs_Dtdef06_ dtdef06;
		struct cs_Dtdef07_ dtdef07;
		struct cs_Dtdef08_ dtdef08;
	} u;
};

__ALIGNMENT__1					/* For some versions of Sun compiler. */
static struct csDtrup_ csDtrup;

__ALIGNMENT__2					/* For some versions of Sun compiler. */
static struct cs_Dtdef_ cs_Dtdef;

int CSdtrupReadOld (csFILE *oldStrm,struct csDtrup_ *dtrup,int old_lvl);
int CSdtrupRead05 (csFILE *oldStrm,struct csDtrup_ *dtrup);
int CSdtrupRead06 (csFILE *oldStrm,struct csDtrup_ *dtrup);
int CSdtrupRead07 (csFILE *oldStrm,struct csDtrup_ *dtrup);
int CSdtrupRead08 (csFILE *oldStrm,struct csDtrup_ *dtrup);
int CSdtrupTo06 (struct csDtrup_ *dtrup);
int CSdtrupTo07 (struct csDtrup_ *dtrup);
int CSdtrupTo08 (struct csDtrup_ *dtrup);

int EXP_LVL3 CS_dtrup (Const char *distrb,Const char *bkupnm)
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_Dtname [];
	extern char csErrnam [];
	extern char cs_DirsepC;

	int st;
	int cmp;
	int upd_st;
	int old_st;
	int old_lvl;
	int upd_crypt;

	size_t rdCnt;
	size_t wrCnt;

	const char *kCp;
	csFILE *oldStrm;
	csFILE *tmpStrm;
	csFILE *updStrm;

	cs_magic_t magic;

	char tmp_fnm [MAXPATH];
	char ctemp [MAXPATH];

	/* Prepare for the possibility of an error of some sort. */

	oldStrm = NULL;
	tmpStrm = NULL;
	updStrm = NULL;
	tmp_fnm [0] = '\0';
	old_lvl = 0;

	/* Open up the distribution file if there is one. */
	if (distrb != NULL && distrb [0] != '\0')
	{
		kCp = strchr (distrb,cs_DirsepC);
		if (kCp == NULL)
		{
			/* No directory information. We try cs_Dir first. */
			CS_stcpy (cs_DirP,distrb);
			updStrm = CS_fopen (cs_Dir,_STRM_BINRD);
		}
		if (updStrm == NULL)
		{
			updStrm = CS_fopen (distrb,_STRM_BINRD);
			if (updStrm == NULL)
			{
				CS_stncp (csErrnam,distrb,MAXPATH);
				CS_erpt (cs_FL_OPEN);
				goto error;
			}
		}
	
		/* Read the magic number. */
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),updStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (CS_ferror (updStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");
		if (magic != cs_DTDEF_MAGIC)
		{
			CS_erpt (cs_DT_BAD_MAGIC);
			goto error;
		}
	}

	/* Open up the existing file and get it's magic number
	   so we know what we are expected to do. */
	strcpy (cs_DirP,cs_Dtname);
	oldStrm = CS_fopen (cs_Dir,_STRM_BINRD);
	if (oldStrm != NULL)
	{
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),oldStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");

		/* Determine the release level of the old file.  We're 100%
		   ANSI now, so we can use a switch on a long. */
		switch (magic) {
		case cs_DTDEF_MAGIC05: old_lvl = 5; break;
		case cs_DTDEF_MAGIC06: old_lvl = 6; break;
		case cs_DTDEF_MAGIC07: old_lvl = 7; break;
		case cs_DTDEF_MAGIC:   old_lvl = 8; break;
		default:
			/* It isn't a datum dictionary file. */
			CS_erpt (cs_DT_BAD_MAGIC);
			goto error;
		}
	}

	/* Now we have the lay of the land, see what we are supposed
	   to do. */
	if (updStrm == NULL && oldStrm == NULL)
	{
		/* There are no files, we can't really do anything. */
		CS_erpt (cs_DT_NOT_FND);
		goto error;
	}

	/* One of three things to do. Regardless of which, we'll need
	   a temporary file.  Create it now.  We could use CS_tmpfn,
	   but it calls CS_erpt, and we want to avoid that in this
	   module. */
	st = CS_tmpfn (tmp_fnm);
	if (st != 0) goto error;
	tmpStrm = CS_fopen (tmp_fnm,_STRM_BINWR);
	if (tmpStrm == NULL)
	{
		CS_erpt (cs_NWDT_WRIT);
		goto error;
	}
	magic = cs_DTDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite (&magic,1,sizeof (magic),tmpStrm);
	if (wrCnt != sizeof (magic))
	{
		CS_erpt (cs_NWDT_WRIT);
		goto error;
	}

	/* OK, we have someting to do, and a temporary file to do it
	   it with. Let's get to it. */
	if (updStrm != NULL && oldStrm == NULL)
	{
		/* We have a distribution file, but no old file. A
		   simple file copy will do it. */
		while ((rdCnt = CS_fread (&cs_Dtdef,1,sizeof (cs_Dtdef),updStrm)) == sizeof (cs_Dtdef))
		{
			wrCnt = CS_fwrite (&cs_Dtdef,1,rdCnt,tmpStrm);
			if (wrCnt != rdCnt)
			{
				CS_erpt (cs_NWDT_WRIT);
				goto error;
			}
		}
		if (!CS_feof (updStrm))
		{
			CS_erpt (CS_ferror (updStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
	}
	else if (updStrm == NULL && oldStrm != NULL)
	{
		/* We have an old file, but no update file.  We
		   simply reformat the old file. */
		for (;;)
		{
			old_st = CSdtrupReadOld (oldStrm,&csDtrup,old_lvl);
			if (old_st < 0) goto error;
			if (old_st == 0) break;
			CS_dtwr (tmpStrm,&csDtrup.u.dtdefCUR,csDtrup.crypt);
		}
	}
	else
	{
		/* Here if we have a merge to do. Prime the pump with
		   one record from each file. */
		upd_st = CS_dtrd (updStrm,&cs_Dtdef,&upd_crypt);
		if (upd_st < 0) goto error;
		if (upd_st == 0) cs_Dtdef.key_nm [0] = '~';
		old_st = CSdtrupReadOld (oldStrm,&csDtrup,old_lvl);
		if (old_st < 0) goto error;
		if (old_st == 0) cs_Dtdef.key_nm [0] = '~';

		/* Loop, writing records in sorted order. If there are
		   duplicates, we write the update record. */
		while (upd_st > 0 || old_st > 0)
		{
			if      (upd_st == 0) cmp = 1;
			else if (old_st == 0) cmp = -1;
			else     cmp = CS_stricmp (cs_Dtdef.key_nm,csDtrup.u.dtdefCUR.key_nm);
			if (cmp == 0)
			{
				/* Write the distribution record, can the
				   the old record. */
				CS_dtwr (tmpStrm,&cs_Dtdef,upd_crypt);
				upd_st = CS_dtrd (updStrm,&cs_Dtdef,&upd_crypt);
				if (upd_st < 0) goto error;
				old_st = CSdtrupReadOld (oldStrm,&csDtrup,old_lvl);
				if (old_st < 0) goto error;
			}
			else if (cmp < 0)
			{
				/* Write the distribution record. */
				CS_dtwr (tmpStrm,&cs_Dtdef,upd_crypt);
				upd_st = CS_dtrd (updStrm,&cs_Dtdef,&upd_crypt);
				if (upd_st < 0) goto error;
			}
			else
			{
				/* Write the old record. */
				CS_dtwr (tmpStrm,&csDtrup.u.dtdefCUR,csDtrup.crypt);
				old_st = CSdtrupReadOld (oldStrm,&csDtrup,old_lvl);
				if (old_st < 0) goto error;
			}
		}
	}

	/* Clean up. */
	if (tmpStrm != NULL)		/*lint !e774 */  /*Redundant, but nice to have. */
	{
		CS_fclose (tmpStrm);
		tmpStrm = NULL;
	}
	if (oldStrm != NULL)
	{
		CS_fclose (oldStrm);
		oldStrm = NULL;
	}
	if (updStrm != NULL)
	{
		CS_fclose (updStrm);
		updStrm = NULL;
	}

	if (bkupnm != NULL && *bkupnm != '\0')
	{
		CS_stcpy (cs_DirP,bkupnm);
		CS_stncp (ctemp,cs_Dir,sizeof (ctemp));
		remove (ctemp);					/*lint !e534  ignoring return value */
		CS_stcpy (cs_DirP,cs_Dtname);
		CS_rename (cs_Dir,ctemp);					/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	else
	{
		CS_stcpy (cs_DirP,cs_Dtname);
		remove (cs_Dir);					/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	return ((st == 0) ? 0 : -1);
error:
	if (oldStrm != NULL) CS_fclose (oldStrm);
	if (tmpStrm != NULL) CS_fclose (tmpStrm);
	if (updStrm != NULL) CS_fclose (updStrm);
	if (tmp_fnm [0] != '\0')
	{
		remove (tmp_fnm);					/*lint !e534  ignoring return value */
	}
	return (-1);
}

int CSdtrupReadOld (csFILE *oldStrm,struct csDtrup_ *dtrup,int old_lvl)
{
	extern char csErrnam [];

	int old_st;

	switch (old_lvl) {
	case  5: old_st = CSdtrupRead05 (oldStrm,dtrup); break;
	case  6: old_st = CSdtrupRead06 (oldStrm,dtrup); break;
	case  7: old_st = CSdtrupRead07 (oldStrm,dtrup); break;
	case  8: old_st = CSdtrupRead08 (oldStrm,dtrup); break;
	default:
		CS_stncp (csErrnam,"CS_rlsUpdt:3",MAXPATH);
		CS_erpt (cs_ISER);
		old_st = -1;
	}
	if (old_st > 0)
	{
		/* Bring the record up to date. */
		if (dtrup->level == 5)
		{
			if (CSdtrupTo06 (dtrup) != 0) old_st = -1;
		}
		if (dtrup->level == 6)
		{
			if (CSdtrupTo07 (dtrup) != 0) old_st = -1;
		}
		if (dtrup->level == 7)
		{
			if (CSdtrupTo08 (dtrup) != 0) old_st = -1;
		}
	}
	return (old_st);
}

/*
	The following function reads a release 5 Datum Dictionary.
*/
int CSdtrupRead05 (csFILE *oldStrm,struct csDtrup_ *dtrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Dtdef05_ *ptr;

	ptr = &dtrup->u.dtdef05;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		dtrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		dtrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_DTDEF05);

	if (ptr->key_nm [0] < 'A' || ptr->key_nm [0] > 'Z')
	{
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	dtrup->level = 5;
	return (1);
}

int CSdtrupRead06 (csFILE *oldStrm,struct csDtrup_ *dtrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Dtdef06_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &dtrup->u.dtdef06;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		dtrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		dtrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_DTDEF06);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	dtrup->level = 6;
	return (1);
}

int CSdtrupRead07 (csFILE *oldStrm,struct csDtrup_ *dtrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Dtdef07_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &dtrup->u.dtdef07;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		dtrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		dtrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_DTDEF07);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	dtrup->level = 7;
	return (1);
}
/*
	Read a Release 8 datum dictionary structure.
*/
int CSdtrupRead08 (csFILE *oldStrm,struct csDtrup_ *dtrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Dtdef08_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &dtrup->u.dtdef08;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		dtrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		dtrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_DTDEF08);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */
		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	dtrup->level = 8;
	return (1);
}

/*
	The following function upgrades the provided csDtrup_
	structure from release five to release six.
*/
int CSdtrupTo06 (struct csDtrup_ *dtrup)
{
	extern double cs_Zero;

	unsigned int tmp;

	struct cs_Dtdef05_ *ptr05;
	struct cs_Dtdef06_ *ptr06;

	ptr05 = (struct cs_Dtdef05_ *)CS_malc (sizeof (struct cs_Dtdef05_));
	if (ptr05 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr05,&dtrup->u.dtdef05,sizeof (struct cs_Dtdef05_));

	ptr06 = &dtrup->u.dtdef06;
	memset (ptr06,'\0',sizeof (struct cs_Dtdef06_));

	CS_stncp (ptr06->key_nm,ptr05->key_nm,sizeof (ptr06->key_nm));
	CS_stncp (ptr06->ell_knm,ptr05->ell_knm,sizeof (ptr06->ell_knm));
	tmp = sizeof (ptr06->fill);
	if (tmp > sizeof (ptr05->fill)) tmp = sizeof (ptr05->fill);			/*lint !e774 */
	memcpy (ptr06->fill,ptr05->fill,tmp);
	ptr06->delta_X = ptr05->delta_X;
	ptr06->delta_Y = ptr05->delta_Y;
	ptr06->delta_Z = ptr05->delta_Z;
	ptr06->rot_X = cs_Zero;
	ptr06->rot_Y = cs_Zero;
	ptr06->rot_Z = cs_Zero;
	ptr06->bwscale = cs_Zero;
	ptr06->protect = ptr05->protect;
	if (ptr06->protect == 0)
	{
		ptr06->protect = (short)((CS_time ((cs_Time_ *)0) - 1728000L) / 86400L);
	}
	CS_stncp (ptr06->name,ptr05->name,sizeof (ptr06->name));

	CS_free (ptr05);
	dtrup->level = 6;
	return (0);
}

int CSdtrupTo07 (struct csDtrup_ *dtrup)
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_ExtsepC;

	char *cp;
	struct cs_Dtdef06_ *ptr06;
	struct cs_Dtdef07_ *ptr07;

	unsigned int tmp;

	ptr06 = (struct cs_Dtdef06_ *)CS_malc (sizeof (struct cs_Dtdef06_));
	if (ptr06 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr06,&dtrup->u.dtdef06,sizeof (struct cs_Dtdef06_));

	ptr07 = &dtrup->u.dtdef07;
	memset (ptr07,'\0',sizeof (struct cs_Dtdef07_));

	CS_stncp (ptr07->key_nm,ptr06->key_nm,sizeof (ptr07->key_nm));
	CS_stncp (ptr07->ell_knm,ptr06->ell_knm,sizeof (ptr07->ell_knm));
	CS_stncp (ptr07->locatn,ptr06->locatn,sizeof (ptr07->locatn));
	CS_stncp (ptr07->cntry_st,ptr06->cntry_st,sizeof (ptr07->cntry_st));
	tmp = sizeof (ptr07->fill);
	if (tmp > sizeof (ptr06->fill)) tmp = sizeof (ptr06->fill);			/*lint !e774 */
	memcpy (ptr07->fill,ptr06->fill,tmp);
	ptr07->delta_X = ptr06->delta_X;
	ptr07->delta_Y = ptr06->delta_Y;
	ptr07->delta_Z = ptr06->delta_Z;
	ptr07->rot_X   = ptr06->rot_X;
	ptr07->rot_Y   = ptr06->rot_Y;
	ptr07->rot_Z   = ptr06->rot_Z;
	ptr07->bwscale = ptr06->bwscale;
	ptr07->protect = ptr06->protect;
	if (ptr07->protect == 0)
	{
		ptr07->protect = (short)((CS_time ((cs_Time_ *)0) - 1728000L) / 86400L);
	}
	CS_stncp (ptr07->name,ptr06->name,sizeof (ptr07->name));
	CS_stncp (ptr07->source,ptr06->source,sizeof (ptr07->source));

	/* We don't need the old definition anymore. */

	CS_free (ptr06);

	/* Need to set the conversion type */

	if (!CS_stricmp (ptr07->key_nm,"NAD27"))
	{
		ptr07->to84_via = cs_DTCTYP_NAD27;
	}
	else if (!CS_stricmp (ptr07->key_nm,"NAD83"))
	{
		ptr07->to84_via = cs_DTCTYP_NAD83;
	}
	else if (!CS_stricmp (ptr07->key_nm,"WGS84"))
	{
		ptr07->to84_via = cs_DTCTYP_WGS84;
	}
	else if (!CS_stricmp (ptr07->key_nm,"WGS72"))
	{
		ptr07->to84_via = cs_DTCTYP_WGS72;
	}
	else if (!CS_stricmp (ptr07->key_nm,"HPGN"))
	{
		ptr07->to84_via = cs_DTCTYP_HPGN;
	}
	else
	{
		ptr07->to84_via = cs_DTCTYP_MOLO;
		if (ptr07->rot_X != 0.0 ||
			ptr07->rot_Y != 0.0 ||
			ptr07->rot_Z != 0.0 ||
			ptr07->bwscale != 0.0)
		{
			ptr07->to84_via = cs_DTCTYP_BURS;
		}
		cp = CS_stcpy (cs_DirP,ptr07->key_nm);
		*cp++ = cs_ExtsepC;
		CS_stcpy (cp,cs_MULREG_EXT);
		if (CS_access (cs_Dir,0) == 0)
		{
			ptr07->to84_via = cs_DTCTYP_MREG;
		}
	}

	/* Done */

	dtrup->level = 7;
	return (0);
}

int CSdtrupTo08 (struct csDtrup_ *dtrup)
{
	struct cs_Dtdef07_ *ptr07;
	struct cs_Dtdef08_ *ptr08;

	unsigned int tmp;
	long32_t tmpl;

	ptr07 = (struct cs_Dtdef07_ *)CS_malc (sizeof (struct cs_Dtdef07_));
	if (ptr07 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr07,&dtrup->u.dtdef07,sizeof (struct cs_Dtdef07_));

	ptr08 = &dtrup->u.dtdef08;
	memset (ptr08,'\0',sizeof (struct cs_Dtdef08_));

	CS_stncp (ptr08->key_nm,ptr07->key_nm,sizeof (ptr08->key_nm));
	CS_stncp (ptr08->ell_knm,ptr07->ell_knm,sizeof (ptr08->ell_knm));
	CS_stncp (ptr08->locatn,ptr07->locatn,sizeof (ptr08->locatn));
	CS_stncp (ptr08->cntry_st,ptr07->cntry_st,sizeof (ptr08->cntry_st));
	tmp = sizeof (ptr08->fill);
	if (tmp > sizeof (ptr07->fill)) tmp = sizeof (ptr07->fill);		/*lint !e774 */
	memcpy (ptr08->fill,ptr07->fill,tmp);
	ptr08->delta_X = ptr07->delta_X;
	ptr08->delta_Y = ptr07->delta_Y;
	ptr08->delta_Z = ptr07->delta_Z;
	ptr08->rot_X   = ptr07->rot_X;
	ptr08->rot_Y   = ptr07->rot_Y;
	ptr08->rot_Z   = ptr07->rot_Z;
	ptr08->bwscale = ptr07->bwscale;
	ptr08->to84_via = ptr07->to84_via;
	CS_stncp (ptr08->name,ptr07->name,sizeof (ptr08->name));
	CS_stncp (ptr08->source,ptr07->source,sizeof (ptr08->source));

	if (ptr07->protect <= 1)
	{
		ptr08->protect = ptr07->protect;
	}
	else
	{
		/* Fix the protection value. */
		
		tmpl = (ptr07->protect * 86400L) + 1728000L;
		ptr08->protect = (short)((tmpl - 630720000L) / 86400L);
	}

	/* We don't need the old definition anymore. */

	CS_free (ptr07);

	/* Done */

	dtrup->level = 8;
	return (0);
}

/**********************************************************************
**	status = CS_elrup (char *distrb,char *bkupnm);
**
**	char *update;				name of the distribution ellipsoid
**								dictionary file, i.e. the latest
**								distribution.
**	char *bkupnm;				name which is to be given to the old
**								file instead of deleteing it.
**	int status;					returns status value.
**
**	If the distribution file argument has no directory information
**	in it, i.e. no directory separators, it is looked for first
**	in the directory indicated by cs_Dir.  If not found there, the
**	current directory is searched. If directory information is
**	present in the distrb argument, the file must be there.
**
**	A new ellipsoid dictionary is created in a temporary file in the
**	directory indicated by cs_Dir. Thus, a failure in this function
**	(other than the final remove and rename) has no effect on
**	existsing files.
**
**	The normal return status is zero. A negative return value
**	indicates failure.
**
**	Note that functions exist in this module which will not be
**	called. That is, when updating for a specific release, the
**	functions for that release are written which won;t be used
**	until the next release. This, supposedly, makes life easier
**	for the maintainer.
**********************************************************************/

/*
	We use the following defines to upgrade from one release to
	another. Essentially, for each release there shall exist a
	cs_Eldef??_ structure. If the structure has not actually
	changed for a spcific release, as is common, than a define
	is decalred which matches it to the specific structure to
	which it is identical. Note, when a structure is changed,
	the old structure takes a name with a number which is
	the highest release number for which that structure was
	valid. Of course, the byte swap strings follow suite.

	Note, that there never was a separate structure or
	magic number for release 6. In all of the following,
	release 7 stuff applies to release 6 stuff as well.

	Also note, that the ellipsoid definition structure
	did not change from release 7 to release 8.
*/

#define cs_Eldef07_ cs_Eldef_
#define cs_Eldef08_ cs_Eldef_
#define cs_BSWP_ELDEF07 cs_BSWP_ELDEF
#define cs_BSWP_ELDEF08 cs_BSWP_ELDEF
#define eldefCUR eldef08			/* This is the element
									   which actually gets
									   writen. */

/*
	The folloing structure is used to read and convert
	dictionary files from release 5 on up to the
	current level.
*/

struct csElrup_
{
	short level;
	short crypt;
	union
	{
		struct cs_Eldef05_ eldef05;
		struct cs_Eldef07_ eldef07;
		struct cs_Eldef08_ eldef08;
		struct cs_Eldef_   eldef;
	} u;
};

__ALIGNMENT__1					/* For some versions of Sun compiler. */
static struct csElrup_ csElrup;

__ALIGNMENT__2					/* For some versions of Sun compiler. */
static struct cs_Eldef_ cs_Eldef;

int CSelrupReadOld (csFILE *oldStrm,struct csElrup_ *elrup,int old_lvl);
int CSelrupRead05 (csFILE *oldStrm,struct csElrup_ *elrup);
int CSelrupRead07 (csFILE *oldStrm,struct csElrup_ *elrup);
int CSelrupRead08 (csFILE *oldStrm,struct csElrup_ *elrup);
int CSelrupTo07 (struct csElrup_ *elrup);
int CSelrupTo08 (struct csElrup_ *elrup);

int EXP_LVL3 CS_elrup (Const char *distrb,Const char *bkupnm)
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_Elname [];
	extern char csErrnam [];
	extern char cs_DirsepC;

	int st;
	int cmp;
	int upd_st;
	int old_st;
	int old_lvl;
	int upd_crypt;

	size_t rdCnt;
	size_t wrCnt;

	const char* kCp;
	csFILE *oldStrm;
	csFILE *tmpStrm;
	csFILE *updStrm;

	cs_magic_t magic;

	char tmp_fnm [MAXPATH];
	char ctemp [MAXPATH];

	/* Prepare for the possibility of an error of some sort. */

	oldStrm = NULL;
	tmpStrm = NULL;
	updStrm = NULL;
	tmp_fnm [0] = '\0';
	old_lvl = 0;

	/* Open up the distribution file if there is one. */

	if (distrb != NULL && distrb [0] != '\0')
	{
		kCp = strchr (distrb,cs_DirsepC);
		if (kCp == NULL)
		{
			/* No directory information. We try cs_Dir first. */
			CS_stcpy (cs_DirP,distrb);
			updStrm = CS_fopen (cs_Dir,_STRM_BINRD);
		}
		if (updStrm == NULL)
		{
			updStrm = CS_fopen (distrb,_STRM_BINRD);
			if (updStrm == NULL)
			{
				CS_stncp (csErrnam,distrb,MAXPATH);
				CS_erpt (cs_FL_OPEN);
				goto error;
			}
		}
	
		/* Read the magic number. */
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),updStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");
		if (magic != cs_ELDEF_MAGIC)
		{
			CS_erpt (cs_EL_BAD_MAGIC);
			goto error;
		}
	}

	/* Open up the existing file and get it's magic number
	   so we know what we are expected to do. */
	strcpy (cs_DirP,cs_Elname);
	oldStrm = CS_fopen (cs_Dir,_STRM_BINRD);
	if (oldStrm != NULL)
	{
		rdCnt = CS_fread ((char *)&magic,1,sizeof (magic),oldStrm);
		if (rdCnt != sizeof (magic))
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		CS_bswap (&magic,"l");

		/* Determine the release level of the old file.  We're 100%
		   ANSI now, so we can use a switch on a long. */
		switch (magic) {
		case cs_ELDEF_MAGIC05: old_lvl = 5; break;
		case cs_ELDEF_MAGIC07: old_lvl = 7; break;
		case cs_ELDEF_MAGIC:   old_lvl = 8; break;
		default:
			/* It isn't a ellipsoid dictionary file. */
			CS_erpt (cs_EL_BAD_MAGIC);
			goto error;
		}
	}

	/* Now we have the lay of the land, see what we are supposed
	   to do. */
	if (updStrm == NULL && oldStrm == NULL)
	{
		/* There are no files, we can't really do anything. */
		CS_erpt (cs_EL_NOT_FND);
		goto error;
	}

	/* One of three things to do. Regardless of which, we'll need
	   a temporary file.  Create it now. */
	st = CS_tmpfn (tmp_fnm);
	if (st != 0) goto error;
	tmpStrm = CS_fopen (tmp_fnm,_STRM_BINWR);
	if (tmpStrm == NULL)
	{
		CS_erpt (cs_NWEL_WRIT);
		goto error;
	}
	magic = cs_ELDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite (&magic,1,sizeof (magic),tmpStrm);
	if (wrCnt != sizeof (magic))
	{
		CS_erpt (cs_NWEL_WRIT);
		goto error;
	}

	/* OK, we have someting to do, and a temporary file to do it
	   it with. Let's get to it. */
	if (updStrm != NULL && oldStrm == NULL)
	{
		/* We have a distribution file, but no old file. A
		   simple file copy will do it. */
		while ((rdCnt = CS_fread (&cs_Eldef,1,sizeof (cs_Eldef),updStrm)) == sizeof (cs_Eldef))
		{
			wrCnt = CS_fwrite (&cs_Eldef,1,rdCnt,tmpStrm);
			if (wrCnt != rdCnt)
			{
				CS_erpt (cs_NWEL_WRIT);
				goto error;
			}
		}
		if (!CS_feof (tmpStrm))
		{
			CS_erpt (CS_ferror (tmpStrm) ? cs_IOERR : cs_INV_FILE);
			goto error;
		}
	}
	else if (updStrm == NULL && oldStrm != NULL)
	{
		/* We have an old file, but no update file.  We
		   simply reformat the old file. */
		for (;;)
		{
			old_st = CSelrupReadOld (oldStrm,&csElrup,old_lvl);
			if (old_st < 0) goto error;
			if (old_st == 0) break;
			CS_elwr (tmpStrm,&csElrup.u.eldefCUR,csElrup.crypt);
		}
	}
	else
	{
		/* Here if we have a merge to do. Prime the pump with
		   one record from each file. */
		upd_st = CS_elrd (updStrm,&cs_Eldef,&upd_crypt);
		if (upd_st < 0) goto error;
		if (upd_st == 0) cs_Eldef.key_nm [0] = '~';
		old_st = CSelrupReadOld (oldStrm,&csElrup,old_lvl);
		if (old_st < 0) goto error;
		if (old_st == 0) cs_Eldef.key_nm [0] = '~';

		/* Loop, writing records in sorted order. If there are
		   duplicates, we write the update record. */
		while (upd_st > 0 || old_st > 0)
		{
			if      (upd_st == 0) cmp = 1;
			else if (old_st == 0) cmp = -1;
			else     cmp = CS_stricmp (cs_Eldef.key_nm,csElrup.u.eldef.key_nm);
			if (cmp == 0)
			{
				/* Write the distribution record, can the
				   the old record. */
				CS_elwr (tmpStrm,&cs_Eldef,upd_crypt);
				upd_st = CS_elrd (updStrm,&cs_Eldef,&upd_crypt);
				if (upd_st < 0) goto error;
				old_st = CSelrupReadOld (oldStrm,&csElrup,old_lvl);
				if (old_st < 0) goto error;
			}
			else if (cmp < 0)
			{
				/* Write the distribution record. */
				CS_elwr (tmpStrm,&cs_Eldef,upd_crypt);
				upd_st = CS_elrd (updStrm,&cs_Eldef,&upd_crypt);
				if (upd_st < 0) goto error;
			}
			else
			{
				/* Write the old record. */
				CS_elwr (tmpStrm,&csElrup.u.eldefCUR,csElrup.crypt);
				old_st = CSelrupReadOld (oldStrm,&csElrup,old_lvl);
				if (old_st < 0) goto error;
			}
		}
	}

	/* Clean up. */
	CS_fclose (tmpStrm);
	tmpStrm = NULL;
	if (oldStrm != NULL)
	{
		CS_fclose (oldStrm);
		oldStrm = NULL;
	}
	if (updStrm != NULL)
	{
		CS_fclose (updStrm);
		updStrm = NULL;
	}

	if (bkupnm != NULL && *bkupnm != '\0')
	{
		CS_stcpy (cs_DirP,bkupnm);
		CS_stncp (ctemp,cs_Dir,sizeof (ctemp));
		remove (ctemp);								/*lint !e534  ignoring return value */
		CS_stcpy (cs_DirP,cs_Elname);
		CS_rename (cs_Dir,ctemp);					/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	else
	{
		CS_stcpy (cs_DirP,cs_Elname);
		CS_remove (cs_Dir);							/*lint !e534  ignoring return value */
		st = CS_rename (tmp_fnm,cs_Dir);
	}
	return ((st == 0) ? 0 : -1);

error:
	if (oldStrm != NULL) CS_fclose (oldStrm);
	if (tmpStrm != NULL) CS_fclose (tmpStrm);
	if (updStrm != NULL) CS_fclose (updStrm);
	if (tmp_fnm [0] != '\0')
	{
		CS_remove (tmp_fnm);					/*lint !e534  ignoring return value */
	}
	return (-1);
}

int CSelrupReadOld (csFILE *oldStrm,struct csElrup_ *elrup,int old_lvl)
{
	extern char csErrnam [];

	int old_st;

	switch (old_lvl) {
	case  5: old_st = CSelrupRead05 (oldStrm,elrup); break;
	case  7: old_st = CSelrupRead07 (oldStrm,elrup); break;
	case  8: old_st = CSelrupRead08 (oldStrm,elrup); break;
	default:
		CS_stncp (csErrnam,"CS_rlsUpdt:4",MAXPATH);
		CS_erpt (cs_ISER);
		old_st = -1;
	}
	if (old_st > 0)
	{
		/* Bring the record up to date. */

		if (elrup->level == 5)
		{
			if (CSelrupTo07 (elrup) != 0) old_st = -1;
		}
		if (elrup->level == 7)
		{
			if (CSelrupTo08 (elrup) != 0) old_st = -1;
		}
	}
	return (old_st);
}
/*
	The following function reads a release 5 Ellipsoid Dictionary.
*/
int CSelrupRead05 (csFILE *oldStrm,struct csElrup_ *elrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Eldef05_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &elrup->u.eldef05;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}
	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		elrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		elrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_ELDEF05);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */

		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	elrup->level = 5;
	return (1);
}

/*
	The following reads a Release 7 Ellipsoid Dictionary.
	Also applies to release 6 as there was no magic
	number change from 6 to 7.
*/
int CSelrupRead07 (csFILE *oldStrm,struct csElrup_ *elrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Eldef07_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &elrup->u.eldef07;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}

	/* Now for a real kludge. I screw'ed up and changed the
	   cs_Eldef_ structure slightly between release 6.02 and
	   6.03 and didn't change the magic number. I guess the
	   change was so slight, I didn't think it mattered.  However,
	   it did move the encryption key (which is perhaps another
	   reason it never got caught).

	   In any event, the following code deciphers the situation
	   and if the record came from release 6.02, it fixes it up
	   to be a release 6.04 type record.

	   The only problem here is locating the encryption key.
	   There are two possible choices. So, we try with the first
	   choice (where it really is supposed to be) and see if
	   we get a valid ellipsoid name as */
	if (ptr->fill  [0] == '\0' && ptr->fill  [1] == '\0' &&
		(ptr->group [0] ^ ptr->group [1]) == ptr->group [2] &&
		ptr->group [3] == '\0' && ptr->group [4] == '\0' &&
		ptr->group [5] == '\0')
	{
		key = (unsigned char)ptr->group [0];
	}
	else
	{
		key = (unsigned char)ptr->fill [0];
	}

	/* End of kludge. */
	if (key != '\0')
	{
		elrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		elrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_ELDEF07);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */

		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	elrup->level = 7;
	return (1);
}
/*
	The following reads a Release 8 Ellipsoid Dictionary.
*/
int CSelrupRead08 (csFILE *oldStrm,struct csElrup_ *elrup)
{
	unsigned char key;

	size_t rdCnt;

	unsigned char *cp;
	unsigned char *cpe;
	struct cs_Eldef08_ *ptr;

	char tmpKeyName [cs_KEYNM_DEF];

	ptr = &elrup->u.eldef08;

	rdCnt = CS_fread (ptr,1,sizeof (*ptr),oldStrm);			/*lint !e668 */
	if (rdCnt != sizeof (*ptr))
	{
		if (CS_feof (oldStrm)) return 0;
		CS_erpt (CS_ferror (oldStrm) ? cs_IOERR : cs_INV_FILE);
		return (-1);
	}

	key = (unsigned char)ptr->fill [0];
	if (key != '\0')
	{
		elrup->crypt = TRUE;
			cp = (unsigned char *)ptr;
			cpe = cp + sizeof (*ptr);
			while (cp < cpe)
			{
				key ^= *cp;
				*cp++ = key;
			}
		}
	else
	{
		elrup->crypt = FALSE;
	}
	CS_bswap (ptr,cs_BSWP_ELDEF08);

	/* Check the result. The name must always meet the criteria
	   set by the CS_nmpp function.  At least so far, the criteria
	   established by CS_nampp over the years has always been
	   expanded, never restricted.  Thus, any definition which
	   was legitimate in a previous release would always be
	   legitimate iin subsequent releases. */
	CS_stncp (tmpKeyName,ptr->key_nm,sizeof (tmpKeyName));
	if (CS_nampp (tmpKeyName) != 0)
	{
		/* Replace the error condition reported by CS_nampp with
		   and Invalid File indication. */

		CS_erpt (cs_INV_FILE);
		return (-1);
	}
	elrup->level = 8;
	return (1);
}

/*
	The following function upgrades the provided csElrup_
	structure from Release 5 to release 7.
*/

int CSelrupTo07 (struct csElrup_ *elrup)
{
	unsigned int tmp;

	struct cs_Eldef05_ *ptr05;
	struct cs_Eldef07_ *ptr07;

	ptr05 = (struct cs_Eldef05_ *)CS_malc (sizeof (struct cs_Eldef05_));
	if (ptr05 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr05,&elrup->u.eldef05,sizeof (struct cs_Eldef05_));

	ptr07 = &elrup->u.eldef07;
	memset (ptr07,'\0',sizeof (struct cs_Eldef07_));

	CS_stncp (ptr07->key_nm,ptr05->key_nm,sizeof (ptr07->key_nm));

	tmp = sizeof (ptr07->fill);
	if (tmp > sizeof (ptr05->fill)) tmp = sizeof (ptr05->fill);		/*lint !e774 */
	memcpy (ptr07->fill,ptr05->fill,tmp);							/*lint !e669 */

	ptr07->e_rad = ptr05->e_rad;
	ptr07->p_rad = ptr05->p_rad;
	ptr07->flat  = ptr05->flat;
	ptr07->ecent = ptr05->ecent;
	ptr07->protect = ptr05->protect;
	if (ptr07->protect == 0)
	{
		ptr07->protect = (short)((CS_time ((cs_Time_ *)0) - 1728000L) / 86400L);
	}
	CS_stncp (ptr07->name,ptr05->name,sizeof (ptr07->name));

	CS_free (ptr05);
	elrup->level = 6;
	return (0);
}

/*
	Convert a release 7 ellipsoid dictionary record up to release 8.
*/

int CSelrupTo08 (struct csElrup_ *elrup)
{
	struct cs_Eldef07_ *ptr07;
	struct cs_Eldef08_ *ptr08;

	unsigned int tmp;
	long32_t tmpl;

	ptr07 = (struct cs_Eldef07_ *)CS_malc (sizeof (struct cs_Eldef07_));
	if (ptr07 == NULL)
	{
		CS_erpt (cs_NO_MEM);
		return (-1);
	}
	memcpy (ptr07,&elrup->u.eldef07,sizeof (struct cs_Eldef07_));

	ptr08 = &elrup->u.eldef08;
	memset (ptr08,'\0',sizeof (struct cs_Eldef08_));

	CS_stncp (ptr08->key_nm,ptr07->key_nm,sizeof (ptr08->key_nm));
	CS_stncp (ptr08->group,ptr07->group,sizeof (ptr08->group));

	tmp = sizeof (ptr08->fill);
	if (tmp > sizeof (ptr07->fill)) tmp = sizeof (ptr07->fill);		/*lint !e774 */
	memcpy (ptr08->fill,ptr07->fill,tmp);

	ptr08->e_rad = ptr07->e_rad;
	ptr08->p_rad = ptr07->p_rad;
	ptr08->flat  = ptr07->flat;
	ptr08->ecent = ptr07->ecent;

	if (ptr07->protect <= 1)
	{
		ptr08->protect = ptr07->protect;
	}
	else
	{
		tmpl = (ptr07->protect * 86400L) + 1728000L;
		ptr08->protect = (short)((tmpl - 630720000L) / 86400L);
	}

	CS_stncp (ptr08->name,ptr07->name,sizeof (ptr08->name));
	CS_stncp (ptr08->source,ptr07->source,sizeof (ptr08->source));

	/* We don't need the old definition anymore. */

	CS_free (ptr07);
	elrup->level = 7;
	return (0);
}
