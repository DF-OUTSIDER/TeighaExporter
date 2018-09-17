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
#include <ctype.h>

/**********************************************************************
**	flag = CS_altdr (alt_dir);
**
**	char *alt_dir;				the alternate directory to use for mapping
**								stuff if viable.  NULL says use the environmental
**								variable named CS_MAP_DIR.  The null string says
**								check the current directory.
**	int flag;					returns TRUE if cs_Dir contains a valid
**								pointer to a directory containing a
**								COORDSYS file.
**
**	This module will alter the contents of cs_Dir and cs_DirP
**	if the proposed alternate directory is a viable directory
**	for mapping.  If the proposed directory is not viable,
**	we use the default standard defined in CSdata.c.
**
**	The return value simply indicates the presence of a file
**	with the appropriate name.  This function does not test
**	the magic number of the file.
**********************************************************************/

int EXP_LVL1 CS_altdr (Const char *alt_dir)
{
	extern char cs_DirK [];
	extern char cs_Dir [];
	extern char *cs_DirP;

	extern char cs_Csname [];
	extern char cs_Envvar [];
	extern char cs_DirsepC;

	int st;
	int flag;

	char *cp;

	char ctemp [MAXPATH];

	flag = FALSE;

	/* Deal with the possible NULL value of cs_DirP. */

	if (cs_Dir [0] == '\0')
	{
		cs_DirP = CS_stcpy (cs_Dir,cs_DirK);
	}
	if (cs_DirP == NULL)
	{
		cp = strrchr (cs_Dir,cs_DirsepC);
		if (cp != NULL) cs_DirP = cp + 1;
		else            cs_DirP = cs_Dir;
	}

	/* See if the Coordinate System Dictionary exists in the
	   proposed alternate directory. If alt_dir is the NULL
	   pointer, we use the value of the environmental variable
	   whose name is defined in the cs_Envvar global variable. */
	if (alt_dir == NULL)
	{
		cp = CS_getenv (cs_Envvar);
		if (cp != NULL)
		{
			cp = CS_stncp (ctemp,cp,sizeof (ctemp));
			if (*(cp - 1) != cs_DirsepC)
			{
				*cp++ = cs_DirsepC;
				*cp = '\0';
			}
		}
		else
		{
			ctemp [0] = '\0';
			cp = ctemp;
		}
	}
	else if (*alt_dir != '\0')
	{
		/* If we have been given something, use it. Note,
		   we add a directory separator character if there
		   is not one already. */

		strcpy (ctemp,alt_dir);
		cp = ctemp + strlen (ctemp);
		if (*(cp - 1) != cs_DirsepC)
		{
			*cp++ = cs_DirsepC;
			*cp = '\0';							/*lint !e661 */
		}
	}
	else
	{
		/* All else has failed. However, we must initialize to
		   something rational. We'll use the current directory
		   of the current drive. */

		ctemp [0] = '.';
		ctemp [1] = cs_DirsepC;
		ctemp [2] = '\0';
		cp = &ctemp [2];
	}

	/* See if the coordinate system dictionary resides in the
	   desired location.  Note, we only use the name here, we
	   don't check the magic number.  Maybe we should. */

	strcpy (cp,cs_Csname);
	st = CS_access (ctemp,0);
	*cp = '\0';
	if (st == 0)
	{
		/* Here if a Coordinate System Dictionary exists
		   in the proposed alternate directory.  Therefore,
		   we use it. */

		cs_DirP = CS_stcpy (cs_Dir,ctemp);
		flag = TRUE;
	}
	return (flag ? 0 : -1);
}

