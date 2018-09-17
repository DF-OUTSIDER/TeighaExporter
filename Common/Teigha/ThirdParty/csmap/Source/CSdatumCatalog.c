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

/*		       * * * * R E M A R K S * * * *

	A Datum File Catalog provides a list of files associated with
	a specific datum shift calculation.  It enables datum files
	to be located in different directories, and the specification
	of a buffer size associated with each datum file.

	The purpose here is to enable a single copy of datum files
	on a system, and buffer sizes to be customized for a specific
	installation.

	For convenience, datum files can be marked as existsing in
	the same directory as the catalog file.  In this case, the
	catalog file does not have any specific path information
	in it; making copying of datum data files easy.  Of course,
	the whole idea of a datum catalog file is to preclude the
	need for copying datum files.
*/

/*lint -e722   suspicious use of ';', i.e. for loops without a body or statement */

#include "cs_map.h"
#include <ctype.h>

/******************************************************************************
	'Static' support function to replace directory separators as is appropriate.
*/
void EXP_LVL5 CSrplDirSep (char *path)
{
	extern char cs_DirsepC;

	char cc;
	char *cp;

	cp = path;
	while ((cc = *cp++) != '\0')
	{
		if (cc == '/' || cc == '\\') *(cp - 1) = cs_DirsepC;
	}
	return;
}

/******************************************************************************
	Functions to obtain, change, and disable gdc file names at run time.
	Originally invented to support the Gdc File Editor.  Applications wishing
	to change these filenames at run time (you can simply edit the header file
	for a compile time change) should use these functions.
*/
Const char * EXP_LVL1 CS_gdcEnum (int index,int *ident)
{
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	int idx;
	Const char *rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	/* Return a pointer to the index'th file name in the table.  Note,
	   certain entries may be disabled, and we don't count them.  If index
	   is invalid, we simply return null pointer.
	   
	   We assume the cs+GdcCatalogTable alwaqys has at least the terminating
	   entry in it. */

	idx = 0;
	rtnValue = NULL;
	if (index >= 0)
	{
		for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
		{
			if (tblPtr->ident == gdcFileDisabled) continue;
			if (idx == index) break;
			idx += 1;
		}
		if (tblPtr->ident != gdcFileNone)
		{
			rtnValue = tblPtr->gdcName;
			if (ident != NULL) *ident = (int)tblPtr->ident;
		}
	}
	return rtnValue;
}
int EXP_LVL3 CS_gdcSetName (enum cs_GdcCatalogs ident,Const char *newName)
{
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	int rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	rtnValue = -1;
	for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
	{
		if (tblPtr->ident == ident)
		{
			CS_stncp (tblPtr->gdcName,newName,cs_FNM_MAXLEN);
			rtnValue = 0;
			break;
		}
	}
	return rtnValue;
}
Const char * EXP_LVL3 CS_gdcGetName (enum cs_GdcCatalogs ident)
{
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	Const char *rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	rtnValue = NULL;
	for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
	{
		if (tblPtr->ident == ident)
		{
			rtnValue = tblPtr->gdcName;
			break;
		}
	}
	return rtnValue;
}
enum cs_GdcCatalogs EXP_LVL3 CS_gdcGetIdent (Const char *catalogName)
{
	extern char cs_ExtsepC;
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	size_t length;
	char *cp;
	enum cs_GdcCatalogs rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	rtnValue = gdcFileNone;
	for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
	{
		length = 0;
		cp = tblPtr->gdcName;
		while (*cp != '\0' && *cp != cs_ExtsepC)
		{
			length += 1;
			cp += 1;
		}
		if (!CS_strnicmp (tblPtr->gdcName,catalogName,length))
		{
			rtnValue = tblPtr->ident;
			break;
		}
	}
	return rtnValue;
}
int EXP_LVL3 CS_gdcDisable (enum cs_GdcCatalogs ident)
{
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	int rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	rtnValue = -1;
	for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
	{
		if (tblPtr->ident == ident)
		{
			tblPtr->ident = gdcFileDisabled;
			rtnValue = 0;
			break;
		}
	}
	return rtnValue;
}
Const struct cs_GdcCatalogTable_* EXP_LVL3 CS_gdcTable (enum cs_GdcCatalogs ident)
{
	extern struct cs_GdcCatalogTable_ cs_GdcCatalogTable [];

	struct cs_GdcCatalogTable_ *rtnValue;
	struct cs_GdcCatalogTable_ *tblPtr;

	rtnValue = 0;
	for (tblPtr = cs_GdcCatalogTable;tblPtr->ident != gdcFileNone;tblPtr += 1)
	{
		if (tblPtr->ident == ident)
		{
			rtnValue = tblPtr;
			break;
		}
	}
	return rtnValue;
}
/******************************************************************************
	Constructor
	The argument MUST be a full path name.  This function expects to extract
	the directory of the catalog file from the base.
*/
/*lint -esym(429,__This)    pointer is not freed */
/* Verified that it is indeed free'ed in the event of an error.  I suspect the
   rather elaborate macro in CS_free has confused PC-Lint. */
