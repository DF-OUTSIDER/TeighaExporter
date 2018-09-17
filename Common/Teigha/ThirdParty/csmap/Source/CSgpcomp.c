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

/*lint -esym(767,GP_NAME,DESC_NM,SOURCE,SRC_DTM,TRG_DTM,XFORM,GROUP,EPSG_NBR,ACCURACY)  possibly different values in other modules */
/*lint -esym(534,err_func)   ignoring return value */
/*lint -esym(754,cs_GpcmpT_::label)  not referenced directly, only indirectly */

#define GP_NAME  1
#define DESC_NM  2
#define SOURCE   3
#define SRC_DTM  4
#define TRG_DTM  5
#define XFORM 	 6
#define GROUP    7
#define EPSG_NBR 8
#define ACCURACY 9

struct cs_GpcmpT_
{
	char label [32];
	int type;
};

__ALIGNMENT__1		/* For some versions of Sun compiler. */
static struct cs_GpcmpT_ cs_GpcmpT [] =
{
	{"\010GP_NAME:"  ,GP_NAME},
	{"\010DESC_NM:"  ,DESC_NM},
	{"\007SOURCE:"   ,SOURCE},
	{"\010SRC_DTM:"  ,SRC_DTM},
	{"\010TRG_DTM:"  ,TRG_DTM},
	{"\006XFORM:"    ,XFORM},
	{"\006GROUP:"    ,GROUP},
	{"\005EPSG:"     ,EPSG_NBR},
	{"\011ACCURACY:" ,ACCURACY},
	{""              ,0}
};

int CSgpdefwr (	csFILE *outStrm,
				struct cs_GeodeticPath_ *gpdef,
				int warn,
				csFILE* dtmStrm,
				csFILE* xfrmStrm,
				int (*err_func)(char *mesg)
			  );

/**********************************************************************
**	err_cnt = CSgpcomp (inpt,outp,flags,xforms,err_func);
**
**	char *inpt;					full path name to the ASCII source file.
**	char *outp;					full path name to the target file.
**	int flags;					bitmap of options, see REMARKS below.
**	char *datum;				if non-null, indicates the full path name of the
**								datums file which is to be used to verify
**								integrity of geodetic path entries.
**	char *xforms;				if non-null, indicates the full path name
**								of the geodetic transformation dictionary
**								file which is to be used to verify
**								integrity of geodetic path entries.
**	int (*err_func)(char *mesg);function called top present error messages
**								to the user.
**	int err_cnt;				returns the number of errors reported.
**
**	The flags argument is a bit map of the following options:
**
**	cs_CMPLR_TEST  -- include test elements in output.  Normally,
**					  these are suppressed.
**  cs_CMPLR_WARN  -- issue warining messages as appropriate.
**
**	Please excuse the rather crude nature of this program.  However,
**	it is necessary for this program to be compiled, linked, and
**	run in just about any environment, without requiring users
**	to license LEX/YACC.
**********************************************************************/