/**********************************************************************
**	int result = CS_usrdr (usr_dir);
**
**	char *usr_dir;				the directory where to store all custom definitions in.
**								Note, that this directory isn't required to contain
**								any CSD files (yet).
**								Must be less than (MAXPATH - 1) characters.
**								If NULL or empty, the user dictionary path will be unset.
**								That is, all user defined definitions will be written
**								into the CSD files found in the directory specified
**								in [cs_dir].
**
**	result						0 if successfull, -1 otherwise
**
**	Note, that this method can only be called, if cs_Protect is >= 0. That is,
**	the protection scheme for 'system' definitions must be somehow enabled.
**********************************************************************/
int EXP_LVL1 CS_usrdr (Const char *usr_dir)
{
	extern char cs_DirsepC;
	extern short cs_Protect;

	extern char *cs_CsKeyNames;
	extern char *cs_DtKeyNames;
	extern char *cs_ElKeyNames;

	extern char cs_UserDir [];

	char* pathEnd = NULL;

	if (cs_Protect < 0)
		return -1;

	if (NULL != usr_dir && strlen(usr_dir) >= MAXPATH)
		goto arg_error;

	//zero out everything & let [cs_UserDirP] point to '\0'
	memset(cs_UserDir, '\0', MAXPATH * sizeof(char));
	if (NULL == usr_dir || '\0' == *usr_dir)
		goto exit;

	//copy the new user directory into our [cs_UserDir] variable
	//cs_UserDirP now points to the end of the string
	pathEnd = CS_stcpy(cs_UserDir, usr_dir);

	//add the dir-separator char, if it's not there already
	if (*(pathEnd - 1) != cs_DirsepC)
	{
		*pathEnd++ = cs_DirsepC;
		*pathEnd = '\0';//make sure, we've only 1 dir separator char
	}

exit:
	//now that a new directory has been setup, we've to cleanup all of our internal "definition" caches
	CS_free(cs_CsKeyNames);
	cs_CsKeyNames = NULL;
	CS_free(cs_DtKeyNames);
	cs_DtKeyNames = NULL;
	CS_free(cs_ElKeyNames);
	cs_ElKeyNames = NULL;

	CSrlsCategories();

	return 0;

arg_error:

	CS_erpt(cs_INV_ARG1);
	return -1;
}

/**********************************************************************
**	st = CS_nampp (name);
**
**	char *name;					the name to be processed and checked.
**	int st;						returns zero if the name is OK, else
**								returns -1.
**
**	Converts all lower case characters to upper case, and
**	trims leading and traling blanks.  Also trims default
**	wrapping characters from the name if present.
**********************************************************************/

int EXP_LVL5 CS_nampp (char *name)
{
	int st;
	st = CSnampp (name,cs_KEYNM_DEF);
	return st;
}
int EXP_LVL5 CS_nampp64 (char *name)
{
	int st;
	st = CSnampp (name,64);
	return st;
}

