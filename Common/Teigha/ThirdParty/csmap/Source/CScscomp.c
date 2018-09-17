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

/*lint -esym(534,CS_atof) */					/* ignore return value */
/*lint -esym(534,CS_csrd) */					/* ignore return value */
/*lint -esym(534,CS_cswr) */					/* ignore return value */
/*lint -esym(534,CS_fseek) */					/* ignore return value */
/*lint -esym(534,CS_ips) */						/* ignore return value */
/*lint -esym(534,CS_nampp) */					/* ignore return value */
/*lint -esym(534,CSerpt) */						/* ignore return value */
/*lint -esym(534,err_func) */					/* ignore return value */
/*lint -esym(534,CS_remove) */					/* ignore return value */
/*lint -esym(534,CS_trim) */					/* ignore return value */
/*lint -e522	         */						/* return value of function call through  pointer ignored. */

#include "cs_map.h"

/*lint -esym(767,CS_NAME,DESC_NM,DT_NAME,EL_NAME,ORG_LAT,ORG_LNG,SCL_RED,MAP_SCL)   possibly different values in other modules */
/*lint -esym(767,ZERO_X,ZERO_Y,PARM1,PARM2,PARM3,PARM4,PARM5,PARM6,PARM7,PARM8)     possibly different values in other modules */
/*lint -esym(767,PARM9,PARM10,PARM11,PARM12,PARM13,PARM14,PARM15,PARM16,PARM17)     possibly different values in other modules */
/*lint -esym(767,PARM18,PARM19,PARM20,PARM21,PARM22,PARM23,PARM24,X_OFF,Y_OFF)      possibly different values in other modules */
/*lint -esym(767,PROJ,UNIT,HGT_LNG,HGT_LAT,HGT_ZZ,MIN_LNG,MIN_LAT,MAX_LNG,MAX_LAT)  possibly different values in other modules */
/*lint -esym(767,MIN_XX,MIN_YY,MAX_XX,MAX_YY,QUAD,ORDER,ZONES,GROUP,LOCATION)       possibly different values in other modules */
/*lint -esym(767,COUNTRY,SOURCE,EPSG_NBR,SRID_NBR,EPSG_QD)                          possibly different values in other modules */

/*lint -esym(754,cs_CscmpT_::label)  not referenced directly, only indirectly */

/**********************************************************************
**	err_cnt = CScscomp (inpt,outp,flags,elipsoid,datum,err_func);
**
**	char *inpt;					full path name to the source file (ASCII).
**	char *outp;					full path name to the target file (BINARY).
**	int flags;					bitmap of option flags, see REMARKS below.
**	char *elipsoid;				if non-null, indicates the full path name of the
**								ellipsoid file which is to be used to verify
**								database integrity.
**	char *datum;				if non-null, indicates the full path name of the
**								datums file which is to be used to verify
**								database integrity.
**	int (*err_func)(char *mesg);this function is called to report all errors.  A non
**								zero return says cancel.
**	int err_cnt;				returns the number of errors detected.  If non-zero,
**								output file will not have been created.
**
**	This function converts the provided ASCII file into its binary
**	form.  It serves two purposes:
**
**	1) Allows all coordinate system definitions to be maintained
**	   in ASCII and controlled by a source control mechanism.
**	2) Enables production of COORDSYS files on various platforms
**	   with various byte orderings.
**
**	Please the rather crude natire of this program.  For the
**	viability of the CS_MAP product, however, it must be able
**	to run on just about any system which has a C compiler,
**	and we cannot require the user to have a license to a
**	LEX/YACC type product.
**
**	Possible options 'or' ed into flags:
**
**	cs_CMPLR_DEMO  -- Use demonstration magic number on output file.
**	cs_CMPLR_TEST  -- Include test coordinate systems in output. Normally,
**					  these are suppressed.
**	cs_CMPLR_CRYPT -- Encrypt the output.
**********************************************************************/

#if _RUN_TIME == _rt_UNIXPCC || _RUN_TIME == _rt_SUN32 || _RUN_TIME == _rt_SUN64 || _RUN_TIME == _rt_MOSXUNIX || _RUN_TIME == _rt_HPUX
#       include <unistd.h>
#       include <ctype.h>
#endif

