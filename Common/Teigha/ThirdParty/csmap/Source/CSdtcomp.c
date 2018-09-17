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
#include "cs_Legacy.h"

/*lint -esym(767,DT_NAME,DESC_NM,ELLP_NM,DELTA_X,DELTA_Y,DELTA_Z,ROT_X,ROT_Y,ROT_Z)  possibly different values in other modules */
/*lint -esym(767,BWSCALE,LOCATION,COUNTRY,SOURCE,USE,GROUP,EPSG_NBR)                 possibly different values in other modules */
/*lint -esym(754,cs_DtcmpT_::label)  not referenced directly, only indirectly */

extern double cs_DelMax;
extern double cs_RotMax;
extern double cs_SclMax;

#define DT_NAME      1
#define DESC_NM      2
#define ELLP_NM      3
#define DELTA_X      4
#define DELTA_Y      5
#define DELTA_Z      6
#define ROT_X        7
#define ROT_Y        8
#define ROT_Z        9
#define BWSCALE     10
#define LOCATION    11
#define COUNTRY     12
#define SOURCE      13
#define USE         14
#define GROUP       15
#define EPSG_NBR    16

struct cs_DtcmpT_
{
	char label [16];
	int type;
};

__ALIGNMENT__1		/* For some versions of Sun compiler. */
static struct cs_DtcmpT_ cs_DtcmpT [] =
{
	{"\012ELLIPSOID:" ,ELLP_NM},
	{"\011LOCATION:"  ,LOCATION},
	{"\010COUNTRY:"   ,COUNTRY},
	{"\010DT_NAME:"   ,DT_NAME},
	{"\010DESC_NM:"   ,DESC_NM},
	{"\010DELTA_X:"   ,DELTA_X},
	{"\010DELTA_Y:"   ,DELTA_Y},
	{"\010DELTA_Z:"   ,DELTA_Z},
	{"\010BWSCALE:"   ,BWSCALE},
	{"\007SOURCE:"    ,SOURCE},
	{"\006ROT_X:"     ,ROT_X},
	{"\006ROT_Y:"     ,ROT_Y},
	{"\006ROT_Z:"     ,ROT_Z},
	{"\006GROUP:"     ,GROUP},
	{"\005EPSG:"      ,EPSG_NBR},
	{"\004USE:"       ,USE},
	{          ""     ,0},
};

struct cs_DtTypeT_
{
	char label [16];
	int type;
};

__ALIGNMENT__2		/* For some versions of Sun compiler. */
static struct cs_DtTypeT_ cs_DtTypeT [] =
{
	{    "MOLODENSKY",cs_DTCTYP_MOLO},
	{         "BURSA",cs_DTCTYP_BURS},
	{        "MULREG",cs_DTCTYP_MREG},
	{         "NAD27",cs_DTCTYP_NAD27},
	{         "NAD83",cs_DTCTYP_NAD83},
	{         "WGS84",cs_DTCTYP_WGS84},
	{          "CSRS",cs_DTCTYP_CSRS},
	{         "GDA94",cs_DTCTYP_GDA94},
	{        "NZGD2K",cs_DTCTYP_NZGD2K},
	{         "WGS72",cs_DTCTYP_WGS72},
	{          "HPGN",cs_DTCTYP_HPGN},
	{    "7PARAMETER",cs_DTCTYP_7PARM},
	{         "AGD66",cs_DTCTYP_AGD66},
	{         "AGD84",cs_DTCTYP_AGD84},
	{        "NZGD49",cs_DTCTYP_NZGD49},
	{         "ATS77",cs_DTCTYP_ATS77},
	{         "JGD2K",cs_DTCTYP_TOKYO},
	{         "RGF93",cs_DTCTYP_RGF93},
	{          "ED50",cs_DTCTYP_ED50},
	{          "DHDN",cs_DTCTYP_DHDN},
	{        "CHENYX",cs_DTCTYP_CHENYX},
	{        "ETRF89",cs_DTCTYP_ETRF89},
	{    "GEOCENTRIC",cs_DTCTYP_GEOCTR},
	{    "6PARAMETER",cs_DTCTYP_6PARM},
	{    "4PARAMETER",cs_DTCTYP_4PARM},
	{    "3PARAMETER",cs_DTCTYP_3PARM},
	{              "",cs_DTCTYP_NONE}
};