int EXP_LVL7 CSnampp (char *name,size_t nameSize)
{
	extern char csErrnam [];
	extern char cs_Nmchset [];
	extern char cs_Unique;

	int ok;
	size_t size;
	int alphaCount;
	int uniqueCount;

	cs_Register char *pp;
	cs_Register char *qq;

	cs_Register char cc;

	char ctemp [64];

	/* Prepare for a possible error. */
	CS_stncp (csErrnam,name,MAXPATH);

	/* Capture the size of the original array.  If the name
	   is OK, we will be copying back into this array a string
	   which may be smaller, but certainly not larger than
	   the original. */
	size = strlen (name);						/*lint !e713 */

	/* Remove leading and trailing white space while copying
	   name to ctemp. */
	pp = name;
	while (*pp == ' ') pp += 1;
	qq = CS_stncp (ctemp,pp,sizeof (ctemp));
	if (ctemp [0] == '\0')
	{
		/* We were given all blanks. */
		CS_erpt (cs_INV_NAME);
		goto error;
	}

	/* We now know that there is at least one non-space
	   character in the string.  Thus the following does
	   not need a pointer comparison which we like to avoid. */
	while (*(qq - 1) == ' ') qq -= 1;
	*qq = '\0';

	/* If the name is surrounded by either set of default
	   characters, remove them. */
	pp = ctemp;
	qq = ctemp + strlen (pp) - 1;
	if ((*pp == cs_DFLT_IDNTBEG && *qq == cs_DFLT_IDNTEND) ||
		(*pp == cs_DFLT_REPLBEG && *qq == cs_DFLT_REPLEND)
	   )
	{
		*qq = '\0';						/* trims trailing */
		CS_stcpy (ctemp,&ctemp [1]);	/* trims leading */
	}

	/* Careful, ctemp could be the null string now if we were
	   given a set of empty default wrappers. */
	if (*pp == '\0')
	{
		/* We were given all blanks. */
		CS_erpt (cs_INV_NAME);
		goto error;
	}

	/* Make sure all characters are legal.  pp should still
	   point to ctemp, and there must be at least one character
	   in the array.
	   
	   Note, we no longer force the name to upper case. However,
	   all comparisions remain case insensitive. */
	uniqueCount = alphaCount = 0;
	while (*pp != '\0')
	{
		cc = *pp++;
		if (cc == cs_Unique)
		{
			uniqueCount += 1;	
			continue;
		}
		if (cc >= '0' && cc <= '9') continue;
		if ((cc >= 'A' && cc <= 'Z') || (cc >= 'a' && cc <= 'z'))
		{
			alphaCount += 1;
			continue;
		}
		if (strchr (cs_Nmchset,cc) == NULL)
		{
			CS_erpt (cs_INV_NAME);
			goto error;
		}

		/* If cc == ' ' and we are still here, then spaces are
		   allowed in key names (i.e. space is a member of the set
		   defined by cs_Nmchset).  Check for a double space in the
		   name, a condition which we don't allow in any case.  We
		   have a specific error message for that condition. */
		if (cc == ' ' && *pp == ' ')
		{
			CS_erpt (cs_DBL_SPACE);
			goto error;
		}
	}
	if (uniqueCount > 1)
	{
		/* If the cs_Unique feature is enabled, we only allow one
		   of them. */
		CS_erpt (cs_INV_NAME);
		goto error;
	}
	if (alphaCount == 0)
	{
		/* Must have at least one alphabetic in the name. */
		CS_erpt (cs_INV_NAME);
		goto error;
	}

	/* If the result is longer than cs_KEYNM_MAX characters we
	   have an illegal name. */
	size = strlen (ctemp);						/*lint !e713 */
	if (size >= nameSize)
	{
		CS_erpt (cs_INV_NAME);
		goto error;
	}

	/* We used to require that a key name begin with an alphabetic (prior
	   to release 9.01).  We now allow key names to begin with numerics
	   providing that the first non-numeric character is indeed alphabetic.
	   For the purposes of this particular test, the underscore character
	   is considered alphabetic. */
	pp = ctemp;
	if (*pp == cs_Unique) pp += 1;
	while ((cc = *pp++) != '\0') if (cc < '0' || cc > '9') break;
	ok = (cc == '_')  || (cc == cs_Unique) || (cc >= 'A' && cc <= 'Z') || (cc >= 'a' && cc <= 'z');
	if (!ok)
	{
		CS_erpt (cs_INV_NAME);
		goto error;
	}

	/* OK, the name has been processed, and is OK. */
	CS_stncp (name,ctemp,(int)nameSize);
	return (0);

error:
	/* Return error status without modifying name at all. */
	return (-1);
}

/**********************************************************************
**	scale = CS_un2c (name);
**
**	char *name;					the name of a unit of length as a null
**								terminated string.
**	double scale;				returns the scale required to convert a value
**								in the named unit to meters by multiplication,
**								else zero.
**
**	Previously, only length units were supported and this
**	function peformed the search of the single unit table,
**	cs_Unittab and returned the appropriate value.  Now,
**	multiple types of units are supported.  To remain
**	compatible with existing code, this function performs
**	a length unit look up.  New code should use the
**	CS_unitlu function.  Eventually, we would like to do
**	away with this function.
**********************************************************************/

double EXP_LVL9 CS_un2d (Const char *name)

{
	return (CS_unitlu (cs_UTYP_LEN,name));
}