#define CS_NAME    1
#define DESC_NM    2
#define DT_NAME    3
#define EL_NAME    4
#define ORG_LAT    5
#define ORG_LNG    6
#define SCL_RED    7
#define MAP_SCL    8
#define ZERO_X     9
#define ZERO_Y    10
#define PARM1     11
#define PARM2     12
#define PARM3     13
#define PARM4     14
#define PARM5     15
#define PARM6     16
#define PARM7     17
#define PARM8     18
#define PARM9     19
#define PARM10    20
#define PARM11    21
#define PARM12    22
#define PARM13    23
#define PARM14    24
#define PARM15    25
#define PARM16    26
#define PARM17    27
#define PARM18    28
#define PARM19    29
#define PARM20    30
#define PARM21    31
#define PARM22    32
#define PARM23    33
#define PARM24    34
#define X_OFF     35
#define Y_OFF     36
#define PROJ      37
#define UNIT      38
#define HGT_LNG   39
#define HGT_LAT   40
#define HGT_ZZ    41
#define MIN_LNG   42
#define MIN_LAT   43
#define MAX_LNG   44
#define MAX_LAT   45
#define MIN_XX    46
#define MIN_YY    47
#define MAX_XX    48
#define MAX_YY    49
#define QUAD      50
#define ORDER     51
#define ZONES     52
#define GROUP     53
#define LOCATION  54
#define COUNTRY   55
#define SOURCE    56
#define EPSG_NBR  57
#define SRID_NBR  58
#define EPSG_QD   59

struct cs_CscmpT_
{
	char label [16];
	int type;
};

__ALIGNMENT__1		/* For some versions of Sun compiler. */

static struct cs_CscmpT_ cs_CscmpT [] =
{
	{"\011LOCATION:" ,LOCATION},
	{"\010COUNTRY:"  ,COUNTRY},
	{"\010CS_NAME:"  ,CS_NAME},
	{"\010DESC_NM:"  ,DESC_NM},
	{"\010DT_NAME:"  ,DT_NAME},
	{"\010EL_NAME:"  ,EL_NAME},
	{"\010ORG_LAT:"  ,ORG_LAT},
	{"\010ORG_LNG:"  ,ORG_LNG},
	{"\010SCL_RED:"  ,SCL_RED},
	{"\010MAP_SCL:"  ,MAP_SCL},
	{"\010MIN_LNG:"  ,MIN_LNG},
	{"\010MIN_LAT:"  ,MIN_LAT},
	{"\010MAX_LNG:"  ,MAX_LNG},
	{"\010MAX_LAT:"  ,MAX_LAT},
	{"\010HGT_LNG:"  ,HGT_LNG},
	{"\010HGT_LAT:"  ,HGT_LAT},
	{"\007MIN_XX:"   ,MIN_XX},
	{"\007MIN_YY:"   ,MIN_YY},
	{"\007MAX_XX:"   ,MAX_XX},
	{"\007MAX_YY:"   ,MAX_YY},
	{"\007HGT_ZZ:"   ,HGT_ZZ},
	{"\007ZERO_X:"   ,ZERO_X},
	{"\007ZERO_Y:"   ,ZERO_Y},
	{"\007PARM10:"   ,PARM10},
	{"\007PARM11:"   ,PARM11},
	{"\007PARM12:"   ,PARM12},
	{"\007PARM13:"   ,PARM13},
	{"\007PARM14:"   ,PARM14},
	{"\007PARM15:"   ,PARM15},
	{"\007PARM16:"   ,PARM16},
	{"\007PARM17:"   ,PARM17},
	{"\007PARM18:"   ,PARM18},
	{"\007PARM19:"   ,PARM19},
	{"\007PARM20:"   ,PARM20},
	{"\007PARM21:"   ,PARM21},
	{"\007PARM22:"   ,PARM22},
	{"\007PARM23:"   ,PARM23},
	{"\007PARM24:"   ,PARM24},
	{"\007SOURCE:"   ,SOURCE},
	{"\006PARM1:"    ,PARM1},
	{"\006PARM2:"    ,PARM2},
	{"\006PARM3:"    ,PARM3},
	{"\006PARM4:"    ,PARM4},
	{"\006PARM5:"    ,PARM5},
	{"\006PARM6:"    ,PARM6},
	{"\006PARM7:"    ,PARM7},
	{"\006PARM8:"    ,PARM8},
	{"\006PARM9:"    ,PARM9},
	{"\006X_OFF:"    ,X_OFF},
	{"\006Y_OFF:"    ,Y_OFF},
	{"\006ORDER:"    ,ORDER},
	{"\006ZONES:"    ,ZONES},
	{"\006GROUP:"    ,GROUP},
	{"\005PROJ:"     ,PROJ},
	{"\005UNIT:"     ,UNIT},
	{"\005QUAD:"     ,QUAD},
	{"\005EPSG:"     ,EPSG_NBR},
	{"\005SRID:"     ,SRID_NBR},
	{"\010EPSG_QD:"  ,EPSG_QD},
	{""		 ,0}
};

static double novalue = -1.1E+22;
static double novalueTest = -1.0E+22;
static int line_nbr;

int CScsdefwr (	csFILE *outStrm,
				struct cs_Csdef_ *cs_def,
				int crypt,
				int warn,
				csFILE *elStrm,
				csFILE *dtStrm,
				int (*err_func)(char *mesg)
			  );