int EXP_LVL9 CSgpcomp (	Const char *inpt,
						Const char *outp,
						int flags,
						char *datum,
						char* xforms,
						int (*err_func)(char *mesg)
					  )
{
	short index;

	int st;
	int test;
	int warn;
	int cancel;
	int line_nbr;
	int err_cnt;

	size_t rdCnt;
	size_t wrCnt;

	char *cp;
	char *cp1;
	char *dummy;
	csFILE *inStrm;
	csFILE *outStrm;
	csFILE *dtmStrm;
	csFILE *xfrmStrm;
	struct cs_GpcmpT_ *tp;
	struct cs_GeodeticPathElement_* pathElemPtr;

	cs_magic_t magic;

	char err_seg [18];
	char buff [128];
	char err_msg [128];
	char last_name [128];
	char workBufr [256];

	__ALIGNMENT__2					/* For some versions of Sun compiler. */
	struct cs_GeodeticPath_ gpdef;

	memset ((void*)&gpdef,0,sizeof (gpdef));

	/* Process the arguments on the command line. */
	test = ((flags & cs_CMPLR_TEST) != 0);
	warn = ((flags & cs_CMPLR_WARN) != 0);
	inStrm = NULL;
	outStrm = NULL;
	dtmStrm = NULL;
	xfrmStrm = NULL;

	/* OK, lets do it.  Open the source file. */
	inStrm = CS_fopen (inpt,_STRM_TXTRD);
	if (inStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for input.",inpt);
		cancel = (*err_func)(err_msg);
		return (1);
	}

	if (datum != NULL && *datum != '\0')
	{
		dtmStrm = CS_fopen (datum,_STRM_BINRD);
		if (dtmStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as a datum dictionary.",datum);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),dtmStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) || (magic != cs_DTDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not a Datum Dictionary file.",datum);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			CS_fclose (dtmStrm);
			return (1);
		}
	}

	if (xforms != NULL && *xforms != '\0')
	{
		xfrmStrm = CS_fopen (xforms,_STRM_BINRD);
		if (xfrmStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as a geodetic transform dictionary.",xforms);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			if (dtmStrm != NULL)
			{
				CS_fclose (dtmStrm);
			}
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),xfrmStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) || (magic != cs_GXDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not a geodetic transformation dictionary file.",xforms);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			if (dtmStrm != NULL)
			{
				CS_fclose (dtmStrm);
			}
			CS_fclose (xfrmStrm);
			return (1);
		}
	}

	/* Open the output file and write the magic number.  BINRW creates a
	   new file (truncates if necessary) with read write access.  We
	   need read/write as we will be sorting below. */
	outStrm = CS_fopen (outp,_STRM_BINRW);
	if (outStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for output.",outp);
		cancel = (*err_func)(err_msg);
		CS_fclose (inStrm);
		if (dtmStrm != NULL)
		{
			CS_fclose (dtmStrm);
		}
		if (xfrmStrm != NULL)
		{
			CS_fclose (xfrmStrm);
		}
		return (1);
	}

	magic = cs_GPDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite (&magic,1,sizeof (magic),outStrm);
	if (wrCnt != sizeof (magic))
	{
		sprintf (err_msg,"Write failure on %s.",outp);
		cancel = (*err_func)(err_msg);
		CS_fclose (inStrm);
		CS_fclose (outStrm);
		CS_remove (outp);										/*lint !e534 */
		if (dtmStrm != NULL)
		{
			CS_fclose (dtmStrm);
		}
		if (xfrmStrm != NULL)
		{
			CS_fclose (xfrmStrm);
		}
		return (1);
	}

	/* Process each line in the source file. */
	err_cnt = 0;
	cancel = FALSE;
	line_nbr = 0;
	while (CS_fgets (buff,sizeof (buff),inStrm) != NULL)
	{
		if (cancel)
		{
			CS_fclose (outStrm);
			CS_remove (outp);										/*lint !e534 */
			CS_fclose (inStrm);
			if (dtmStrm != NULL)
			{
				CS_fclose (dtmStrm);
			}
			if (xfrmStrm != NULL)
			{
				CS_fclose (xfrmStrm);
			}
			return (err_cnt);
		}
		line_nbr += 1;

		/* Ignore comments and blank lines. */
		(void)CS_trim (buff);
		if (buff [0] == '#' || buff [0] == '\0')
		{
			continue;
		}
		cp = buff;
		while ((cp = strchr (cp,'#')) != NULL)
		{
			if (*(cp + 1) != '#' &&
				*(cp - 1) != '\\')				/* backslash character here is the escape character */
			{
				*cp = '\0';
				break;
			}
		}

		/* Run the line through the table parser.  We see what type of line
		   we have. */
		cp = buff;
		tp = (struct cs_GpcmpT_ *)CS_tpars (&cp,(char *)cs_GpcmpT,sizeof (struct cs_GpcmpT_));
		if (tp == NULL)
		{
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Invalid line label (%s) at line %d.",err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			gpdef.pathName [0] = '\0';
			continue;
		}

		/* Here when we have a valid line. Cp points to the first
		   character after the colon. */
		(void)CS_trim (cp);
		switch (tp->type) {

		case GP_NAME:
			/* Here each time we encounter a new geodetic path name.  We see
			   if there is an existing geodetic path which must be written. */
			if (gpdef.pathName [0] != '\0' &&
				(test || CS_stricmp (gpdef.group,"TEST")))
			{
				st = CSgpdefwr (outStrm,&gpdef,warn,dtmStrm,xfrmStrm,err_func);
				if (st >= 0) err_cnt += st;
				else
				{
					err_cnt += -st;
					cancel = TRUE;
				}
			}
		
			/* Prepare for the next geodetic path definition. */
			(void)memset ((char *)&gpdef,'\0',sizeof (gpdef));
			(void)CS_stncp (gpdef.pathName,cp,sizeof (gpdef.pathName));

			st = CS_nampp64 (gpdef.pathName);
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid geodetic path key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case DESC_NM:
			(void)CS_stncp (gpdef.description,cp,sizeof (gpdef.description));
			break;

		case SOURCE:
			CS_stncp (gpdef.source,cp,sizeof (gpdef.source));
			break;

		case GROUP:
			CS_stncp (gpdef.group,cp,sizeof (gpdef.group));
			break;
		
		case SRC_DTM:
			CS_stncp (gpdef.srcDatum,cp,sizeof (gpdef.srcDatum));
			st = CS_nampp (gpdef.srcDatum);
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid datum key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;
		
		case TRG_DTM:
			CS_stncp (gpdef.trgDatum,cp,sizeof (gpdef.trgDatum));
			st = CS_nampp (gpdef.trgDatum);
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid datum key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case XFORM:
			index = gpdef.elementCount;
			if (index >= csPATH_MAXXFRM)
			{
				sprintf (err_msg,"Too many geodetic transformations specified for path named %s; max is %d.",gpdef.pathName,csPATH_MAXXFRM);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				pathElemPtr = &gpdef.geodeticPathElements [index];
				CS_stncp (workBufr,cp,sizeof (workBufr));
				cp1 = strrchr (workBufr,',');
				if (cp1 == NULL)
				{
					sprintf (err_msg,"%s is not a valid geodetic transformation reference; line %d.",cp,line_nbr);
					cancel = (*err_func)(err_msg);
					err_cnt += 1;
					break;
				}
				*cp1++ = '\0';
				st = CS_nampp64 (workBufr);
				if (st != 0)
				{
					sprintf (err_msg,"%s is not a valid geodetic transformation key name; line %d.",cp,line_nbr);
					cancel = (*err_func)(err_msg);
					err_cnt += 1;
					break;
				}
				if (CS_stricmp (cp1,"FWD") && CS_stricmp (cp1,"INV"))
				{
					sprintf (err_msg,"%s is not a valid geodetic transformation reference; line %d.",cp,line_nbr);
					cancel = (*err_func)(err_msg);
					err_cnt += 1;
					break;
				}
				pathElemPtr->direction = cs_DTCDIR_NONE;
				if (!CS_stricmp (cp1,"FWD"))
				{
					pathElemPtr->direction = cs_DTCDIR_FWD;
				}
				if (!CS_stricmp (cp1,"INV"))
				{
					pathElemPtr->direction = cs_DTCDIR_INV;
				}
				CS_stncp (pathElemPtr->geodeticXformName,workBufr,sizeof (pathElemPtr->geodeticXformName));
				gpdef.elementCount += 1;
			}
			break;

		case EPSG_NBR:
			gpdef.epsgCode = (short)atoi (cp);
			break;

		case ACCURACY:
			gpdef.accuracy = strtod (cp,&dummy);
			break;

		default:
			sprintf (err_msg,"Software problem at line %d in module %s.",__LINE__,__FILE__);
			cancel = (*err_func)(err_msg);
			break;
		}
		/* On to the next input line. */
	}
	CS_fclose (inStrm);

	/* Write out the last coordinate system if not in error. */
	if (gpdef.pathName [0] != '\0')
	{
		st = CSgpdefwr (outStrm,&gpdef,warn,dtmStrm,xfrmStrm,err_func);
		if (st >= 0) err_cnt += st;
		else
		{
			err_cnt += -st;
			cancel = TRUE;
		}
	}
	if (cancel)
	{
		CS_fclose (outStrm);
		CS_remove (outp);										/*lint !e534 */
		if (dtmStrm != NULL)
		{
			CS_fclose (dtmStrm);
		}
		if (xfrmStrm != NULL)
		{
			CS_fclose (xfrmStrm);
		}
		return (err_cnt);
	}

	/* Sort the output file. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_ips (outStrm,sizeof (gpdef),0L,(CMPFUNC_CAST)CS_gpcmp);		/*lint !e534 */

	/* Check for duplicate names. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_gprd (outStrm,&gpdef);				/*lint !e534  ignorin return value */
	CS_stncp (last_name,gpdef.pathName,sizeof (last_name));
	while (!cancel && CS_gprd (outStrm,&gpdef))
	{
		if (!CS_stricmp (last_name,gpdef.pathName))
		{
			sprintf (err_msg,"Geodetic path named %s appears more than once.",last_name);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		CS_stncp (last_name,gpdef.pathName,sizeof (last_name));
	}
	CS_fclose (outStrm);
	if (dtmStrm != NULL)
	{
		CS_fclose (dtmStrm);
	}
	if (xfrmStrm != NULL)
	{
		CS_fclose (xfrmStrm);
	}

	/* If there were any errors, delete the output file and
	   exit now. */
	if (err_cnt > 0)
	{
		CS_remove (outp);										/*lint !e534 */
	}

	/* That's it. */
	return (err_cnt);
}