/**********************************************************************
**	CS_quadF (xy,xx,yy,x_off,y_off,quad);
**	CS_quadI (&xx,&yy,xy,x_off,y_off,quad);
**
**	double xy [2];				the user defined cartesian system XY coordinate.
**	double xx;					the X coordinate in the projection cartesian
**								system.
**	double yy;					the Y coordinate in the projection cartesian
**								system.
**	double x_off;				the X offset, aka False Easting, aka the X
**								coordinate of the False Origin.
**	double y_off;				the Y offset, aka False Northing, aka the Y
**								coordinate of the False Origin.
**	short quad;					a bit map of the operations to be performed.
**
**	The xx [] and the &xx,&yy arguments may point to the same
**	memory locations.
**
**	The quadF form is designed specifically for calling by the
**	forward projection functions; the quadI form for use by the
**	inverse projection functions.
**
**	QuadF returns its results in the xy array.  QuadI returns
**	its results in the xx and yy variables.  Thus, in the
**	quadI form, the xx and yy arguments must be pointers to
**	double.
**
**	The quad argument is a bit map of the functions to be
**	performed.  Zero produces the standard right handed result.
**	If quad is zero, these functions are an expensive way to deal
**	with the false origin.  For performance purposes, and since
**	non-standard cartesian systems are so rare, most all
**	projection functions deal with the false origin on their
**	own if quad is zero.
**
**	The bit map should be defined by oring one or more of the
**	following manifest constants:
**
**	cs_QUAD_SWAP:	swap the X and Y axis.
**	cs_QUAD_INVX:	invert X axis.
**	cs_QUAD_INVY:	invert Y axis.
**
**	In quadF, the axis inversion is applied first.  The false
**	origin is then applied.  Axis swapping is performed last.
**	Obviously, the order is reversed in quadI.
**
**********************************************************************/

void EXP_LVL9 CS_quadF (	double xy [2],
							double xx,
							double yy,
							double x_off,
							double y_off,
							short quad)
{
	if ((quad & cs_QUAD_SWAP) == 0)
	{
		xy [XX] = ((quad & cs_QUAD_INVX) != 0 ? -xx : xx) + x_off;
		xy [YY] = ((quad & cs_QUAD_INVY) != 0 ? -yy : yy) + y_off;
	}
	else
	{
		xy [YY] = ((quad & cs_QUAD_INVX) != 0 ? -xx : xx) + x_off;
		xy [XX] = ((quad & cs_QUAD_INVY) != 0 ? -yy : yy) + y_off;
	}
	return;
}

void EXP_LVL9 CS_quadI (double *xx,
						double *yy,
						Const double xy [2],
						double x_off,
						double y_off,
						short quad)
{
	double my_x;
	double my_y;

	if ((quad & cs_QUAD_INVX) != 0)
	{
		my_x = -(((quad & cs_QUAD_SWAP) == 0 ? xy [XX] : xy [YY]) - x_off);
	}
	else
	{
		my_x =   ((quad & cs_QUAD_SWAP) == 0 ? xy [XX] : xy [YY]) - x_off;
	}

	if ((quad & cs_QUAD_INVY) != 0)
	{
		my_y = -(((quad & cs_QUAD_SWAP) == 0 ? xy [YY] : xy [XX]) - y_off);
	}
	else
	{
		my_y =   ((quad & cs_QUAD_SWAP) == 0 ? xy [YY] : xy [XX]) - y_off;
	}
	*xx = my_x;
	*yy = my_y;
	return;
}

/*
	The following function is used to convert XY min/maxes as
	developed by the Setup functions and:

	1) Apply the approriate quad inversions.
	2) Add the false origin.
	3) Provide for the axis swap.
	4) Assure that the end result is a properly ordered
	   min/max.
*/

void EXP_LVL9 CS_quadMM (	double min_xy [2],
							double max_xy [2],
							double x_off,
							double y_off,
							short quad)
{
	double my_min [2];
	double my_max [2];

	CS_quadF (my_min,min_xy [XX],min_xy [YY],x_off,y_off,quad);
	CS_quadF (my_max,max_xy [XX],max_xy [YY],x_off,y_off,quad);

	min_xy [XX] = (my_min [XX] <= my_max [XX]) ? my_min [XX] : my_max [XX];
	min_xy [YY] = (my_min [YY] <= my_max [YY]) ? my_min [YY] : my_max [YY];

	max_xy [XX] = (my_max [XX] >= my_min [XX]) ? my_max [XX] : my_min [XX];
	max_xy [YY] = (my_max [YY] >= my_min [YY]) ? my_max [YY] : my_min [YY];
	return;
}