int EXP_LVL9 CScscomp (	Const char *inpt,
						Const char *outp,
						int flags,
						Const char *elipsoid,
						Const char *datum,
						int (*err_func)(char *)
					  )
{																/*lint !e578 */
#if _MEM_MODEL == _mm_VIRTUAL
	extern int cs_Sortbs;
#endif
	extern struct cs_Prjtab_ cs_Prjtab [];
	extern struct cs_Grptbl_ cs_CsGrptbl [];
	extern short cs_QuadMin;
	extern short cs_QuadMax;
	extern double cs_K360;
	extern double cs_Km360;
	extern double cs_MaxLat;
	extern double cs_MinLat;

	int st;
	int demo;
	int test;
	int warn;
	int crypt;
	int dummy;
	int cancel;
	int err_cnt;

	size_t rdCnt;
	size_t wrCnt;
	size_t strLen;
	
	long tmpLong;

	char *cp;
	char *trmPtr;
	csFILE *inStrm;
	csFILE *outStrm;
	csFILE *dtStrm;
	csFILE *elStrm;
	struct cs_Prjtab_ *pp;
	struct cs_CscmpT_ *tp;
	struct cs_Grptbl_ *gp;
	unsigned short* usPtr;

	cs_magic_t magic;
	
	char err_seg [18];
	char buff [256];
	char err_msg [256];
	char last_nm [48];

	__ALIGNMENT__2		/* For some versions of Sun compiler. */
	struct cs_Csdef_ cs_def;

#if _MEM_MODEL == _mm_VIRTUAL
	cs_Sortbs = (512 * 1024);
#endif

	demo = ((flags & cs_CMPLR_DEMO) != 0);
	crypt = ((flags & cs_CMPLR_CRYPT) != 0);
	test = ((flags & cs_CMPLR_TEST) != 0);
	warn = ((flags & cs_CMPLR_WARN) != 0);

	/* Open the source file. */
	inStrm = CS_fopen (inpt,_STRM_TXTRD);
	if (inStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for input.",inpt);
		(*err_func)(err_msg);
		return (1);
	}

	/* If we have been given a datum file name, we open it now. */
	if (datum != NULL && *datum != '\0')
	{
		dtStrm = CS_fopen (datum,_STRM_BINRD);
		if (dtStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as a datum file.",datum);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),dtStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) ||
			(demo && magic != cs_DTDEF_MAGIC) ||
			(!demo && magic != cs_DTDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not a Datum Dictionary file.",datum);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			CS_fclose (dtStrm);
			return (1);
		}
	}
	else
	{
		dtStrm = NULL;
	}

	/* If we have been given an ellipsoid file name, we open it now. */
	if (elipsoid != NULL && *elipsoid != '\0')
	{
		elStrm = CS_fopen (elipsoid,_STRM_BINRD);
		if (elStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as an Ellipsoid Dictionary.",elipsoid);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			if (dtStrm != NULL) CS_fclose (dtStrm);
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),elStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) ||
			(demo && magic != cs_ELDEF_MAGIC) ||
			(!demo && magic != cs_ELDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not an Elipsoid Dictionary file.",elipsoid);
			(*err_func)(err_msg);
			CS_fclose (inStrm);
			if (dtStrm != NULL) CS_fclose (dtStrm);
			CS_fclose (elStrm);
			return (1);
		}
	}
	else
	{
		elStrm = NULL;
	}

	/* Open the output file and write the magic number.  BINRW creates
	   a new file with read/write access.  We need read/write as we
	   will need to dort the result. */
	outStrm = CS_fopen (outp,_STRM_BINRW);
	if (outStrm == NULL)
	{
		sprintf (err_msg,"Couldn't open %s for output.",outp);
		(*err_func)(err_msg);
		CS_fclose (inStrm);
		if (dtStrm != NULL) CS_fclose (dtStrm);
		if (elStrm != NULL) CS_fclose (elStrm);
		return (1);
	}

	/* Write the magic number. */
	if (demo) magic = cs_CSDEF_MAGIC;
	else      magic = cs_CSDEF_MAGIC;
	CS_bswap (&magic,"l");
	wrCnt = CS_fwrite ((char *)&magic,1,sizeof (magic),outStrm);
	if (wrCnt != sizeof (magic))
	{
		sprintf (err_msg,"Failure detected writing to %s.",outp);
		(*err_func)(err_msg);
		CS_fclose (outStrm);
		CS_fclose (inStrm);
		if (dtStrm != NULL) CS_fclose (dtStrm);
		if (elStrm != NULL) CS_fclose (elStrm);
		CS_remove (outp);
		return (1);
	}

	/* Process each line in the source file. */
	err_cnt = 0;
	line_nbr = 0;
	cancel = FALSE;
	memset (&cs_def,'\0',sizeof (cs_def));
	cs_def.scl_red = 1.0;
	cs_def.map_scl = 1.0;
	cs_def.unit_scl = 1.0;
	cs_def.scale = 1.0;
	cs_def.quad = 1;
	cs_def.ll_min [LNG] = novalue;
	cs_def.ll_min [LAT] = novalue;
	cs_def.ll_max [LNG] = novalue;
	cs_def.ll_max [LAT] = novalue;
	cs_def.xy_min [XX] = novalue;
	cs_def.xy_min [YY] = novalue;
	cs_def.xy_max [XX] = novalue;
	cs_def.xy_max [YY] = novalue;

	while (CS_fgets (buff,sizeof (buff),inStrm) != NULL)
	{
		if (cancel)
		{
			cs_def.key_nm [0] = '\0';
			break;
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
				*(cp - 1) != '\\')			/* backslash here is an escape character */
			{
				*cp = '\0';
				break;
			}
		}

		/* Run the line through the table parser.  We
		   see what type of line we have. */
		cp = buff;
		tp = (struct cs_CscmpT_ *)CS_tpars (&cp,(char *)cs_CscmpT,sizeof (struct cs_CscmpT_));

		if (tp == NULL)
		{
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Unknonwn line type: %s, at line %d.",
							err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			cs_def.key_nm [0] = '\0';
			err_cnt += 1;
			continue;
		}

		/* Here when we have a valid line. Cp points to the first
		   character after the colon. */
		CS_trim (cp);
		switch (tp->type) {

		case CS_NAME:
			/* Here each time we encounter a new coordinate
			   system name.  We see if there is an existing
			   coordinate system which must be written. */

			if (cs_def.key_nm [0] != '\0' &&   /* Skip first.*/
				(test || CS_stricmp (cs_def.group,"TEST")))
			{
				st = CScsdefwr (outStrm,&cs_def,crypt,warn,dtStrm,elStrm,err_func);
				if (st >= 0) err_cnt += st;
				else
				{
					cancel = TRUE;
					err_cnt += -st;
				}
			}

			/* Prepare the structure for the next
			   definition. */
			memset (&cs_def,'\0',sizeof (cs_def));

			cs_def.scl_red = 1.0;
			cs_def.map_scl = 1.0;
			cs_def.unit_scl = 1.0;
			cs_def.scale = 1.0;
			cs_def.quad = 1;
			cs_def.ll_min [LNG] = novalue;
			cs_def.ll_min [LAT] = novalue;
			cs_def.ll_max [LNG] = novalue;
			cs_def.ll_max [LAT] = novalue;
			cs_def.xy_min [XX] = novalue;
			cs_def.xy_min [YY] = novalue;
			cs_def.xy_max [XX] = novalue;
			cs_def.xy_max [YY] = novalue;

			strLen = strlen (cp);
			CS_stncp (cs_def.key_nm,cp,sizeof (cs_def.key_nm));
			st = CS_nampp (cs_def.key_nm);
			if (strLen > cs_KEYNM_MAX)
			{
				sprintf (err_msg,"Key name %s exceeds maximun length of %d; line %d.",cp,cs_KEYNM_MAX,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid coordinate system key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case GROUP:
			for (gp = cs_CsGrptbl;gp->group [0] != '\0';gp += 1)
			{
				if (!CS_stricmp (gp->group,cp)) break;
			}
			if (gp->group [0] == '\0')
			{
				sprintf (err_msg,"Invalid group (%s) specified at line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				CS_stncp (cs_def.group,cp,sizeof (cs_def.group));
			}
			break;

		case LOCATION:
			if (strlen (cp) >= sizeof (cs_def.locatn))
			{
				sprintf (err_msg,"Location description for %s at line %d is too long.\n",
						cs_def.key_nm,line_nbr);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (cs_def.locatn,cp,sizeof (cs_def.locatn));
			break;

		case COUNTRY:
			if (strlen (cp) >= sizeof (cs_def.cntry_st))
			{
				sprintf (err_msg,"Country/State enumeration for %s at line %d is too long.\n",
						cs_def.key_nm,line_nbr);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (cs_def.cntry_st,cp,sizeof (cs_def.cntry_st));
			break;

		case DESC_NM:
			if (strlen (cp) >= sizeof (cs_def.desc_nm))
			{
				sprintf (err_msg,"Descriptive name for %s at line %d is too long.\n",
						cs_def.key_nm,line_nbr);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (cs_def.desc_nm,cp,sizeof (cs_def.desc_nm));
			break;

		case PROJ:
			CS_stncp (cs_def.prj_knm,cp,sizeof (cs_def.prj_knm));
			for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
			{
				if (!CS_stricmp (pp->key_nm,cs_def.prj_knm)) break;
			}

			/* Make sure that we found one. */
			if (*pp->key_nm == '\0')
			{
				sprintf (err_msg,"Unsupported projection type %s at line %d.",
						cs_def.prj_knm,line_nbr);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
				cancel = (*err_func)(err_msg);
			}
			break;

		case DT_NAME:
			CS_stncp (cs_def.dat_knm,cp,sizeof (cs_def.dat_knm));
			break;

		case EL_NAME:
			CS_stncp (cs_def.elp_knm,cp,sizeof (cs_def.elp_knm));
			break;

		case UNIT:
			CS_stncp (cs_def.unit,cp,sizeof (cs_def.unit));
			break;

		case PARM1:  CS_atof (&cs_def.prj_prm1, cp); break;
		case PARM2:  CS_atof (&cs_def.prj_prm2, cp); break;
		case PARM3:  CS_atof (&cs_def.prj_prm3, cp); break;
		case PARM4:  CS_atof (&cs_def.prj_prm4, cp); break;
		case PARM5:  CS_atof (&cs_def.prj_prm5, cp); break;
		case PARM6:  CS_atof (&cs_def.prj_prm6, cp); break;
		case PARM7:  CS_atof (&cs_def.prj_prm7, cp); break;
		case PARM8:  CS_atof (&cs_def.prj_prm8, cp); break;
		case PARM9:  CS_atof (&cs_def.prj_prm9, cp); break;
		case PARM10: CS_atof (&cs_def.prj_prm10,cp); break;
		case PARM11: CS_atof (&cs_def.prj_prm11,cp); break;
		case PARM12: CS_atof (&cs_def.prj_prm12,cp); break;
		case PARM13: CS_atof (&cs_def.prj_prm13,cp); break;
		case PARM14: CS_atof (&cs_def.prj_prm14,cp); break;
		case PARM15: CS_atof (&cs_def.prj_prm15,cp); break;
		case PARM16: CS_atof (&cs_def.prj_prm16,cp); break;
		case PARM17: CS_atof (&cs_def.prj_prm17,cp); break;
		case PARM18: CS_atof (&cs_def.prj_prm18,cp); break;
		case PARM19: CS_atof (&cs_def.prj_prm19,cp); break;
		case PARM20: CS_atof (&cs_def.prj_prm20,cp); break;
		case PARM21: CS_atof (&cs_def.prj_prm21,cp); break;
		case PARM22: CS_atof (&cs_def.prj_prm22,cp); break;
		case PARM23: CS_atof (&cs_def.prj_prm23,cp); break;
		case PARM24: CS_atof (&cs_def.prj_prm24,cp); break;

		case ORG_LNG:
			CS_atof (&cs_def.org_lng,cp);
			if (cs_def.org_lng <= -180.0 ||
				cs_def.org_lng > 180.0)
			{
				sprintf (err_msg,"Invalid longitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case ORG_LAT:
			CS_atof (&cs_def.org_lat,cp);
			if (cs_def.org_lat < -90.0 ||
				cs_def.org_lat >  90.0)
			{
				sprintf (err_msg,"Invalid latitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case X_OFF:
			CS_atof (&cs_def.x_off,cp);
			break;

		case Y_OFF:
			CS_atof (&cs_def.y_off,cp);
			break;

		case SCL_RED:
			CS_atof (&cs_def.scl_red,cp);
			if (cs_def.scl_red > 1.1 || cs_def.scl_red < 0.75)
			{
				sprintf (err_msg,"Invalid scale reduction on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case MAP_SCL:
			CS_atof (&cs_def.map_scl,cp);
			break;

		case ZERO_X:
			CS_atof (&cs_def.zero [0],cp);
			break;

		case ZERO_Y:
			CS_atof (&cs_def.zero [1],cp);
			break;

		case HGT_LNG:
			CS_atof (&cs_def.hgt_lng,cp);
			break;

		case HGT_LAT:
			CS_atof (&cs_def.hgt_lat,cp);
			break;

		case HGT_ZZ:
			CS_atof (&cs_def.hgt_zz,cp);
			break;

		case MIN_LNG:
			CS_atof (&cs_def.ll_min [LNG],cp);
			if (cs_def.ll_min [LNG] <= cs_Km360 ||
				cs_def.ll_min [LNG] >  cs_K360)
			{
				sprintf (err_msg,"Invalid longitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case MIN_LAT:
			CS_atof (&cs_def.ll_min [LAT],cp);
			if (cs_def.ll_min [LAT] < cs_MinLat ||
				cs_def.ll_min [LAT] > cs_MaxLat)
			{
				sprintf (err_msg,"Invalid latitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case MAX_LNG:
			CS_atof (&cs_def.ll_max [LNG],cp);
			if (cs_def.ll_max [LNG] <= cs_Km360 ||
				cs_def.ll_max [LNG] >  cs_K360)
			{
				sprintf (err_msg,"Invalid longitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case MAX_LAT:
			CS_atof (&cs_def.ll_max [LAT],cp);
			if (cs_def.ll_max [LAT] < cs_MinLat ||
				cs_def.ll_max [LAT] > cs_MaxLat)
			{
				sprintf (err_msg,"Invalid latitude on line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
				cs_def.key_nm [0] = '\0';
			}
			break;

		case MIN_XX:
			CS_atof (&cs_def.xy_min [XX],cp);
			break;

		case MIN_YY:
			CS_atof (&cs_def.xy_min [YY],cp);
			break;

		case MAX_XX:
			CS_atof (&cs_def.xy_max [XX],cp);
			break;

		case MAX_YY:
			CS_atof (&cs_def.xy_max [YY],cp);
			break;

		case QUAD:
			cs_def.quad = (short)atoi (cp);
			if (cs_def.quad < cs_QuadMin ||
				cs_def.quad > cs_QuadMax)
			{
				sprintf (err_msg,"Invalid quadrant specification (%d) at line %d.",cs_def.quad,line_nbr);
				cancel = (*err_func)(err_msg);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
			}
			break;

		case ORDER:
			cs_def.order = (short)atoi (cp);
			if (cs_def.order < 0 || cs_def.order > 12)
			{
				sprintf (err_msg,"Invalid order specification (%d) at line %d.",cs_def.order,line_nbr);
				cancel = (*err_func)(err_msg);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
			}
			break;

		case ZONES:
			cs_def.zones = (short)atoi (cp);
			if (cs_def.zones < 0 || cs_def.zones > 8)
			{
				sprintf (err_msg,"Invalid ZONES specification (%d) at line %d.",cs_def.zones,line_nbr);
				cancel = (*err_func)(err_msg);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
			}
			break;

		case SOURCE:
			if (strlen (cp) >= sizeof (cs_def.source))
			{
				sprintf (err_msg,"Source description for %s at line %d is too long.\n",cs_def.key_nm,line_nbr);
				cs_def.key_nm [0] = '\0';
				err_cnt += 1;
				cancel = (*err_func)(err_msg);
			}
			CS_stncp (cs_def.source,cp,sizeof (cs_def.source));
			break;

		case EPSG_NBR:
			cs_def.epsgNbr = (short)atoi (cp);
			cs_def.epsgNbr = 0;
			tmpLong = strtol (cp,&trmPtr,10);
			if (tmpLong > 0 && tmpLong < 65536)
			{
				usPtr = (unsigned short*)(&cs_def.epsgNbr);
				*usPtr = (unsigned short)tmpLong;
			}
			break;

		case SRID_NBR:
			cs_def.srid = 0;
			tmpLong = strtol (cp,&trmPtr,10);
			if (tmpLong > 0 && tmpLong < 65536)
			{
				usPtr = (unsigned short*)(&cs_def.srid);
				*usPtr = (unsigned short)tmpLong;
			}
			break;

		case EPSG_QD:
			cs_def.epsg_qd = (short)atoi (cp);
			break;

		default:
			sprintf (err_msg,"Software problem detected at line %d.",__LINE__);
			cancel = (*err_func)(err_msg);
			cs_def.key_nm [0] = '\0';
			err_cnt += 1;
			break;
		}
		/* On to the next input line. */
	}
	CS_fclose (inStrm);

	/* Write out the last coordinate system if not in error. */
	if (cs_def.key_nm [0] != '\0')
	{
		st = CScsdefwr (outStrm,&cs_def,crypt,warn,dtStrm,elStrm,err_func);
		if (st >= 0) err_cnt += st;
		else
		{
			err_cnt += -st;
			cancel = TRUE;
		}
	}
	if (dtStrm != NULL)
	{
		CS_fclose (dtStrm);
		dtStrm = NULL;
	}
	if (elStrm != NULL)
	{
		CS_fclose (elStrm);
		elStrm = NULL;
	}

	if (cancel)
	{
		CS_fclose (outStrm);
		CS_remove (outp);
		return (err_cnt);
	}

	/* Sort the file. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_ips (outStrm,sizeof (cs_def),(long32_t)0,(CMPFUNC_CAST)CS_cscmp);

	/* Scan through the sorted file and see if there are any duplicate
	   names. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_csrd (outStrm,&cs_def,&dummy);
	CS_stncp (last_nm,cs_def.key_nm,sizeof (last_nm));
	while (!cancel && CS_csrd (outStrm,&cs_def,&dummy))
	{
		if (!CS_stricmp (cs_def.key_nm,last_nm))
		{
			sprintf (err_msg,"Key name %s is used more than once.",last_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		CS_stncp (last_nm,cs_def.key_nm,sizeof (last_nm));
	}
	CS_fclose (outStrm);

	/* Delete the ouput if there were errors. */
	if (err_cnt != 0)
	{
		CS_remove (outp);
	}

	/* That's it. */
	return (err_cnt);
}

/*
	Common code for checking and writting a coordinate system
	definition.
*/

int CScsdefwr (	csFILE *outStrm,
				struct cs_Csdef_ *cs_def,
				int crypt,
				int warn,
				csFILE *dtStrm,
				csFILE *elStrm,
				int (*err_func)(char *mesg)
			  )
{
	extern double cs_Zero;
	extern char csErrnam [];	/* Expected to be dimensioned at MAXPATH */
	extern struct cs_Prjtab_ cs_Prjtab [];

	int st;
	int ii;
	int lclCrypt;
	int flag;
	int chk_cnt;
	int err_cnt;
	int cancel;
	int list_sz;
	int mm_cnt;

	struct cs_Prjtab_ *pp;

	double unit_k;

	char err_msg [256];
	int err_list [12];

 	__ALIGNMENT__2		/* For some versions of Sun compiler. */
	struct cs_Dtdef_ dt_def;
 	__ALIGNMENT__3		/* For some versions of Sun compiler. */
	struct cs_Eldef_ el_def;

	
	list_sz = sizeof (err_list) / sizeof (int);
	for (ii = 0;ii < list_sz;ii += 1) err_list [ii] = 0;
	err_cnt = 0;
	cancel = FALSE;
	if (0!=CS_stricmp(cs_def->prj_knm, "NERTH"))
	{
		if (cs_def->dat_knm [0] == '\0' &&
			cs_def->elp_knm [0] == '\0')
		{
			sprintf (err_msg,"Neither datum or ellipsoid specified for coordinate system %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			cs_def->key_nm [0] = '\0';
			err_cnt += 1;
		}
		if (cs_def->dat_knm [0] != '\0' &&
			cs_def->elp_knm [0] != '\0')
		{
			sprintf (err_msg,"Datum AND ellipsoid specified for coordinate system %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	if (dtStrm != NULL && cs_def->dat_knm [0] != '\0')
	{
		CS_stncp (dt_def.key_nm,cs_def->dat_knm,sizeof (dt_def.key_nm));
		CS_nampp (dt_def.key_nm);
		dt_def.fill [0] = '\0';
		dt_def.fill [1] = '\0';
		flag = CS_bins (dtStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (dt_def),&dt_def,(CMPFUNC_CAST)CS_dtcmp);
		if (flag == 1)
		{
			st = CS_dtrd (dtStrm,&dt_def,&lclCrypt);
			if (st == 1)
			{
				if (!CS_stricmp (dt_def.group,"LeGACY") && CS_stricmp (cs_def->group,"LEGACY"))
				{
					if (warn)
					{
						sprintf (err_msg,"Non-legacy system named %s references legacy datum named %s.",cs_def->key_nm,dt_def.key_nm);
						cancel = (*err_func)(err_msg);
					}
				}
			}
		}
		else if (flag == 0)
		{
			sprintf (err_msg,"Invalid datum key name, %s, given on line %d.",cs_def->dat_knm,line_nbr);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		else
		{
			sprintf (err_msg,"Datum dictionary access failure detected when checking system named %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	if (elStrm != NULL && cs_def->elp_knm [0] != '\0')
	{
		CS_stncp (el_def.key_nm,cs_def->elp_knm,sizeof (el_def.key_nm));
		CS_nampp (el_def.key_nm);
		el_def.fill [0] = '\0';
		el_def.fill [1] = '\0';
		flag = CS_bins (elStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (el_def),&el_def,(CMPFUNC_CAST)CS_elcmp);
		if (flag == 1)
		{
			st = CS_elrd (elStrm,&el_def,&lclCrypt);
			if (st == 1)
			{
				if (!CS_stricmp (el_def.group,"LGACY") && CS_stricmp (cs_def->group,"LEGACY"))
				{
					if (warn)
					{
						sprintf (err_msg,"Non-legacy system named %s references legacy ellipsoid named %s.",cs_def->key_nm,el_def.key_nm);
						cancel = (*err_func)(err_msg);
					}
				}
			}
		}
		else if (!flag)
		{
			sprintf (err_msg,"Invalid ellipsoid key name, %s, given on line %d.",cs_def->elp_knm,line_nbr);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		else
		{
			sprintf (err_msg,"Ellipsoid dictionary access failure detected when checking system named %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	if (cs_def->unit [0] == '\0')
	{
		sprintf (err_msg,"No unit name provided for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}

	/* Now, we check the useful limits. */
	mm_cnt = 0;
	if (cs_def->ll_min [LNG] > novalueTest) mm_cnt += 1;
	if (cs_def->ll_min [LAT] > novalueTest) mm_cnt += 1;
	if (cs_def->ll_max [LNG] > novalueTest) mm_cnt += 1;
	if (cs_def->ll_max [LAT] > novalueTest) mm_cnt += 1;
	if (mm_cnt == 0)
	{
		cs_def->ll_min [LNG] = cs_Zero;
		cs_def->ll_min [LAT] = cs_Zero;
		cs_def->ll_max [LNG] = cs_Zero;
		cs_def->ll_max [LAT] = cs_Zero;
	}
	else if (mm_cnt == 4)
	{
		if (cs_def->ll_min [LNG] >= cs_def->ll_max [LNG])
		{
			sprintf (err_msg,"Minimum longitude is not less than maximum for %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		if (cs_def->ll_min [LAT] >= cs_def->ll_max [LAT])
		{
			sprintf (err_msg,"Minimum latitude is not less than maximum for %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	else
	{
		sprintf (err_msg,"Geographic useful range incomplete for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}

	mm_cnt = 0;
	if (cs_def->xy_min [XX] > novalueTest) mm_cnt += 1;
	if (cs_def->xy_min [YY] > novalueTest) mm_cnt += 1;
	if (cs_def->xy_max [XX] > novalueTest) mm_cnt += 1;
	if (cs_def->xy_max [YY] > novalueTest) mm_cnt += 1;
	if (mm_cnt == 0)
	{
		cs_def->xy_min [XX] = cs_Zero;
		cs_def->xy_min [YY] = cs_Zero;
		cs_def->xy_max [XX] = cs_Zero;
		cs_def->xy_max [YY] = cs_Zero;
	}
	else if (mm_cnt == 4)
	{
		if (cs_def->xy_min [XX] >= cs_def->xy_max [XX])
		{
			sprintf (err_msg,"Minimum useful X is not less than maximum for %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		if (cs_def->xy_min [YY] >= cs_def->xy_max [YY])
		{
			sprintf (err_msg,"Minimum useful Y is not less than maximum for %s.",cs_def->key_nm);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	else
	{
		sprintf (err_msg,"Cartesian useful range incomplete for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
		err_cnt += 1;
	}

	chk_cnt = 0;
	if (0!=CS_stricmp(cs_def->prj_knm, "NERTH"))
	{
		/* Now, we can do the ultimate check, sans the datum and
		   ellipsoid checks which we did above.  We need the above
		   checks as the dictionaries we are providing may not be
		   the same as those which the CS-MAP library routines may
		   think are the current ones. */
		list_sz = sizeof (err_list) / sizeof (int);
		chk_cnt = CS_cschk (cs_def,0,err_list,list_sz);
		if (chk_cnt == 1 && list_sz > 0 && err_list [0] == cs_SYS34_NOSRC)	/*lint !e774 */
		{
			if (warn)
			{
				CSerpt (err_msg,sizeof (err_msg),err_list [0]);
				sprintf (err_msg,"Skipping definition of %s.  Reason: %s",cs_def->key_nm,err_msg);
			}
			err_cnt += 1;
		}
		else if (chk_cnt != 0)
		{
			CS_stncp (csErrnam,cs_def->key_nm,MAXPATH);
			for (ii = 0;ii < chk_cnt && ii < list_sz;ii++)
			{
				CSerpt (err_msg,sizeof (err_msg),err_list [ii]);
				cancel = (*err_func)(err_msg);
			}
			err_cnt += chk_cnt;
		}
	}

	if (chk_cnt == 0)
	{
		/* If we ever get here, the unit will have already been verified.
		   However, since CS_cschk is used elsewhere in the system, it would
		   be inappropriate for it to set the unit_scl. So we do all of this
		   again. We issue the message just in case some one changes
		   CS_cschk and doesn't know that this module was relying on it. */
		for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
		{
			if (!CS_stricmp (pp->key_nm,cs_def->prj_knm))
			{
				if (pp->code == cs_PRJCOD_UNITY)
				{
					unit_k = CS_unitlu (cs_UTYP_ANG,cs_def->unit);
				}
				else
				{
					unit_k = CS_unitlu (cs_UTYP_LEN,cs_def->unit);
				}
				if (unit_k == 0.0)
				{
					sprintf (err_msg,"Unit specification specified in system named %s.",cs_def->key_nm);
					cancel = (*err_func)(err_msg);
					err_cnt += 1;
				}
				else
				{
					cs_def->unit_scl = unit_k;
				}
				break;
			}
		}
	}

	/* If error count is still zero, we can write this thing out. */

	if (err_cnt == 0)
	{
		cs_def->protect = TRUE;
		/* The following sets the "creation date" to Jan 1, 2002 (approx) for
		   all test coordinate systems.  This enables the protection system to
		   be tested.  Normally, distribution definitions are protected from
		   any kind of change. */
		if (!CS_stricmp (cs_def->group,"TEST")) cs_def->protect = 4383;
		cs_def->fill [0] = '\0';
		cs_def->fill [1] = '\0';
		CS_cswr (outStrm,cs_def,crypt);
	}
	if (warn && cs_def->desc_nm [0] == '\0')
	{
		sprintf (err_msg,"Warning: No description provided for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (warn && cs_def->source [0] == '\0')
	{
		sprintf (err_msg,"Warning: No source provided for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
	}
	if (warn && cs_def->group [0] == '\0')
	{
		sprintf (err_msg,"Warning: No group provided for %s.",cs_def->key_nm);
		cancel = (*err_func)(err_msg);
	}

	if (err_cnt == 1 && list_sz > 0 && err_list [0] == cs_SYS34_NOSRC)		/*lint !e774  boolean always evaluates to true */
	{
		err_cnt = 0;
	}
	if (cancel && err_cnt == 0) err_cnt = 1;
	return (cancel ? -err_cnt : err_cnt);
}