int CSgpdefwr (	csFILE *outStrm,
				struct cs_GeodeticPath_ *gpdef,
				int warn,
				csFILE* dtmStrm,
				csFILE* xfrmStrm,
				int (*err_func)(char *mesg)
			  )
{
	short count;
	short index;

	int st;
	int flag;
	int err_cnt;
	int cancel;

	char *cp;

	char err_msg [128];

	struct cs_GeodeticPathElement_ *pathElemPtr;
	struct cs_GeodeticTransform_ gx_def;

	err_cnt = 0;
	cancel = FALSE;

	gpdef->protect = TRUE;
	/* The following sets the "creation date" to Jan 1, 2002 (approx) for
	   all test coordinate systems.  This enables the protection system to
	   be tested.  Normally, distribution definitions are protected from
	   any kind of change. */
	if (!CS_stricmp (gpdef->group,"TEST")) gpdef->protect = 4383;

	st = CS_gpwr (outStrm,gpdef);
	if (st != 0)
	{
		err_cnt += 1;
	}

	/* Verify  that all of the referenced geodetic transformations
	   do indeed exist in the provided Geodetic Transformation
	   Dictionary. */
	if (xfrmStrm != NULL && err_cnt == 0)
	{
		count = gpdef->elementCount;
		for (index = 0;index < count;index += 1)
		{
			pathElemPtr = &gpdef->geodeticPathElements [index];
			cp = pathElemPtr->geodeticXformName;
			
			memset ((void*)&gx_def,0,sizeof (gx_def));
			CS_stncp (gx_def.xfrmName,cp,sizeof (gx_def.xfrmName));
			flag = CS_bins (xfrmStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (gx_def),&gx_def,(CMPFUNC_CAST)CS_gxcmp);
			if (flag == 1)
			{
				st = CS_gxrd (dtmStrm,&gx_def);
				if (st == 1)
				{
					if (!CS_stricmp (gx_def.group,"LeGACY") && CS_stricmp (gpdef->group,"LEGACY"))
					{
						if (warn)
						{
							sprintf (err_msg,"Non-legacy geodetic path named %s references legacy geodetic transformation named %s.",gpdef->pathName,gx_def.xfrmName);
							cancel = (*err_func)(err_msg);
						}
					}
				}
			}
			else if (flag == 0)
			{
				sprintf (err_msg,"Invalid geodetic transformation, %s, in geodetic path named %s.",gx_def.xfrmName,gpdef->pathName);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				sprintf (err_msg,"Geodetic transformation dictionary access failure detected when checking geodetic path named %s.",gpdef->pathName);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
		}
	}

	if (warn && gpdef->description [0] == '\0')
	{
		sprintf (err_msg,"No description for geodetic path named %s.",gpdef->pathName);
		cancel = (*err_func)(err_msg);
	} 
	if (warn && gpdef->source [0] == '\0')
	{
		sprintf (err_msg,"No source for geodetic path named %s.",gpdef->pathName);
		cancel = (*err_func)(err_msg);
	}
	if (cancel && err_cnt == 0) err_cnt = 1;
	return (cancel ? -err_cnt : err_cnt);
}