struct csDatumCatalog_* CSnewDatumCatalog (Const char* pathName)
{
	extern char cs_DirsepC;
	extern char csErrnam [];

	short relative;
	ulong32_t flags;
	int room;
	int quote;
	size_t length;
	long32_t bufferSize;

	char *cp, *cpt;
	char *comment;
	csFILE* catFstr;
	struct csDatumCatalog_ *__This;
	struct csDatumCatalogEntry_ *findPtr;
	struct csDatumCatalogEntry_ *entryPtr;

	double density;

	char relString [4];
	char relStringPar [4];
	char ctemp [csMAXPATH];
	char lineBufr [csMAXPATH + 64];
	char filePath [csMAXPATH];

	catFstr = NULL;
	__This = NULL;
	entryPtr = NULL;
	comment = NULL;

	/* The following initializations are kind of hokey, but we want to
	   reference the cs_DirsepC variable.  ALternative is to use something
	   like sprintf.  That isn't very esthetic either. */
	relString [0] = '.';
	relString [1] = cs_DirsepC;
	relString [2] = '\0';
	relString [3] = '\0';

	relStringPar [0] = '.';
	relStringPar [1] = '.';
	relStringPar [2] = cs_DirsepC;
	relStringPar [3] = '\0';

	/* Create a new DatumCatalog object. */
	__This = (struct csDatumCatalog_ *)CS_malc (sizeof (struct csDatumCatalog_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	__This->fileFolder [0] = '\0';
	__This->fallback [0] = '\0';
	__This->listHead = NULL;
	__This->initialComment = 0;
	__This->middleComment = 0;
	__This->trailingComment = 0;

	/* Open up the file. */
	CS_stncp (ctemp,pathName,sizeof (ctemp));
	CSrplDirSep (ctemp);
	catFstr = CS_fopen (ctemp,_STRM_TXTRD);
	if (catFstr == NULL)
	{
		CS_stncp (csErrnam,ctemp,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* Extract the directory.  If this fails, we have a hard error. */
	cp = strrchr (ctemp,cs_DirsepC);
	if (cp == NULL)
	{
		CS_stncp (csErrnam,pathName,MAXPATH);
		CS_erpt (cs_DTC_CATFILE);
		goto error;
	}
	*cp = '\0';
	CS_stncp (__This->fileFolder,ctemp,sizeof (__This->fileFolder));

	/* Process each line in the file; extract a catalog entry for each. */
	while (CS_fgets (lineBufr,sizeof (lineBufr),catFstr) != NULL)
	{
		/* Always have to check for errors. */
		if (CS_ferror (catFstr))
		{
			CS_stncp (csErrnam,pathName,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}

		/* Make sure it's null terminated. */
		lineBufr [sizeof (lineBufr) - 1] = '\0';

		/* Trim white space on the front, and end. */
		CS_trim (lineBufr);

		/* Ignore comment lines, empty lines. */
		if (lineBufr [0] == '\0')
		{
			continue;
		}
		if (lineBufr [0] == '#')
		{
			/* If we have already captured three comments, we
			   can stop screwing around with them. */
			if (__This->trailingComment != 0) continue;

			/* If its a whole line comment, tack a new line back
			   on to the end. */
			strcat (lineBufr,"\n");

			/* Start/Continue the comment capture. */
			if (comment == 0)
			{
				comment = (char *)CS_malc (4096);
				*comment = '\0';
			}
			length = strlen (comment) + strlen (lineBufr);
			if (length < 4095)
			{
				strcat (comment,lineBufr);
			}
			continue;
		}

		/* Deal with any accumulated comment. */
		if (comment != NULL)
		{
			length = strlen (comment);
			comment = (char *)CS_ralc (comment,length + 1);

			if      (__This->initialComment  == 0) __This->initialComment = comment;
			else if (__This->middleComment   == 0) __This->middleComment = comment;
			else if (__This->trailingComment == 0) __This->trailingComment = comment;
			else
			{
				CS_free (comment);
			}
			comment = NULL;
		}

		/* Trim, comments at the end of the line. */
		quote = FALSE;
		for (cp = lineBufr;*cp != '\0';cp += 1)
		{
			if (*cp == '"') quote = !quote;
			if (quote) continue;
			if (*cp == '#') break;
		}
		if (*cp == '#')
		{
			*cp = '\0';
			CS_trim (lineBufr);
		}

		/* See if this is a fallback definition.  They can appear anywhere,
		   we are not case sensitive.  If there are more than one, we
		   honor only the last one that we see. */
		if (CS_strnicmp (lineBufr,"fallback",8) == 0)
		{
			/* This is a fallback definition. */
			cp = lineBufr + 8;
			while (isspace (*cp)) cp += 1;
			if (*cp == '=' || *cp == ',')
			{
				cp += 1;
				while (isspace (*cp)) cp += 1;
			}
			CS_stncp (__This->fallback,cp,sizeof (__This->fallback));
			continue;
		}

		/* Parse the file name and the buffer size from the file. */
		cpt = ctemp;
		quote = FALSE;
		for (cp = lineBufr;*cp != '\0';cp += 1)			/*lint !e850   loop variable (cp) is modified within the loop body */
		{
			if (*cp == '"')
			{
				if (quote)
				{
					if (*(cp + 1) == '"')
					{
						*cpt++ = '"';
						cp += 1;
						continue;
					}
					else
					{
						quote = FALSE;
					}
				}
				else
				{
					quote = TRUE;
				}
			}
			else if (!quote && *cp == ',')
			{
				break;
			}
			else
			{
				*cpt++ = *cp;
			}
		}
		*cpt = '\0';						/*lint !e850   loop variable (cp) is modified within the loop body ~~*/

		/* Parse the buffer size, if its there. */
		bufferSize = 0L;
		if (*cp != '\0')
		{
			cp += 1;
			bufferSize = CS_strtol (cp,NULL,0);

			/* Skip past the buffer size. */
			while (*cp != '\0' && *cp != ',') cp += 1;
			if (*cp == ',') cp += 1;
		}

		/* Parse the flags argument if there.  Note, this can be
		   decimal or hexadecimal. */
		flags = 0UL;
		if (*cp != '\0')
		{
			flags = CS_strtoul (cp,NULL,0);

			/* Skip past the flags. */
			while (*cp != '\0' && *cp != ',') cp += 1;
			if (*cp == ',') cp += 1;
		}

		/* Parse the density argument if there. */
		density = 0.0;
		if (*cp != '\0')
		{
			density = CS_strtod (cp,NULL);
			if (density < 0.5 || density > 60.0)
			{
				goto error;
			}
			density /= 60.0;

			/* Skip past the flags. */
			while (*cp != '\0' && *cp != ',') cp += 1;
			if (*cp == ',') cp += 1;
		}

		/* Standardize the directory separator character. */
		CSrplDirSep (ctemp);

		/* Do the relative bit. */
		if (!strncmp (ctemp,relString,strlen (relString)))
		{
			/* It's a relative entry; relative to the directory of the
			   catalog file.  Convert to an absolute path. */
			relative = 1;
			cp = CS_stncp (filePath,__This->fileFolder,sizeof (filePath));
			room = (int)sizeof (filePath) - (int)strlen (filePath) - 1;
			*cp++ = cs_DirsepC;
			CS_stncp (cp,&ctemp [strlen (relString)],room);
		}
		else if (0==strncmp (ctemp,relStringPar,strlen(relStringPar)))
		{
			/* It's a relative entry; relative to the parent of the
			   directory in which the catalog file resides.  Convert
			   to absolute by leaving the .. reference in the middle.
			   This is much easier and less error prone than trying to
			   back out a directory from the catalog path. */
			relative = 2;
			cp = CS_stncp(filePath,__This->fileFolder,sizeof(filePath));
			room = (int)sizeof(filePath) - (int)strlen(filePath) - 1;
			*cp++ = cs_DirsepC;
			CS_stncp(cp,ctemp,room);
		}
		else
		{
			/* Its a full path name. */
			relative = 0;
			CS_stncp (filePath,ctemp,sizeof (filePath));
		}

		/* Add a catalog entry to the catalog list. */
		entryPtr = CSnewDatumCatalogEntry (filePath,relative,bufferSize,flags,density);
		if (entryPtr == NULL)
		{
			goto error;
		}
		/* Keep the list in the same order as they appear in the file. */
		if (__This->listHead == NULL)
		{
			__This->listHead = entryPtr;
		}
		else
		{
			for (findPtr = __This->listHead;findPtr->next != NULL;findPtr = findPtr->next);
			findPtr->next = entryPtr;
		}
	}
	CS_fclose (catFstr);

	/* Handle any comment tacked on the end. */
	if (comment != NULL)
	{
		length = strlen (comment);
		comment = (char *)CS_ralc (comment,length + 1);
		if      (__This->initialComment  == 0) __This->initialComment = comment;
		else if (__This->middleComment   == 0) __This->middleComment = comment;
		else if (__This->trailingComment == 0) __This->trailingComment = comment;
		else
		{
			CS_free (comment);
		}
	}

	return __This;

error:
	if (catFstr != NULL) CS_fclose (catFstr);
	if (__This != NULL)
	{
		while (__This->listHead != NULL)
		{
			entryPtr = __This->listHead;
			__This->listHead = __This->listHead->next;
			CS_free (entryPtr);
		}
		if (__This->initialComment  != 0) CS_free (__This->initialComment);
		if (__This->middleComment   != 0) CS_free (__This->middleComment);
		if (__This->trailingComment != 0) CS_free (__This->trailingComment);
		CS_free (__This);
	}
	return NULL;
}
/*lint +esym(429,__This) */

/******************************************************************************
	Destructor
*/
void CSdeleteDatumCatalog (struct csDatumCatalog_ *__This)
{
	struct csDatumCatalogEntry_* entryPtr;

	if (__This != NULL)
	{
		if (__This->initialComment  != 0) CS_free (__This->initialComment);
		if (__This->middleComment   != 0) CS_free (__This->middleComment);
		if (__This->trailingComment != 0) CS_free (__This->trailingComment);
		while (__This->listHead != NULL)
		{
			entryPtr = __This->listHead;
			__This->listHead = __This->listHead->next;
			CSdeleteDatumCatalogEntry (entryPtr);
		}
		CS_free (__This);
	}
}
/******************************************************************************
	Write a datum catalog file.  Provided for possible use in a editor of some
	sort.  USe of this function assures some sort of independence from the
	actual file format.
*/
int CSwriteDatumCatalog (struct csDatumCatalog_ *__This,Const char *path)
{
	extern char cs_DirsepC;
	extern char csErrnam [];

	char *cp;
	FILE *catFstr;
	struct csDatumCatalogEntry_* entryPtr;
	char newDir [csMAXPATH];
	char baseDir [csMAXPATH];

	/* Prepare for an error. */
	catFstr = NULL;

	/* Standardize */
	CS_stncp (newDir,path,sizeof (newDir));
	CSrplDirSep (newDir);
	CS_stncp (baseDir,path,sizeof (baseDir));
	CSrplDirSep (baseDir);
	cp = strrchr (baseDir,cs_DirsepC);
	if (cp != NULL) *cp = '\0';
	else baseDir [0] = '\0';

	/* Open/create the new file. */
	catFstr = CS_fopen (newDir,_STRM_TXTWR);
	if (catFstr == NULL)
	{
		CS_stncp (csErrnam,newDir,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* Write the initial comment if there is one. */
	if (__This->initialComment != 0)
	{
		CS_fputs (__This->initialComment,catFstr);
	}	

	/* Write each entry. */
	for (entryPtr = __This->listHead;entryPtr != NULL;entryPtr = entryPtr->next)
	{
		CSwriteDatumCatalogEntry (entryPtr,catFstr,baseDir);
		if (ferror (catFstr))
		{
			CS_stncp (csErrnam,newDir,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}
	}

	/* Write the middle comment if there is one. */
	if (__This->middleComment != 0)
	{
		CS_fputs (__This->middleComment,catFstr);
	}	

	/* Write the name of the fallback if there is one. */
	if (__This->fallback [0] != '\0')
	{
		CS_fprintf (catFstr,"Fallback = %s\n",__This->fallback);
	}	

	/* Write the trailing comment if there is one. */
	if (__This->trailingComment != 0)
	{
		CS_fputs (__This->trailingComment,catFstr);
	}	

	CS_fclose (catFstr);
	return 0;

error:
	if (catFstr != NULL) CS_fclose (catFstr);
	return -1;
}
/******************************************************************************
	Returns a struct cs_DtcXform_ pointer which can be used to perform the
	fallback calculation for this datum.  NULL pointer is returned if no
	fallback has been specified.  If, upon return, cs_Error is not zero,
	there was an error establishing the fallback position.
*/
Const char *CSgetFallbackName (struct csDatumCatalog_ *__This)
{
	return __This->fallback;
}
/* The following are provided to support an interactive editor
   of Datum Catalof text files.  The return values are:

	+1 = COmpleted, entry actually moved.
	 0 = No error, move not required, so no change.
	-1 = some sort of error, i.e. bogus index value.

	In the case of an error, the entry list remains unchanged. */
int CSaddEntryDataumCatalog (struct csDatumCatalog_ *__This,struct csDatumCatalogEntry_ *entPtr)
{
	int rtnVal = -1;
	struct csDatumCatalogEntry_ *prvPtr;

	entPtr->next = NULL;
	if (__This != NULL)
	{
		rtnVal = 0;
		if (__This->listHead == NULL)
		{
			__This->listHead = entPtr;
			rtnVal = 1;
		}
		else
		{
			for (prvPtr = __This->listHead;prvPtr->next != NULL;prvPtr = prvPtr->next);
			if (prvPtr != NULL)
			{
				prvPtr->next = entPtr;
				rtnVal = 1;
			}
		}
	}
	return rtnVal;	
}
int CSdeleteEntryDatumCatalog (struct csDatumCatalog_ *__This,size_t index)
{
	int rtnVal = -1;
	struct csDatumCatalogEntry_ *entPtr;
	struct csDatumCatalogEntry_ *prvPtr;

	/* Get a pointer to the entry which is to be deleted. */
	for (entPtr = __This->listHead;index-- > 0 && entPtr != NULL;entPtr = entPtr->next);

	/* Return if index was bogus. */
	if (entPtr == NULL) return rtnVal;

	/* Remove entPtr from the list. */
	if (__This->listHead == entPtr)
	{
		__This->listHead = entPtr->next;
		rtnVal = 1;
	}
	else
	{
		rtnVal = 0;
		for (prvPtr = __This->listHead;prvPtr != NULL;prvPtr = prvPtr->next)
		{
			if (prvPtr->next == entPtr)
			{
				prvPtr->next = prvPtr->next->next;
				CSdeleteDatumCatalogEntry (entPtr);
				rtnVal = 1;
				break;
			}
		}
	}
	return rtnVal;
}
int CSmoveUpDatumCatalog (struct csDatumCatalog_ *__This,size_t index)
{
	int rtnVal = -1;

	struct csDatumCatalogEntry_ *entPtr;
	struct csDatumCatalogEntry_ *prvPtr;

	/* Get a pointer to the entry which is to be moved. */
	for (entPtr = __This->listHead;index-- > 0 && entPtr != NULL;entPtr = entPtr->next);

	/* If index is bogus, do nothing. */
	if (entPtr == NULL) return rtnVal;

	/* If it's already at the top, nothing to do. */
	if (entPtr == __This->listHead) return 0;

	/* EntPtr now points to the entry which is to be "moved up". */
	if (__This->listHead->next == entPtr)
	{
		/* Special case where the entry is to become the first one. */
		prvPtr = __This->listHead;
		prvPtr->next = entPtr->next;
		entPtr->next = prvPtr;
		__This->listHead = entPtr;
		rtnVal = 1;
	}
	else
	{
		for (prvPtr = __This->listHead;prvPtr != NULL;prvPtr = prvPtr->next)
		{
			if (prvPtr->next->next == entPtr)
			{
				/* Take entPtr out pf the list. */
				prvPtr->next->next = entPtr->next;

				/* Put it back in. */
				entPtr->next = prvPtr->next;
				prvPtr->next = entPtr;
				rtnVal = 1;
				break;
			}
		}
	}
	return rtnVal;
}
int CSmoveDownDatumCatalog (struct csDatumCatalog_ *__This,size_t index)
{
	int rtnVal = -1;

	struct csDatumCatalogEntry_ *entPtr;
	struct csDatumCatalogEntry_ *prvPtr;

	/* Get a pointer to the entry which is to be moved. */
	for (entPtr = __This->listHead;index-- > 0 && entPtr != NULL;entPtr = entPtr->next);

	/* If index is bogus, do nothing. */
	if (entPtr == NULL) return rtnVal;

	/* If it's already at the bottom, nothing to do. */
	if (entPtr->next == NULL) return 0;

	/* EntPtr now points to an entry which does need to be moved. */
	if (__This->listHead == entPtr)
	{
		/* Take entPtr out of the list. */
		__This->listHead = entPtr->next;

		/* Put it back in. */
		entPtr->next = __This->listHead->next;
		__This->listHead->next = entPtr;
		rtnVal = 1;
	}
	else
	{
		for (prvPtr = __This->listHead;prvPtr != NULL;prvPtr = prvPtr->next)
		{
			if (prvPtr->next == entPtr)
			{
				/* Take entPtr out of the list. */
				prvPtr->next = entPtr->next;

				/* Put it back in. */
				prvPtr = prvPtr->next;
				entPtr->next = prvPtr->next;
				prvPtr->next = entPtr;
				rtnVal = 1;
				break;
			}
		}
	}
	return rtnVal;
}
int CSmakeFirstDatumCatalog (struct csDatumCatalog_ *__This,size_t index)
{
	int rtnVal = -1;

	struct csDatumCatalogEntry_* entPtr;
	struct csDatumCatalogEntry_* prvPtr;

	/* Get a pointer to the entry which is to be moved. */
	for (entPtr = __This->listHead;index-- > 0 && entPtr != NULL;entPtr = entPtr->next);

	/* If index is bogus, do nothing. */
	if (entPtr == NULL) return -1;

	/* If it's already at the top, nothing to do. */
	if (__This->listHead == entPtr) return 0;

	/* Need to do some moveing.  We need to take entPtr put of the list,
	   where ever it is. */
	for (prvPtr = __This->listHead;prvPtr != NULL;prvPtr = prvPtr->next)
	{
		if (prvPtr->next == entPtr) break;
	}
	if (prvPtr != NULL)
	{
		prvPtr->next = entPtr->next;

		/* We now make entPtr the first one in the list. */
		entPtr->next = __This->listHead;
		__This->listHead = entPtr;
		rtnVal = 1;
	}
	return rtnVal;
}
int CSmakeLastDatumCatalog (struct csDatumCatalog_ *__This,size_t index)
{
	int rtnVal = -1;

	struct csDatumCatalogEntry_* entPtr;
	struct csDatumCatalogEntry_* prvPtr;

	/* Get a pointer to the entry which is to be moved. */
	for (entPtr = __This->listHead;index-- > 0 && entPtr != NULL;entPtr = entPtr->next);

	/* If index is bogus, do nothing. */
	if (entPtr == NULL) return -1;

	/* If it's already the last, nothing to do. */
	if (entPtr->next == NULL) return 0;

	/* Take this entry out of the list. */
	if (__This->listHead == entPtr)
	{
		__This->listHead = entPtr->next;
	}
	else
	{
		for (prvPtr = __This->listHead;prvPtr != NULL;prvPtr = prvPtr->next)
		{
			if (prvPtr->next == entPtr)
			{
				prvPtr->next = entPtr->next;
				break;
			}
		}
	}

	/* Now, we put entPtr back in. */
	for (prvPtr = __This->listHead;prvPtr->next != NULL;prvPtr = prvPtr->next);
	if (prvPtr != NULL)
	{
		prvPtr->next = entPtr;
		entPtr->next = NULL;
		rtnVal = 1;
	}
	return rtnVal;
}
/******************************************************************************
	Returns a specific datum catalog entry given an index value.  Use this
	to sequence through a datum catalog, thus providing some independence from
	future revisions.
*/
struct csDatumCatalogEntry_* CSgetDatumCatalogEntry (struct csDatumCatalog_ *__This,int index)
{
	struct csDatumCatalogEntry_* entryPtr;

	entryPtr = __This->listHead;
	while (index-- > 0)
	{
		if (entryPtr == NULL) break;
		entryPtr = entryPtr->next;
	}
	return entryPtr;
}
/******************************************************************************
	Datum catalog 'Entry' constructor.
*/
struct csDatumCatalogEntry_* CSnewDatumCatalogEntry (Const char* path,short relative,long32_t bufferSize,ulong32_t flags,double density)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	const char *cp;
	struct csDatumCatalogEntry_* __This;

	/* Prepare for an error. */
	__This = NULL;

	/* It is an error to create an entry which does not have an extension or
	   at least one directory separator character. */

	cp = strrchr (path,cs_DirsepC);
	if (cp == NULL)
	{
		CS_stncp (csErrnam,path,MAXPATH);
		CS_erpt (cs_DTC_PATH);
		goto error;
	}
	cp = strchr (cp,cs_ExtsepC);
	if (cp == NULL || *(cp + 1) == '\0')
	{
		CS_stncp (csErrnam,path,MAXPATH);
		CS_erpt (cs_DTC_EXT);
		goto error;
	}

	/* Allocate an object. */
	__This = (struct csDatumCatalogEntry_*) CS_malc (sizeof (struct csDatumCatalogEntry_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	__This->next = NULL;
	__This->relative = 0;

	/* Populate the object. */
	CS_stncp (__This->pathName,path,sizeof (__This->pathName));
	__This->bufferSize = bufferSize;
	__This->flags = flags;
	__This->density = density;
	__This->relative = relative;
	return __This;

error:
	if (__This != NULL)						/*lint !e774 */
	{
		CS_free (__This);
	}
	return NULL;
}
/******************************************************************************
	'Entry' Destructor
*/
void CSdeleteDatumCatalogEntry (struct csDatumCatalogEntry_* __This)
{
	if (__This != NULL)
	{
		CS_free (__This);
	}
	return;
}
/******************************************************************************
	Writes a specific catalog entry to the indicated stream.  Use this to
	retain independence from changes in file format.  Note, baseDir is used
	to determine if a relative reference can be used.  Absolute says don't
	use a relative reference ever.
*/
void CSwriteDatumCatalogEntry (struct csDatumCatalogEntry_* __This,FILE *fstr,Const char *baseDir)
{
	extern char cs_DirsepC;

	size_t length;
	char *cp;
	char extra [64];
	char pathOne [MAXPATH];
	char pathTwo [MAXPATH];
	char relPath [MAXPATH + 32];
	char pathPart [MAXPATH + 32];
	 
	/* Build the extra stuff as necessary. */
	extra [0] = '\0';
	if (__This->bufferSize != 0 || __This->flags != 0 || __This->density != 0.0)
	{
		/* There is some "extra" stuff.  We put it together, but not more than
		   we absolutely need. */
		cp = extra;
		if (__This->bufferSize != 0)
		{
			cp += CS_sprintf (cp,"%d",(int)__This->bufferSize);
		}
		if (__This->flags != 0 || __This->density != 0.0)
		{
			*cp++ = ',';
			if (__This->flags != 0)
			{
				cp += CS_sprintf (cp,"%#x",(unsigned)__This->flags);
			}
			if (__This->density != 0.0)
			{
				*cp++ = ',';
				cp += CS_sprintf (cp,"%g",__This->density * 60.0);
			}
		}
		*cp = '\0';
	}

	/* See if we can do the relative thing.  Note, if the indicated relative business
	   doesn't work, we force it to absolute. */
	CS_stncp (relPath,__This->pathName,sizeof (pathPart));
	if (__This->relative != 0)
	{
		__This->relative = 0;
		CS_stncp (pathOne,baseDir,sizeof (pathOne));
		CS_stncp (pathTwo,baseDir,sizeof (pathTwo));

		cp = strrchr (pathTwo,cs_DirsepC);
		if (cp == NULL) cp = pathTwo;
		*cp = '\0';

		length = strlen (pathOne);
		if (!CS_strnicmp (pathOne,relPath,length))
		{
			cp = relPath + length + 1;
			CS_stncp (relPath,cp,sizeof (relPath));
			__This->relative = 1;
		}
		else
		{
			length = strlen (pathTwo);
			if (!CS_strnicmp (pathTwo,relPath,length))
			{
				cp = relPath + length + 1;
				CS_stncp (relPath,cp,sizeof (relPath));
				__This->relative = 2;
			}
		}
	}

	/* Prepare the output string.  Note, we rely on cp being set as above. */
	if (__This->relative == 1)
	{
		/* See if we need quotes. */
		if (strchr (relPath,' ') != NULL)
		{
			CS_sprintf (pathPart,".%c\"%s\"",cs_DirsepC,relPath);
		}
		else
		{
			CS_sprintf (pathPart,".%c%s",cs_DirsepC,relPath);
		}
	}
	else if (__This->relative == 2)
	{
		/* See if we need quotes. */
		if (strchr (relPath,' ') != NULL)
		{
			CS_sprintf (pathPart,"..%c\"%s\"",cs_DirsepC,relPath);
		}
		else
		{
			CS_sprintf (pathPart,"..%c%s",cs_DirsepC,relPath);
		}
	}
	else
	{
		__This->relative = 0;
		cp = __This->pathName;
		if (strchr (cp,' ') != NULL)
		{
			CS_sprintf (pathPart,"\"%s\"",relPath);
		}
		else
		{
			CS_sprintf (pathPart,"%s",relPath);
		}
	}

	/* Print the combined result. */
	if (extra [0] != '\0')
	{
		CS_fprintf (fstr,"%s,%s\n",pathPart,extra);
	}
	else
	{
		CS_fprintf (fstr,"%s\n",pathPart);
	}
	/* Calling function is responsible for testing ferror on stream. */
	return;
}