/**********************************************************************
**	protected = CS_prchk (prot_val);
**
**	short prot_val;				a protection value extracted from a dictionary
**								definition record.
**	int protected;				returns TRUE if the item would be considered
**								protected by the appropriate update function.
**								Else, returns FALSE.
**
**	In all probability it will never happen, but it is possible that
**	a clock tick between the time you call this function and the
**	time that you call the update function will change the protection
**	status of the dictionary item.  Note, the clock tick may not
**	necessarily be exactly at midnight.
**********************************************************************/

int EXP_LVL1 CS_prchk (short prot_val)
{
	extern short cs_Protect;

	short cs_time;		/* Number of days since Jan 1, 1990 */


	if (cs_Protect < 0) return (FALSE);
	if (prot_val == 1) return (TRUE);
	cs_time = (short)((CS_time ((cs_Time_ *)0) - 630720000L) / 86400L);
	return (prot_val < (cs_time - cs_Protect));
}
/* 
	Extracts a double from a text stream, the double is assumed to be
	surrounded white space.  Leading white space is ignored.

	Return value:
	 0 = normal completion.
	 1 = End of File
	-1 = some sort of error, typically a double value not delimited by
		 white space.
*/
int EXP_LVL5 CSextractDbl (csFILE *aStrm,double* result)
{
	int st;
	int cc;
	int idx;
	enum { initial,
		   extract,
		   evaluate,
		   done
		 } state;
	char* trmPtr;
	char ascii [64];

	st = -1;
	idx = 0;
	ascii [idx] = '\0';
	state = initial;
	while (state != done)
	{
		cc = CS_fgetc (aStrm);
		switch (state) {
		case initial:
			if (cc == EOF)
			{
				st = 1;
				state = done;
			}
			else if (!isspace (cc))
			{
				ascii [idx++] = (char)cc;
				ascii [idx] = '\0';
				state = extract;
			}
			break;

		case extract:
			/* Can't get here unless we have at least one character to
			   evaluate. */
			if (cc == EOF)
			{
				state = evaluate;
			}
			else if (!isspace (cc))
			{
				ascii [idx++] = (char)cc;
				ascii [idx] = '\0';
			}
			else
			{
				state = evaluate;
			}
			break;

		case evaluate:
			*result = strtod (ascii,&trmPtr);
			st = (*trmPtr == '\0') ? 0 : -1;
			state = done;
			break;

		case done:
			state = done;
			break;

		default:
			st = -1;
			break;
		}
	}
	return st;
}

// The following parses a line of text into space separated tokens.  Note
// this function is destructive to the "lineBuffer: argument.
unsigned CS_spaceParse (char *lineBuffer,char *ptrs [],unsigned maxPtrs)
{
	unsigned index = 0;
	char *cp, *cp1;

	if (maxPtrs == 0) return index;
	CS_removeRedundantWhiteSpace (lineBuffer);
	if (lineBuffer [0] == '\0') return index;
	cp = lineBuffer;
	do
	{
		ptrs [index++] = cp;
		cp1 = strchr (cp,' ');
		if (cp1 == 0) break;
		*cp1++ = '\0';
		cp = cp1;
	} while (index < maxPtrs);
	return index;
}

// The following does itsmagic inplace; how nice.
void CS_removeRedundantWhiteSpace (char *string)
{
	char cc;
	char* fromPtr;

	/* Skip over any initial whitespace. */
	fromPtr = string;
	while ((cc = *fromPtr) != '\0' && isspace (cc))
	{
		fromPtr += 1;
	}
	if (*fromPtr == '\0')
	{
		*string = '\0';
		return;
	}

	/* Copy the string into itself, replacing a consecutive white space
	   characters with a single space character. Note, isspace defines
	   what white space is. */
	while ((cc = *fromPtr++) != '\0')
	{
		/* Copy the character, regardless of what it is. */
		*string++ = cc;

		/* If the current character is a space, advance fromPtr to the
		   next non-white space character */		
		if (isspace (cc))
		{
			while ((cc = *fromPtr) != '\0' && isspace (cc))
			{
				fromPtr += 1;
			}
		}
	}
	
	/* The above could easily leave a single white-space character on the
	   end.  The following is safe as we have determined above that the
	   string has at least one non-whitespace character in it. */
	cc = *(string - 1);
	if (isspace (cc))
	{
		string -= 1;
	}
	*string = '\0';
	return;
}