static double no_value = -1.5E+22;
static double no_valueTest = -1.0E+22;
static int line_nbr;

int CSdtdefwr (	csFILE *outStrm,
				struct cs_Dtdef_ *dtdef,
				int crypt,
				int warn,
				csFILE *elStrm,
				int (*err_func)(char *mesg)
			  );

/**********************************************************************
**	err_cnt = CSdtcomp (inpt,outp,flags,elipsoid,err_func);
**
**	char *inpt;					full path to the source file to be compiled.
**	char *oupt;					full path to the target file.
**	int flags;					option flags, see REMARKS below.
**	char *elipsoid;				full path name to the Elipsoid Dictionary
**								to be used to validate ellipsoid names.
**	int (*err_func)(char *mesg);function used to pass error messages.
**	int err_cnt;				returns number of errors encountered.  If not
**								zero, the output file will have been removed.
**
**	This program will produce a binary Datum Dictionary FIle from
**	an ASCII source file.  It has two purposes:
**
**	1) Provides abilty to maintain all Datums deinitions in an
**	   ASCII text file which can be controlled.
**	2) Enables production of Datum Dictionary files on various
**	   platforms with different byte ordering.
**
**	The flags argument is a bit map of the following options:
**
**	cs_CMPLR_CRYPT -- produce encrypted output files.
**	cs_CMPLR_DEMO  -- use demonstration magic numbers.
**	cs_CMPLR_TEST  -- include test elements in the output.
**					  Normally, test elements are suppressed.
**
**	Please excuse the rather crude way this program is written.
**	It is necessary for this program to work in just about
**	any environement; and to be compilable without requiring
**	CS_MAP licensee's to also be LEX/YACC licensees.
**********************************************************************/

