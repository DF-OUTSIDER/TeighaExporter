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

/*lint -esym(767,EL_NAME,DESC_NM,E_RAD,P_RAD,FLAT,ECENT,GROUP,SOURCE,EPSG_NBR)  possibly different values in other modules */
/*lint -esym(754,cs_ElcmpT_::label)  not referenced directly, only indirectly */

#define EL_NAME 1
#define DESC_NM 2
#define E_RAD   3
#define P_RAD   4
#define FLAT    5
#define ECENT   6
#define GROUP   7
#define SOURCE  8
#define EPSG_NBR 9

struct cs_ElcmpT_
{
	char label [10];
	int type;
};

__ALIGNMENT__1		/* For some versions of Sun compiler. */
static struct cs_ElcmpT_ cs_ElcmpT [] =
{
	{"\010EL_NAME:"  ,EL_NAME},
	{"\010DESC_NM:"  ,DESC_NM},
	{"\007SOURCE:"   ,SOURCE},
	{"\006E_RAD:"    ,E_RAD},
	{"\006P_RAD:"    ,P_RAD},
	{"\006ECENT:"    ,ECENT},
	{"\006GROUP:"    ,GROUP},
	{"\005FLAT:"     ,FLAT},
	{"\005EPSG:"     ,EPSG_NBR},
	{""              ,0}
};

int CSeldefwr (	csFILE *outStrm,
				struct cs_Eldef_ *eldef,
				int crypt,
				int warn,
				int (*err_func)(char *mesg)
			  );

/**********************************************************************
**	err_cnt = CSelcomp (inpt,outp,flags,err_func);
**
**	char *inpt;					full pathe name to the ASCII source file.
**	char *outp;					full path name to the target file.
**	int flags;					bitmat of options, see REMARKS below.
**	int (*err_func)(char *mesg);function called top present error messages
**								to the user.
**	int err_cnt;				returns the number of errors reported.
**
**	The flags argument is a bit map of the following options:
**
**	cs_CMPLR_CRYPT -- produce an encrypted output file.
**	cs_CMPLR_DEMO  -- use DEMO magic numbers.
**	cs_CMPLR_TEST  -- include test elements in outut.  Normally,
**					  these are suppressed.
**
**	Please excuse the rather crude nature of this program.  However,
**	it is necessary for this program to be compiled, linked, and
**	run in just about any environment, without requiring users
**	to license LEX/YACC.
**********************************************************************/


int EXP_LVL9 CSelcomp (	Const char *inpt,
						Const char *outp,
						int flags,
						int (*err_func)(char *mesg)
					  )