int EXP_LVL9 CSdtcomp (	Const char *inpt,
						Const char *outp,
						int flags,
						Const char *elipsoid,
						int (*err_func)(char *mesg)
					  )
{
	int st;
	int test;
	int demo;
	int warn;
	int dummy;
	int cancel;
	int err_cnt;
	int crypt;

	size_t rdCnt;
	size_t wrCnt;
	size_t strLen;

	char *cp;
	csFILE *inStrm;
	csFILE *elStrm;
	csFILE *outStrm;
	struct cs_DtcmpT_ *tp;
	struct cs_DtTypeT_ *typPtr;

	cs_magic_t magic;

	char err_seg [18];
	char buff [128];
	char err_msg [128];
	char last_name [48];

	__ALIGNMENT__2		/* For some versions of Sun compiler. */
	struct cs_Dtdef_ dtdef;

	crypt = ((flags & cs_CMPLR_CRYPT) != 0);
	demo  = ((flags & cs_CMPLR_DEMO) != 0);
	test  = ((flags & cs_CMPLR_TEST) != 0);
	warn  = ((flags & cs_CMPLR_WARN) != 0);

	/* Open the source file. */
	inStrm = CS_fopen (inpt,_STRM_TXTRD);
	if (inStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for input.",inpt);
		cancel = (*err_func)(err_msg);
		return (1);
	}

	/* Open the output file and write the magic number.  BINRW mode
	   creates a new file with read and write access. */
	outStrm = CS_fopen (outp,_STRM_BINRW);
	if (outStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for output.",outp);
		cancel = (*err_func)(err_msg);
		CS_fclose (inStrm);
		return (1);
	}
	if (demo) magic = cs_DTDEF_MAGIC;
	else      magic = cs_DTDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite ((char *)&magic,1,sizeof (magic),outStrm);
	if (wrCnt != sizeof (magic))
	{
		sprintf (err_msg,"Failure detected during write to %s.",outp);
		cancel = (*err_func)(err_msg);  
		CS_fclose (outStrm);
		CS_remove (outp);										/*lint !e534 */
		CS_fclose (inStrm);
		return (1);
	}

	/* If we have been given an ellipsoid file name, we open it now. */
	if (elipsoid != NULL && *elipsoid != '\0')
	{
		elStrm = CS_fopen (elipsoid,_STRM_BINRD);
		if (elStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as an Ellipsoid Dictionary.",elipsoid);
			cancel = (*err_func)(err_msg);
			CS_fclose (inStrm);
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),elStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) ||
			(demo && magic != cs_ELDEF_MAGIC) ||
			(!demo && magic != cs_ELDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not an Elipsoid Dictionary file.",elipsoid);
			cancel = (*err_func)(err_msg);
			CS_fclose (inStrm);
			CS_fclose (elStrm);
			return (1);
		}
	}
	else
	{
		elStrm = NULL;
	}

	/* Process each line in the source file. */
	line_nbr = 0;
	err_cnt = 0;
	cancel = FALSE;
	memset (&dtdef,'\0',sizeof (dtdef));
	dtdef.delta_X = no_value;
	dtdef.delta_Y = no_value;
	dtdef.delta_Z = no_value;
	dtdef.rot_X = no_value;
	dtdef.rot_Y = no_value;
	dtdef.rot_Z = no_value;
	dtdef.bwscale = no_value;
	dtdef.to84_via = cs_DTCTYP_NONE;
	while (CS_fgets (buff,sizeof (buff),inStrm) != NULL)
	{
		if (cancel)
		{
			CS_fclose (inStrm);
			CS_fclose (outStrm);
			if (elStrm != NULL) CS_fclose (elStrm);
			CS_remove (outp);										/*lint !e534 */
			return (err_cnt);
		}
		line_nbr += 1;

		/* Ignore comments and blank lines. */
		CS_trim (buff);
		if (buff [0] == '#' || buff [0] == '\0')
		{
			continue;
		}
		cp = buff;
		while ((cp = strchr (cp,'#')) != NULL)
		{
			if (*(cp + 1) != '#' &&
				*(cp - 1) != '\\')				/* backslash here is the escape character */
			{
				*cp = '\0';
				break;
			}
		}

		/* Run the line through the table parser.  We
		   see what type of line we have. */
		cp = buff;
		tp = (struct cs_DtcmpT_ *)CS_tpars (&cp,cs_DtcmpT,sizeof (struct cs_DtcmpT_));

		if (tp == NULL)
		{
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Invalid keyword (%s) on line %d.",err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
			dtdef.key_nm [0] = '\0';
			continue;
		}

		/* Here when we have a valid line. Cp points to the first
		   character after the colon. */
		CS_trim (cp);
		switch (tp->type) {
		case DT_NAME:

			/* Here each time we encounter a new datum
			   name.  We see if there is an existing
			   datum which must be written. */
			if (dtdef.key_nm [0] != '\0' &&		/* Skip 1st one. */
				(test || CS_stricmp (dtdef.group,"TEST")))
			{
				st = CSdtdefwr (outStrm,&dtdef,crypt,warn,elStrm,err_func);
				if (st >= 0) err_cnt += st;
				else
				{
					cancel = TRUE;
					err_cnt += -st;
				}
				
			}

			/* Prepare for the next coordinate system. */
			memset (&dtdef,'\0',sizeof (dtdef));
			dtdef.delta_X = no_value;
			dtdef.delta_Y = no_value;
			dtdef.delta_Z = no_value;
			dtdef.rot_X = no_value;
			dtdef.rot_Y = no_value;
			dtdef.rot_Z = no_value;
			dtdef.bwscale = no_value;
			dtdef.protect = FALSE;
			dtdef.to84_via = cs_DTCTYP_NONE;

			strLen = strlen (cp);
			CS_stncp (dtdef.key_nm,cp,sizeof (dtdef.key_nm));
			st = CS_nampp (dtdef.key_nm);
			if (strLen > cs_KEYNM_MAX)
			{
				sprintf (err_msg,"Key name %s exceeds maximun length of %d; line %d.",cp,cs_KEYNM_MAX,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid datum key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case DESC_NM:
			if (strlen (cp) >= sizeof (dtdef.name) && warn)
			{
				sprintf (err_msg,"Warning: Description for %s on line %d is too long.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (dtdef.name,cp,sizeof (dtdef.name));
			break;

		case ELLP_NM:
			CS_stncp (dtdef.ell_knm,cp,sizeof (dtdef.ell_knm));
			break;

		case LOCATION:
			if (strlen (cp) >= sizeof (dtdef.locatn) && warn)
			{
				sprintf (err_msg,"Warning: Country list for %s on line %d is too long.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (dtdef.locatn,cp,sizeof (dtdef.locatn));
			break;

		case COUNTRY:
			if (strlen (cp) >= sizeof (dtdef.cntry_st) && warn)
			{
				sprintf (err_msg,"Warning: Country list for %s on line %d is too long.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (dtdef.cntry_st,cp,sizeof (dtdef.cntry_st));
			break;

		case SOURCE:
			if (strlen (cp) >= sizeof (dtdef.source) && warn)
			{
				sprintf (err_msg,"Warning: Source for %s on line %d is too long.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (dtdef.source,cp,sizeof (dtdef.source));
			break;

		case DELTA_X:
			dtdef.delta_X = atof (cp);
			if (warn && (fabs (dtdef.delta_X) > 2000.0))
			{
				sprintf (err_msg,"Warning: Delta X value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			break;

		case DELTA_Y:
			dtdef.delta_Y = atof (cp);
			if (warn && (fabs (dtdef.delta_Y) > 2000.0))
			{
				sprintf (err_msg,"Warning: Delta Y value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			break;

		case DELTA_Z:
			dtdef.delta_Z = atof (cp);
			if (warn && (fabs (dtdef.delta_Z) > 2000.0))
			{
				sprintf (err_msg,"Warning: Delta Z value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			break;

		case ROT_X:
			dtdef.rot_X = atof (cp);
			if (warn && (fabs (dtdef.rot_X) >= 60.0))
			{
				sprintf (err_msg,"Warning: X Rotation value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}		
			break;

		case ROT_Y:
			dtdef.rot_Y = atof (cp);
			if (warn && (fabs (dtdef.rot_Y) >= 60.0))
			{
				sprintf (err_msg,"Warning: Y Rotation value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}		
			break;

		case ROT_Z:
			dtdef.rot_Z = atof (cp);
			if (warn	 && (fabs (dtdef.rot_Z) >= 60.0))
			{
				sprintf (err_msg,"Warning: Y Rotation value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}		
			break;

		case BWSCALE:
			dtdef.bwscale = atof (cp);
			if (warn && (fabs (dtdef.bwscale) >= 25.0))
			{
				sprintf (err_msg,"Warning: Bursa Wolfe scale value for %s on line %d is suspicious.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}		
			break;

		case USE:
			dtdef.to84_via = cs_DTCTYP_NONE;
			for (typPtr = cs_DtTypeT;typPtr->type != cs_DTCTYP_NONE;typPtr += 1)
			{
				if (!CS_stricmp (cp,typPtr->label))
				{
					dtdef.to84_via = (short)typPtr->type;
					break;
				}
			}
			if (dtdef.to84_via == cs_DTCTYP_NONE)
			{
				sprintf (err_msg,"Invalid 'USE' specification detected on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case GROUP:
			if (strlen (cp) >= sizeof (dtdef.group) && warn)
			{
				sprintf (err_msg,"Warning: Group for %s on line %d is too long.",dtdef.key_nm,line_nbr);
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (dtdef.group,cp,sizeof (dtdef.group));
			break;

		case EPSG_NBR:
			dtdef.epsgNbr = (short)atoi (cp);
			break;

		default:
			sprintf (err_msg,"Software problem detected at line %d in module %s.",__LINE__,__FILE__);
			(void)(*err_func)(err_msg);
			cancel = TRUE;
			break;
		}

		/* On to the next input line. */
	}
	CS_fclose (inStrm);

	/* Write out the last coordinate system if not in error. */
	if (dtdef.key_nm [0] != '\0' && (test || CS_stricmp (dtdef.group,"TEST")))
	{
		st = CSdtdefwr (outStrm,&dtdef,crypt,warn,elStrm,err_func);
		if (st >= 0) err_cnt += st;
		else
		{
			cancel = TRUE;
			err_cnt += -st;
		}
	}
	if (elStrm != NULL) CS_fclose (elStrm);
	if (cancel)
	{
		CS_fclose (outStrm);
		CS_remove (outp);										/*lint !e534 */
		return (err_cnt);
	}

	/* Sort the output file. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_ips (outStrm,sizeof (dtdef),0L,(CMPFUNC_CAST)CS_dtcmp);		/*lint !e534 */

	/* Verify that there are no duplicates. */
	CS_fseek (outStrm,(long)sizeof (magic),0);
	CS_dtrd (outStrm,&dtdef,&dummy);								/*lint !e534 */
	CS_stncp (last_name,dtdef.key_nm,sizeof (last_name));
	while (!cancel && CS_dtrd (outStrm,&dtdef,&dummy) != 0)
	{
		if (!CS_stricmp (dtdef.key_nm,last_name))
		{
			sprintf (err_msg,"Datum key name %s appears more than once.",last_name);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		CS_stncp (last_name,dtdef.key_nm,sizeof (last_name));
	}
	CS_fclose (outStrm);

	/* Remove the output file if there were errors. */
	if (err_cnt != 0)
	{
		CS_remove (outp);										/*lint !e534 */
	}
	return (err_cnt);
}

int CSdtdefwr (	csFILE *outStrm,
				struct cs_Dtdef_ *dtdef,
				int crypt,
				int warn,
				csFILE *elStrm,
				int (*err_func)(char *mesg)
			  )
{
	extern double cs_Zero;			/* 0.0 */

	int st;
	int elCrypt;
	int err_cnt;
	int cancel;
	int flag;
	int tr_cnt;
	int rt_cnt;
	int sc_cnt;

	char err_msg [128];
 
 	__ALIGNMENT__2		/* For some versions of Sun compiler. */
	struct cs_Eldef_ eldef;

	err_cnt = 0;
	cancel = 0;
	
	if (dtdef->to84_via == cs_DTCTYP_NONE)
	{
		sprintf (err_msg,"Conversion specification missing in definition of %s.",dtdef->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}

	/* Make some last minute checks which can't be done by
	   the line processor. */
	tr_cnt = 3;
	if (dtdef->delta_X <= no_valueTest) tr_cnt -= 1;
	if (dtdef->delta_Y <= no_valueTest) tr_cnt -= 1;
	if (dtdef->delta_Z <= no_valueTest) tr_cnt -= 1;
	rt_cnt = 3;
	if (dtdef->rot_X <= no_valueTest) rt_cnt -= 1;
	if (dtdef->rot_Y <= no_valueTest) rt_cnt -= 1;
	if (dtdef->rot_Z <= no_valueTest) rt_cnt -= 1;
	sc_cnt = 1;
	if (dtdef->bwscale <= no_valueTest) sc_cnt -= 1;

	switch (dtdef->to84_via) {

	case cs_DTCTYP_MOLO:
	case cs_DTCTYP_GEOCTR:
	case cs_DTCTYP_3PARM:
		if (tr_cnt != 3)
		{
			sprintf (err_msg,"Delta[XYZ] value(s) missing for datum named %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		if ((rt_cnt != 0 || sc_cnt != 0) && warn)
		{
			sprintf (err_msg,"Warning: unused Bursa/Wolfe parameters present for %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
		}
		break;

	case cs_DTCTYP_BURS:
	case cs_DTCTYP_7PARM:
		if (tr_cnt != 3 || rt_cnt != 3 || sc_cnt != 1)
		{
			sprintf (err_msg,"Missing parameters for %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		break;

	case cs_DTCTYP_6PARM:
		if (tr_cnt != 3 || rt_cnt != 3)
		{
			sprintf (err_msg,"Missing parameters for %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		if (sc_cnt != 0 && warn)
		{
			sprintf (err_msg,"Warning: datum %s has unused scale parameter specified.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
		}
		break;

	case cs_DTCTYP_4PARM:
		if (tr_cnt != 3 || sc_cnt != 1)
		{
			sprintf (err_msg,"Missing parameters for %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		if (rt_cnt != 0 && warn)
		{
			sprintf (err_msg,"Warning: datum %s has unused rotation parameters specified.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
		}
		break;

	case cs_DTCTYP_MREG:
	case cs_DTCTYP_NAD27:
	case cs_DTCTYP_NAD83:
	case cs_DTCTYP_WGS84:
	case cs_DTCTYP_WGS72:
	case cs_DTCTYP_HPGN:
	case cs_DTCTYP_AGD66:
	case cs_DTCTYP_AGD84:
	case cs_DTCTYP_NZGD49:
	case cs_DTCTYP_ATS77:
	case cs_DTCTYP_GDA94:
	case cs_DTCTYP_NZGD2K:
	case cs_DTCTYP_CSRS:
	case cs_DTCTYP_TOKYO:
	case cs_DTCTYP_RGF93:
	case cs_DTCTYP_ED50:
	case cs_DTCTYP_DHDN:
	case cs_DTCTYP_CHENYX:
	case cs_DTCTYP_ETRF89:
		break;

	default:
		sprintf (err_msg,"Internal software error, CSdtcomp:%d.",__LINE__);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
		break;
	}

	/* For those values not specified, we reset them to hard zero. */
	if (dtdef->delta_X <= no_valueTest) dtdef->delta_X = cs_Zero;
	if (dtdef->delta_Y <= no_valueTest) dtdef->delta_Y = cs_Zero;
	if (dtdef->delta_Z <= no_valueTest) dtdef->delta_Z = cs_Zero;
	if (dtdef->rot_X <= no_valueTest)   dtdef->rot_X = cs_Zero;
	if (dtdef->rot_Y <= no_valueTest)   dtdef->rot_Y = cs_Zero;
	if (dtdef->rot_Z <= no_valueTest)   dtdef->rot_Z = cs_Zero;
	if (dtdef->bwscale <= no_valueTest) dtdef->bwscale = cs_Zero;

	if (dtdef->ell_knm [0] == '\0')
	{
		sprintf (err_msg,"No ellipsoid specified for datum named %s.",dtdef->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}
	else if (elStrm != 0)
	{
		CS_stncp (eldef.key_nm,dtdef->ell_knm,sizeof (eldef.key_nm));
		CS_nampp (eldef.key_nm);							/*lint !e534 */
		eldef.fill [0] = '\0';
		eldef.fill [1] = '\0';
		flag = CS_bins (elStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,
						sizeof (eldef),
						&eldef,
						(CMPFUNC_CAST)CS_elcmp);
		if (flag == 1)
		{
			st = CS_elrd (elStrm,&eldef,&elCrypt);
			if (st == 1)
			{
				if (!CS_stricmp (eldef.group,"LGACY") && CS_stricmp (dtdef->group,"LEGACY"))
				{
					sprintf (err_msg,"Non-legacy datum named %s references legacy ellipsoid named %s.",dtdef->key_nm,dtdef->ell_knm);
					cancel = (*err_func)(err_msg);
					err_cnt += 1;
				}
			}
		}
		else if (flag == 0)
		{
			sprintf (err_msg,"Invalid ellipsoid name, %s, on line %d.",dtdef->ell_knm,line_nbr);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		else
		{
			sprintf (err_msg,"Ellipsoid dictionary access failure detected when checking datum named %s.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}

	/* Verify that the values are not completely bogus. */
	if (fabs (dtdef->delta_X) > cs_DelMax ||
		fabs (dtdef->delta_Y) > cs_DelMax ||
		fabs (dtdef->delta_Z) > cs_DelMax ||
		fabs (dtdef->rot_X) > cs_RotMax   ||
		fabs (dtdef->rot_Y) > cs_RotMax   ||
		fabs (dtdef->rot_Z) > cs_RotMax   ||
		fabs (dtdef->bwscale) > cs_SclMax)
	{
		sprintf (err_msg,"Definition of datum named %s contains some obnoxious parameter values.",dtdef->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}
	
	/* Verify that if the transformation technique is 3PARAMETER the
	   group is LEGACY. */
	if (dtdef->to84_via == cs_DTCTYP_3PARM)
	{
		if (CS_stricmp (dtdef->group,"LEGACY"))
		{
			sprintf (err_msg,"Definition of datum named %s refers to deprecated 3PARAMETER technique.",dtdef->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}

	if (err_cnt == 0)
	{
		dtdef->protect = TRUE;
		/* The following sets the "creation date" to Jan 1, 2002 (approx) for
		   all test coordinate systems.  This enables the protection system to
		   be tested.  Normally, distribution definitions are protected from
		   any kind of change. */
		if (!CS_stricmp (dtdef->group,"TEST")) dtdef->protect = 4383;
		dtdef->fill [0] = '\0';
		dtdef->fill [1] = '\0';
		CS_dtwr (outStrm,dtdef,crypt);					/*lint !e534 */
	}
	if (warn && dtdef->name [0] == '\0')
	{
		sprintf (err_msg,"No description provided for datum %s.",dtdef->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (warn && dtdef->source [0] == '\0')
	{
		sprintf (err_msg,"No source provided for datum %s.",dtdef->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (cancel && err_cnt == 0) err_cnt = 1;
	return (cancel ? -err_cnt : err_cnt);
}