{
	int st;
	int demo;
	int test;
	int warn;
	int dummy;
	int cancel;
	int crypt;
	int line_nbr;
	int err_cnt;
	
	size_t wrCnt;
	size_t strLen;

	char *cp;
	csFILE *inStrm;
	csFILE *outStrm;
	struct cs_ElcmpT_ *tp;

	cs_magic_t magic;

	char err_seg [18];
	char buff [128];
	char err_msg [128];
	char last_name [48];

	__ALIGNMENT__2		/* For some versions of Sun compiler. */
	struct cs_Eldef_ eldef;

	/* Process the arguments on the command line. */
	crypt = ((flags & cs_CMPLR_CRYPT) != 0);
	demo = ((flags & cs_CMPLR_DEMO) != 0);
	test = ((flags & cs_CMPLR_TEST) != 0);
	warn = ((flags & cs_CMPLR_WARN) != 0);

	/* OK, lets do it.  Open the source file. */
	inStrm = CS_fopen (inpt,_STRM_TXTRD);
	if (inStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for input.",inpt);
		cancel = (*err_func)(err_msg);
		return (1);
	}

	/* Open the output file and write the magic number.  BINRW creats a
	   new file (truncates if necessary) with read write access.  We
	   need read/write as we will be dorting below. */
	outStrm = CS_fopen (outp,_STRM_BINRW);
	if (outStrm == NULL)
	{
		sprintf (err_msg,"Couln't open %s for output.",outp);
		cancel = (*err_func)(err_msg);
		CS_fclose (inStrm);
		return (1);
	}

	if (demo) magic = cs_ELDEF_MAGIC;
	else      magic = cs_ELDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite (&magic,1,sizeof (magic),outStrm);
	if (wrCnt != sizeof (magic))
	{
		sprintf (err_msg,"Write failure on %s.",outp);
		cancel = (*err_func)(err_msg);
		CS_fclose (inStrm);
		CS_fclose (outStrm);
		CS_remove (outp);										/*lint !e534 */
		return (1);
	}

	/* Process each line in the source file. */
	err_cnt = 0;
	cancel = FALSE;
	line_nbr = 0;
	memset (&eldef,'\0',sizeof (eldef));
	eldef.e_rad = 0.0;
	eldef.p_rad = 0.0;
	while (CS_fgets (buff,sizeof (buff),inStrm) != NULL)
	{
		if (cancel)
		{
			CS_fclose (outStrm);
			CS_remove (outp);										/*lint !e534 */
			CS_fclose (inStrm);
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

		/* Run the line through the table parser.  WWe
		   see what type of line we have. */
		cp = buff;
		tp = (struct cs_ElcmpT_ *)CS_tpars (&cp,(char *)cs_ElcmpT,sizeof (struct cs_ElcmpT_));
		if (tp == NULL)
		{
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Invalid line label (%s) at line %d.",err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			eldef.key_nm [0] = '\0';
			continue;
		}

		/* Here when we have a valid line. Cp points to the first
		   character after the colon. */
		(void)CS_trim (cp);
		switch (tp->type) {

		case EL_NAME:
			/* Here each time we encounter a new ellipsoid
			   name.  We see if there is an existing
			   ellipsoid which must be written. */

			if (eldef.key_nm [0] != '\0' &&
				(test || CS_stricmp (eldef.group,"TEST")))
			{
				
				st = CSeldefwr (outStrm,&eldef,crypt,warn,err_func);
				if (st >= 0) err_cnt += st;
				else
				{
					err_cnt += -st;
					cancel = TRUE;
				}
			}
		
			/* Prepare for the next ellipsoid definition. */
			(void)memset ((char *)&eldef,'\0',sizeof (eldef));

			strLen = strlen (cp);
			(void)CS_stncp (eldef.key_nm,cp,sizeof (eldef.key_nm));
			st = CS_nampp (eldef.key_nm);
			if (strLen > cs_KEYNM_MAX)
			{
				sprintf (err_msg,"key name %s exceeds maximun length of %d; line %d.",cp,cs_KEYNM_MAX,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid ellipsoid key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case DESC_NM:
			(void)CS_stncp (eldef.name,cp,sizeof (eldef.name));
			break;

		case E_RAD:
			eldef.e_rad = atof (cp);
			break;

		case P_RAD:
			eldef.p_rad = atof (cp);
			break;

		case FLAT:
			eldef.flat = atof (cp);
			break;

		case ECENT:
			eldef.ecent = atof (cp);
			break;

		case SOURCE:
			CS_stncp (eldef.source,cp,sizeof (eldef.source));
			break;

		case GROUP:
			CS_stncp (eldef.group,cp,sizeof (eldef.group));
			break;
		
		case EPSG_NBR:
			eldef.epsgNbr = (short)atoi (cp);
			break;

		default:
			sprintf (err_msg,"Software Problem at line %d in module %s.",__LINE__,__FILE__);
			break;
		}
		/* On to the next input line. */
	}
	CS_fclose (inStrm);

	/* Write out the last coordinate system if not in error. */
	if (eldef.key_nm [0] != '\0')
	{
		st = CSeldefwr (outStrm,&eldef,crypt,warn,err_func);
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
		return (err_cnt);
	}

	/* Sort the output file. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_ips (outStrm,sizeof (eldef),0L,(CMPFUNC_CAST)CS_elcmp);		/*lint !e534 */

	/* Check for duplicate names. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_elrd (outStrm,&eldef,&dummy);								/*lint !e534 */
	CS_stncp (last_name,eldef.key_nm,sizeof (last_name));
	while (!cancel && CS_elrd (outStrm,&eldef,&dummy))
	{
		if (!CS_stricmp (last_name,eldef.key_nm))
		{
			sprintf (err_msg,"Ellipsoid named %s appears more than once.",last_name);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		CS_stncp (last_name,eldef.key_nm,sizeof (last_name));
	}
	CS_fclose (outStrm);
	
	/* If there were any errors, delete the output file and
	   exit now. */
	if (err_cnt > 0)
	{
		CS_remove (outp);										/*lint !e534 */
	}

	/* That's it. */
	return (err_cnt);
}

int CSeldefwr (	csFILE *outStrm,
				struct cs_Eldef_ *eldef,
				int crypt,
				int warn,
				int (*err_func)(char *mesg)
			  )
{
	int err_cnt;
	int cancel;

	char err_msg [128];

	err_cnt = 0;
	cancel = FALSE;

	if (eldef->e_rad == 0.0)
	{
		sprintf (err_msg,"No equatorial radius specified for %s.          \n",eldef->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}
	else if (eldef->p_rad == 0.0)
	{
		sprintf (err_msg,"No polar radius specified for %s.",eldef->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}
	else if (warn && ((eldef->e_rad < 6000000.0) || (eldef->e_rad > 7000000.0)))
	{
		sprintf (err_msg,"Warning: Equatorial radius of ellipsoid named %s is suspicious.",eldef->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (err_cnt == 0)
	{
		eldef->protect = TRUE;
		/* The following sets the "creation date" to Jan 1, 2002 (approx) for
		   all test coordinate systems.  This enables the protection system to
		   be tested.  Normally, distribution definitions are protected from
		   any kind of change. */
		if (!CS_stricmp (eldef->group,"TEST")) eldef->protect = 4383;
		eldef->fill [0] = '\0';
		eldef->fill [1] = '\0';

		/* Compute the flattening and eccentricity. */
		eldef->flat = 1.0 - (eldef->p_rad / eldef->e_rad);
		eldef->ecent = sqrt ((2.0 * eldef->flat) - (eldef->flat * eldef->flat));
		if (warn &&
			!CS_cmpDbls (eldef->p_rad,eldef->e_rad) &&
			((eldef->flat > 0.0040) ||
			 (eldef->flat < 0.0032)))
		{
			sprintf (err_msg,"Warning: Polar radius of ellipsoid named %s is suspicious.",eldef->key_nm);
			cancel = (*err_func)(err_msg);
		}
		CS_elwr (outStrm,eldef,crypt);								/*lint !e534 */
	}
	if (warn && eldef->name [0] == '\0')
	{
		sprintf (err_msg,"No description for ellipsoid named %s.",eldef->key_nm);
		cancel = (*err_func)(err_msg);
	} 
	if (warn && eldef->source [0] == '\0')
	{
		sprintf (err_msg,"No source for ellipsoid named %s.",eldef->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (cancel && err_cnt == 0) err_cnt = 1;
	return (cancel ? -err_cnt : err_cnt);
}
