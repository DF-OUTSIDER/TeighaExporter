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

/*lint -esym(767,GX_NAME,SRC_DTM,TRG_DTM,GROUP,DESC_NM,SOURCE,METHOD,EPSG_NBR,EPSG_VAR,INVERSE)  possibly different values in other modules */
/*lint -esym(767,MAX_ITR,CNVRG_VAL,ERROR_VAL,ACCURACY,MIN_LNG,MAX_LNG,MIN_LAT,MAX_LAT,FALLBACK)  possibly different values in other modules */
/*lint -esym(767,DELTA_X,DELTA_Y,DELTA_Z,ROT_X,ROT_Y,ROT_Z,BWSCALE,XLATE_X,XLATE_Y,XLATE_Z)      possibly different values in other modules */
/*lint -esym(767,VALIDATION,TEST_LNG,TEST_LAT,RSLT_DELTA_LNG,RSLT_DELTA_LAT,RSLT_DELTA_HGT)      possibly different values in other modules */
/*lint -esym(767,SRC_LAT_OFF,SRC_LNG_OFF,SRC_HGT_OFF,TRG_LAT_OFF,TRG_LNG_OFF,TRG_HGT_OFF)        possibly different values in other modules */
/*lint -esym(767,NRML_KK,LAT_COEF,LNG_COEF,HGT_COEF)  different values in other modules */
/*lint -esym(534,err_func)   ignoring return value */
/*lint -esym(754,cs_GxCmpT_::label)  not referenced directly, only indirectly */

/* Common attributes of a Geodetic Transform */
#define GX_NAME     1
#define SRC_DTM     2
#define TRG_DTM     3
#define GROUP       4
#define DESC_NM     5
#define SOURCE      6
#define METHOD      7
#define EPSG_NBR    8
#define EPSG_VAR    9
#define INVERSE    10
#define MAX_ITR    11
#define CNVRG_VAL  12
#define ERROR_VAL  13
#define ACCURACY   14
#define MIN_LNG    15
#define MAX_LNG    16
#define MIN_LAT    17
#define MAX_LAT    18
#define FALLBACK   19

/* Attributes related to the Geocentric Algorithms */
#define DELTA_X   101
#define DELTA_Y   102
#define DELTA_Z   103
#define ROT_X     104
#define ROT_Y     105
#define ROT_Z     106
#define BWSCALE   107
#define XLATE_X   108
#define XLATE_Y   109
#define XLATE_Z   110

/* Attributes related to the Multiple Regression/Power series Algorithms.
   Note, these algorithms are very similar.  AT the time of this writting,
   it is presumed that the two algoithms can be made identical by (during
   initialization of the multiple regression algorithm) setting the
   TRG_LAT_OFF and TRG_LNG_OFF values to be equal to the SRC_LAT_OFF and
   SRC_LNG_OFF values respectively.  Once we get to a point where we're
   actually generating numbers, we'll find outif we are correct. */
#define VALIDATION       201		/* value of normaized horizontal coordinates which
									   marks the extents of the useful range; typically
									   1.0 */
#define TEST_LNG         202		/* longitude of the built in test case */
#define TEST_LAT         203		/* latitude of the built in test case */
#define RSLT_DELTA_LNG   204		/* test case: expected delta longitude */
#define RSLT_DELTA_LAT   205		/* test case: expected delta latitude */
#define RSLT_DELTA_HGT   206		/* test case: expected delta height */
#define SRC_LAT_OFF      207		/* normalization offset for source latitudes */
#define SRC_LNG_OFF      208		/* normalization offset for source longitudes */
#define SRC_HGT_OFF      209		/* not used */
#define TRG_LAT_OFF      210		/* unnormalization offset for target latitudes */
#define TRG_LNG_OFF      211		/* unnormalization offset for target longitudes */
#define TRG_HGT_OFF      212		/* not used */
#define NRML_KK          213		/* normalization scale factor */
#define LAT_COEF         214		/* preamble for a latitude power series coefficient */
#define LNG_COEF         215		/* preamble for a longitude power series coefficient */
#define HGT_COEF         216		/* preamble for a height power series coeffificent */

/* Attributes for the grid interpolation data file approach. */
#define GRID_FILE        301		/* a grid file specification, includes format and direction */
									
struct cs_GxCmpT_
{
	char label [24];
	int type;
};

#if defined (_MSC_VER) && _MSC_VER >= 800	/* MS Visual C++ 1.0 or later */
#	pragma warning(push)
#	pragma warning(disable : 4125)		/* decimal digit terminates octal escape sequence */
#endif

__ALIGNMENT__1		/* For some versions of Sun compiler. */
static struct cs_GxCmpT_ cs_GxCmpT [] =
{
	{  "\006GROUP:",            GROUP           },
	{  "\006ROT_X:",            ROT_X           },
	{  "\006ROT_Y:",            ROT_Y           },
	{  "\006ROT_Z:",            ROT_Z           },
	{  "\007SOURCE:",           SOURCE          },
	{  "\007METHOD:",           METHOD          },
	{  "\010GX_NAME:",          GX_NAME         },
	{  "\010SRC_DTM:",          SRC_DTM         },
	{  "\010TRG_DTM:",          TRG_DTM         },
	{  "\010DESC_NM:",          DESC_NM         },
	{  "\010INVERSE:",          INVERSE         },
	{  "\010MAX_ITR:",          MAX_ITR         },
	{  "\010DELTA_X:",          DELTA_X         },
	{  "\010DELTA_Y:",          DELTA_Y         },
	{  "\010DELTA_Z:",          DELTA_Z         },
	{  "\010BWSCALE:",          BWSCALE         },
	{  "\010XLATE_X:",          XLATE_X         },
	{  "\010XLATE_Y:",          XLATE_Y         },
	{  "\010XLATE_Z:",          XLATE_Z         },
	{  "\010LAT_COEF",          LAT_COEF        },
	{  "\010LNG_COEF",          LNG_COEF        },
	{  "\010HGT_COEF",          HGT_COEF        },
	{  "\010NRML_KK:",          NRML_KK         },
	{  "\010MIN_LNG:",          MIN_LNG         },
	{  "\010MAX_LNG:",          MAX_LNG         },
	{  "\010MIN_LAT:",          MIN_LAT         },
	{  "\010MAX_LAT:",          MAX_LAT         },
	{  "\011ACCURACY:",         ACCURACY        },
	{  "\011FALLBACK:",         FALLBACK        },
	{  "\011EPSG_NBR:",         EPSG_NBR        },
	{  "\011EPSG_VAR:",         EPSG_VAR        },
	{  "\011TEST_LNG:",         TEST_LNG        },
	{  "\011TEST_LAT:",         TEST_LAT        },
	{  "\012GRID_FILE:",        GRID_FILE       },
	{  "\012CNVRG_VAL:",        CNVRG_VAL       },
	{  "\012ERROR_VAL:",        ERROR_VAL       },
	{  "\013VALIDATION:",       VALIDATION      },
	{  "\014SRC_LAT_OFF:",      SRC_LAT_OFF     },
	{  "\014SRC_LNG_OFF:",      SRC_LNG_OFF     },
	{  "\014SRC_HGT_OFF:",      SRC_HGT_OFF     },
	{  "\014TRG_LAT_OFF:",      TRG_LAT_OFF     },
	{  "\014TRG_LNG_OFF:",      TRG_LNG_OFF     },
	{  "\014TRG_HGT_OFF:",      TRG_HGT_OFF     },
	{  "\017RSLT_DELTA_LNG:",   RSLT_DELTA_LNG  },
	{  "\017RSLT_DELTA_LAT:",   RSLT_DELTA_LAT  },
	{  "\017RSLT_DELTA_HGT:",   RSLT_DELTA_HGT  },
	{  "",                      0               }
};

__ALIGNMENT__2		/* For some versions of Sun compiler. */
static struct cs_GxCmpT_ cs_GxMthdT [] =
{
	{    "\004NULL",           cs_DTCMTH_NULLX},
	{    "\005WGS72",          cs_DTCMTH_WGS72},
	{    "\012MOLODENSKY",     cs_DTCMTH_MOLOD},
	{    "\012GEOCENTRIC",     cs_DTCMTH_GEOCT},
	{    "\0124PARAMETER",     cs_DTCMTH_4PARM},
	{    "\0126PARAMETER",     cs_DTCMTH_6PARM},
	{    "\011BURSAWOLF",      cs_DTCMTH_BURSA},
	{    "\011BURSAFRAME",     cs_DTCMTH_FRAME},
	{    "\0127PARAMETER",     cs_DTCMTH_7PARM},
	{    "\013MOLOBADEKAS",    cs_DTCMTH_BDKAS},
	{    "\006MULREG",         cs_DTCMTH_MULRG},
	{    "\006POLYNM",         cs_DTCMTH_PLYNM},
	{    "\011GRID_INTERP",    cs_DTCMTH_GFILE},
	{    "\0123PARAMETER",     cs_DTCMTH_3PARM},
	{    "",                   cs_DTCMTH_NONE}
};

__ALIGNMENT__3		/* For some versions of Sun compiler. */
static struct cs_GxCmpT_ cs_GxFrmtT [] =
{
	{   "\004NTv1",      cs_DTCFRMT_CNTv1},
	{   "\004NTv2",      cs_DTCFRMT_CNTv2},
	{   "\006NADCON",    cs_DTCFRMT_NADCN},
	{   "\005FRRGF",     cs_DTCFRMT_FRNCH},
	{   "\005JPPAR",     cs_DTCFRMT_JAPAN},
	{   "\005ATS77",     cs_DTCFRMT_ATS77},
	{   "\006OSTN97",    cs_DTCFRMT_OST97},
	{   "\006OSTN02",    cs_DTCFRMT_OST02},
	{   "\006GEOCON",    cs_DTCFRMT_GEOCN},
	{   "",              cs_DTCFRMT_NONE}
};
#if defined (_MSC_VER) && _MSC_VER >= 800	/* MS Visual C++ 1.0 or later */
#	pragma warning(pop)
#endif

int CSgxdefwr (	csFILE *outStrm,
				struct cs_GeodeticTransform_ *gxdef,
				int warn,
				csFILE* dtmStrm,
				int (*err_func)(char *mesg)
			  );

/**********************************************************************
**	err_cnt = CSgxcomp (inpt,outp,flags,datum,err_func);
**
**	char *inpt;					full path name to the ASCII source file.
**	char *outp;					full path name to the target file.
**	int flags;					bitmap of options, see REMARKS below.
**	char *datum;				if non-null, indicates the full path name of the
**								datums file which is to be used to verify
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

/*lint -esym(550,mulregCount)      variable not accessed; but could valuable in future */
/*lint -esym(550,gridFileCount)    variable not accessed; but could valuable in future */
/*lint -esym(550,geocentricCount)  variable not accessed; but could valuable in future */
/*lint -esym(550,currentMethod)    variable not accessed; but could valuable in future */
int EXP_LVL9 CSgxcomp (	Const char *inpt,
						Const char *outp,
						int flags,
						char *datum,
						int (*err_func)(char *mesg)
					  )
{
	int st;
	int ii;
	int jj;
	int cnt;
	int idx;
	int test;
	int warn;
	int cancel;
	int line_nbr;
	int err_cnt;
	int currentMethod;
	int geocentricCount;
	int mulregCount;
	int gridFileCount;
	int gridFileFormat;

	size_t rdCnt;
	size_t wrCnt;

	char *cp;
	char *dummy;
	char *cpDir;
	char* cpFile;
	csFILE *inStrm;
	csFILE *outStrm;
	csFILE *dtmStrm;
	struct cs_GxCmpT_ *tp;
	struct cs_GxCmpT_ *mp;
	struct cs_GxCmpT_ *fp;

	cs_magic_t magic;

	double coef;
	double tmpDbl;

	char err_seg [18];
	char buff [128];
	char err_msg [128];
	char last_name [128];

	__ALIGNMENT__2					/* For some versions of Sun compiler. */
	struct cs_GeodeticTransform_ gxdef;

	memset ((void*)&gxdef,0,sizeof (gxdef));
	currentMethod = 0;
	geocentricCount = 0;
	mulregCount = 0;
	gridFileCount = 0;

	/* Process the arguments on the command line. */
	test = ((flags & cs_CMPLR_TEST) != 0);
	warn = ((flags & cs_CMPLR_WARN) != 0);
	inStrm = NULL;
	outStrm = NULL;
	dtmStrm = NULL;

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
		return (1);
	}

	magic = cs_GXDEF_MAGIC;
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
		tp = (struct cs_GxCmpT_ *)CS_tpars (&cp,(char *)cs_GxCmpT,sizeof (struct cs_GxCmpT_));
		if (tp == NULL)
		{
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Invalid line label (%s) at line %d.",err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			gxdef.xfrmName [0] = '\0';
			continue;
		}

		/* Here when we have a valid line. Cp points to the first
		   character after the colon. */
		(void)CS_trim (cp);
		switch (tp->type) {

		case GX_NAME:
			/* Here each time we encounter a new geodetic transformation name.  We see
			   if there is an existing geodetic transformation which must be written. */
			if (gxdef.xfrmName [0] != '\0' &&
				(test || CS_stricmp (gxdef.group,"TEST")))
			{
				/* Replace directory separators in grid file paths as appropriate. */
				CS_gxsep (&gxdef);
				
				/* Write it to the dictionary. */
				st = CSgxdefwr (outStrm,&gxdef,warn,dtmStrm,err_func);
				if (st >= 0) err_cnt += st;
				else
				{
					err_cnt += -st;
					cancel = TRUE;
				}
			}
		
			/* Prepare for the next geodetic transformation definition. */
			(void)memset ((char *)&gxdef,'\0',sizeof (gxdef));
			(void)CS_stncp (gxdef.xfrmName,cp,sizeof (gxdef.xfrmName));
			gxdef.inverseSupported = TRUE;
			gxdef.maxIterations = 8;
			gxdef.protect = 1;
			gxdef.cnvrgValue = 1.0E-09;
			gxdef.errorValue = 1.0E-06;
			gxdef.accuracy = 8.0;

			currentMethod = 0;
			geocentricCount = 0;
			mulregCount = 0;
			gridFileCount = 0;

			st = CS_nampp64 (gxdef.xfrmName);
			if (st != 0)
			{
				sprintf (err_msg,"%s is not a valid geodetic path key name; line %d.",cp,line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			break;

		case GROUP:
			CS_stncp (gxdef.group,cp,sizeof (gxdef.group));
			break;

		case DESC_NM:
			(void)CS_stncp (gxdef.description,cp,sizeof (gxdef.description));
			break;

		case SOURCE:
			CS_stncp (gxdef.source,cp,sizeof (gxdef.source));
			break;

		case SRC_DTM:
			CS_stncp (gxdef.srcDatum,cp,sizeof (gxdef.srcDatum));
			break;

		case TRG_DTM:
			CS_stncp (gxdef.trgDatum,cp,sizeof (gxdef.trgDatum));
			break;

		case METHOD:
			mp = (struct cs_GxCmpT_ *)CS_tpars (&cp,(char *)cs_GxMthdT,sizeof (struct cs_GxCmpT_));
			if (mp == NULL)
			{
				CS_stncp (err_seg,buff,sizeof (err_seg));
				sprintf (err_msg,"Invalid geodetic method (%s) at line %d.",err_seg,line_nbr);
				cancel = (*err_func)(err_msg);
				gxdef.xfrmName [0] = '\0';
				continue;
			}
			else
			{
				gxdef.methodCode = (short)mp->type;
				currentMethod = gxdef.methodCode;
			}
			break;

		case EPSG_NBR:
			gxdef.epsgCode = (short)atoi (cp);
			break;
		
		case EPSG_VAR:
			gxdef.epsgVariation = (short)atoi (cp);
			break;

		case INVERSE:
			gxdef.inverseSupported = ((*cp == 'Y') || (*cp == 'y'));
			break;

		case MAX_ITR:
			gxdef.maxIterations = (short)atoi (cp);
			break;

		case CNVRG_VAL:
			gxdef.cnvrgValue = strtod (cp,&dummy);
			break;

		case ERROR_VAL:
			gxdef.errorValue = strtod (cp,&dummy);
			break;

		case ACCURACY:
			gxdef.accuracy = strtod (cp,&dummy);
			break;

		case MIN_LNG:
			gxdef.rangeMinLng = strtod (cp,&dummy);
			break;

		case MAX_LNG:
			gxdef.rangeMaxLng = strtod (cp,&dummy);
			break;

		case MIN_LAT:
			gxdef.rangeMinLat = strtod (cp,&dummy);
			break;

		case MAX_LAT:
			gxdef.rangeMaxLat = strtod (cp,&dummy);
			break;

		case DELTA_X:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.deltaX = strtod (cp,&dummy);
			break;

		case DELTA_Y:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.deltaY = strtod (cp,&dummy);
			break;

		case DELTA_Z:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.deltaZ = strtod (cp,&dummy);
			break;

		case ROT_X:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.rotateX = strtod (cp,&dummy);
			break;

		case ROT_Y:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.rotateY = strtod (cp,&dummy);
			break;

		case ROT_Z:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.rotateZ = strtod (cp,&dummy);
			break;

		case BWSCALE:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.scale = strtod (cp,&dummy);
			break;

		case XLATE_X:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.translateX = strtod (cp,&dummy);
			break;

		case XLATE_Y:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.translateY = strtod (cp,&dummy);
			break;

		case XLATE_Z:
			geocentricCount += 1;
			gxdef.parameters.geocentricParameters.translateZ = strtod (cp,&dummy);
			break;

		case VALIDATION:
			mulregCount += 1;
			gxdef.parameters.dmaMulRegParameters.validation = strtod (cp,&dummy);
			break;

		case TEST_LNG:
			mulregCount += 1;
			CS_atof (&tmpDbl,cp);
			gxdef.parameters.dmaMulRegParameters.testLambda = tmpDbl;
			break;

		case TEST_LAT:
			mulregCount += 1;
			CS_atof (&tmpDbl,cp);
			gxdef.parameters.dmaMulRegParameters.testPhi = tmpDbl;
			break;

		case RSLT_DELTA_LNG:
			mulregCount += 1;
			gxdef.parameters.dmaMulRegParameters.deltaLambda = strtod (cp,&dummy);
			break;

		case RSLT_DELTA_LAT:
			mulregCount += 1;
			gxdef.parameters.dmaMulRegParameters.deltaPhi = strtod (cp,&dummy);
			break;

		case RSLT_DELTA_HGT:
			mulregCount += 1;
			gxdef.parameters.dmaMulRegParameters.deltaHeight = strtod (cp,&dummy);
			break;

		case SRC_LAT_OFF:
			mulregCount += 1;
			CS_atof (&tmpDbl,cp);
			gxdef.parameters.dmaMulRegParameters.phiOffset = tmpDbl;
			break;

		case SRC_LNG_OFF:
			mulregCount += 1;
			CS_atof (&tmpDbl,cp);
			gxdef.parameters.dmaMulRegParameters.lambdaOffset = tmpDbl;
			break;

		case NRML_KK:
			mulregCount += 1;
			gxdef.parameters.dmaMulRegParameters.normalizationScale = strtod (cp,&dummy);
			break;

		case LAT_COEF:
			mulregCount += 1;
			cnt = sscanf (cp,"U%d V%d: %lf",&ii,&jj,&coef);
			if (cnt != 3 || ii < 0 || ii >= 10 || jj < 0 || jj >= 10)
			{
				sprintf (err_msg,"At line %d, invalid coefficient specification.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				// This works for general polynomial, but not for mulreg:
				// idx = ((ii * (ii + 1)) / 2) + jj;	
				idx = ii * 10 + jj;				// This works for MULREG
				gxdef.parameters.dmaMulRegParameters.coeffPhi [idx] = coef;
			}
			break;

		case LNG_COEF:
			mulregCount += 1;
			cnt = sscanf (cp,"U%d V%d: %lf",&ii,&jj,&coef);
			if (cnt != 3 || ii < 0 || ii >= 10 || jj < 0 || jj >= 10)
			{
				sprintf (err_msg,"At line %d, invalid coefficient specification.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				// This works for general polynomial, but not for mulreg:
				// idx = ((ii * (ii + 1)) / 2) + jj;
				idx = ii * 10 + jj;				// This works for MULREG
				gxdef.parameters.dmaMulRegParameters.coeffLambda [idx] = coef;
			}
			break;

		case HGT_COEF:
			mulregCount += 1;
			cnt = sscanf (cp,"U%d V%d: %lf",&ii,&jj,&coef);
			if (cnt != 3 || ii < 0 || ii >= 10 || jj < 0 || jj >= 10)
			{
				sprintf (err_msg,"At line %d, invalid coefficient specification.",line_nbr);
				cancel = (*err_func)(err_msg);
				err_cnt += 1;
			}
			else
			{
				// This works for general polynomial, but not for mulreg:
				// idx = ((ii * (ii + 1)) / 2) + jj;
				idx = ii * 10 + jj;				// This works for MULREG
				gxdef.parameters.dmaMulRegParameters.coeffHeight [idx] = coef;
			}
			break;

		case GRID_FILE:
			/* GridFileCount variable is a count of the specifications
			   encountered which imply that the transformation is of the grid
			   file interpolation type.  This is used to verify that the
			   transformation type and the parameters supplied are consistent. */
			gridFileCount += 1;

			/* Isolate the format, the direction, and then the file specification. */
			cpDir = strchr (cp,',');
			if (cpDir == NULL)
			{
				CS_stncp (err_seg,cp,sizeof (err_seg));
				sprintf (err_msg,"Grid file direction not provided at line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				gxdef.xfrmName [0] = '\0';
				continue;
			}
			*cpDir++ = '\0';
			cpFile = strchr (cpDir,',');
			if (cpFile == NULL)
			{
				CS_stncp (err_seg,cp,sizeof (err_seg));
				sprintf (err_msg,"Grid file format not provided at line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				gxdef.xfrmName [0] = '\0';
				continue;
			}
			*cpFile++ = '\0';
			idx = gxdef.parameters.fileParameters.fileReferenceCount;
			
			fp = (struct cs_GxCmpT_ *)CS_tpars (&cp,(char *)cs_GxFrmtT,sizeof (struct cs_GxCmpT_));
			if (fp == NULL)
			{
				CS_stncp (err_seg,cp,sizeof (err_seg));
				sprintf (err_msg,"Invalid grid file format (%s) at line %d.",err_seg,line_nbr);
				cancel = (*err_func)(err_msg);
				gxdef.xfrmName [0] = '\0';
				continue;
			}
			else
			{
				gridFileFormat = fp->type;
			}

			if (*cpDir == 'f') *cpDir = 'F';
			if (*cpDir == 'i') *cpDir = 'I';
			if (*cpDir != 'F' && *cpDir != 'I')
			{
				CS_stncp (err_seg,cpDir,sizeof (err_seg));
				sprintf (err_msg,"Invalid grid file direction (%s) at line %d.",err_seg,line_nbr);
				cancel = (*err_func)(err_msg);
				gxdef.xfrmName [0] = '\0';
				continue;
			}

			gxdef.parameters.fileParameters.fileNames [idx].fileFormat = (unsigned char)gridFileFormat;
			gxdef.parameters.fileParameters.fileNames [idx].direction = *cpDir;			/*lint !e732  loss of sign */
			CS_stncp (gxdef.parameters.fileParameters.fileNames [idx].fileName,cpFile,sizeof (gxdef.parameters.fileParameters.fileNames[0].fileName));
			gxdef.parameters.fileParameters.fileReferenceCount += 1;
			break;

		case FALLBACK:
			/* GridFileCount variable is a count of the specifications
			   encountered which imply that the transformation is of the grid
			   file interpolation type.  This is used to verify that the
			   transformation type and the parameters supplied are consistent. */
			gridFileCount += 1;
			CS_stncp (gxdef.parameters.fileParameters.fallback,cp,sizeof (gxdef.parameters.fileParameters.fallback));
			break;

		default:
			sprintf (err_msg,"Software problem at line %d in module %s.",__LINE__,__FILE__);
			break;
		}
		/* On to the next input line. */
	}
	CS_fclose (inStrm);

	/* Write out the last coordinate system if not in error. */
	if (gxdef.xfrmName [0] != '\0')
	{
		st = CSgxdefwr (outStrm,&gxdef,warn,dtmStrm,err_func);
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
		return (err_cnt);
	}

	/* Sort the output file. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_ips (outStrm,sizeof (gxdef),0L,(CMPFUNC_CAST)CS_gxcmp);		/*lint !e534 */

	/* Check for duplicate names. */
	CS_fseek (outStrm,(long)sizeof (magic),SEEK_SET);
	CS_gxrd (outStrm,&gxdef);			/*lint !e534  ignoring return value */
	CS_stncp (last_name,gxdef.xfrmName,sizeof (last_name));
	while (!cancel && CS_gxrd (outStrm,&gxdef))
	{
		if (!CS_stricmp (last_name,gxdef.xfrmName))
		{
			sprintf (err_msg,"Geodetic transformation named %s appears more than once.",last_name);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		CS_stncp (last_name,gxdef.xfrmName,sizeof (last_name));
	}
	CS_fclose (outStrm);
	if (dtmStrm != NULL)
	{
		CS_fclose (dtmStrm);
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

int CSgxdefwr (	csFILE *outStrm,
				struct cs_GeodeticTransform_ *gxdef,
				int warn,
				csFILE* dtmStrm,
				int (*err_func)(char *mesg)
			  )
{
	extern char csErrnam [MAXPATH];

	int st;
	int ii;
	int flag;
	int crypt;
	int err_cnt;
	int chk_cnt;
	int cancel;
	int list_sz;

	char err_msg [256];
	
	int err_list [12];

 	__ALIGNMENT__1		/* For some versions of Sun compiler. */
	struct cs_Dtdef_ dt_def;

	err_cnt = 0;
	cancel = FALSE;

	/* Check all the other stuff other than the existence of the datums.  We'll
	   do that below.  This is done as we want to check the datum dictionary
	   which we just compiled for the names, not the currently active
	   dictionary.
	   
	   We skip the CS_gxchk function if the method code is Grid Interpolation.
	   We do this as there is no requirement that the compiler operate in the
	   operational environment which includes all the grid interpolation data
	   files. */
	list_sz = sizeof (err_list) / sizeof (int);
	if (gxdef->methodCode != cs_DTCMTH_GFILE)
	{
		chk_cnt = CS_gxchk (gxdef,0,err_list,list_sz);
		if (chk_cnt != 0)
		{
			CS_stncp (csErrnam,gxdef->xfrmName,MAXPATH);
			for (ii = 0;ii < chk_cnt && ii < list_sz;ii++)
			{
				CSerpt (err_msg,sizeof (err_msg),err_list [ii]);
				cancel = (*err_func)(err_msg);
			}
			err_cnt += chk_cnt;
		}
	}

	/* Use the dtmStrm variable provided to verify that the source datum
	   referenced is a valid datum name. */
	if (dtmStrm != NULL)
	{
		CS_stncp (dt_def.key_nm,gxdef->srcDatum,sizeof (dt_def.key_nm));
		CS_nampp (dt_def.key_nm);				/*lint !e534   ignoring return value */
		dt_def.fill [0] = '\0';
		dt_def.fill [1] = '\0';
		flag = CS_bins (dtmStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (dt_def),&dt_def,(CMPFUNC_CAST)CS_dtcmp);
		if (flag == 1)
		{
			st = CS_dtrd (dtmStrm,&dt_def,&crypt);
			if (st == 1)
			{
				if (!CS_stricmp (dt_def.group,"LeGACY") && CS_stricmp (gxdef->group,"LEGACY"))
				{
					if (warn)
					{
						sprintf (err_msg,"Non-legacy transformation named %s references legacy datum named %s.",gxdef->xfrmName,dt_def.key_nm);
						cancel = (*err_func)(err_msg);
					}
				}
			}
		}
		else if (flag == 0)
		{
			sprintf (err_msg,"Invalid source datum, %s, on transformation named %s.",gxdef->srcDatum,gxdef->xfrmName);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		else
		{
			sprintf (err_msg,"Datum dictionary access failure detected when checking transformation named %s.",gxdef->xfrmName);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}

	/* Use the dtmStrm variable provided to verify that the target datum
	   referenced is a valid datum name. */
	if (dtmStrm != NULL)
	{
		CS_stncp (dt_def.key_nm,gxdef->trgDatum,sizeof (dt_def.key_nm));
		CS_nampp (dt_def.key_nm);				/*lint !e534   ignoring return value */
		dt_def.fill [0] = '\0';
		dt_def.fill [1] = '\0';
		flag = CS_bins (dtmStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (dt_def),&dt_def,(CMPFUNC_CAST)CS_dtcmp);
		if (flag == 1)
		{
			st = CS_dtrd (dtmStrm,&dt_def,&crypt);
			if (st == 1)
			{
				if (!CS_stricmp (dt_def.group,"LeGACY") && CS_stricmp (gxdef->group,"LEGACY"))
				{
					if (warn)
					{
						sprintf (err_msg,"Non-legacy transformation named %s references legacy datum named %s.",gxdef->xfrmName,dt_def.key_nm);
						cancel = (*err_func)(err_msg);
					}
				}
			}
		}
		else if (flag == 0)
		{
			sprintf (err_msg,"Invalid target datum, %s, on transformation named %s.",gxdef->trgDatum,gxdef->xfrmName);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
		else
		{
			sprintf (err_msg,"Datum dictionary access failure detected when checking transformation named %s.",gxdef->xfrmName);
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}

	if (err_cnt == 0)
	{
		gxdef->protect = TRUE;
		/* The following sets the "creation date" to Jan 1, 2002 (approx) for
		   all test coordinate systems.  This enables the protection system to
		   be tested.  Normally, distribution definitions are protected from
		   any kind of change. */
		if (!CS_stricmp (gxdef->group,"TEST"))
		{
			gxdef->protect = 4383;
		}

		/* Write this definition to the distionary file. */
		st = CS_gxwr (outStrm,gxdef);
		if (st != 0)
		{
			CS_errmsg (err_msg,sizeof (err_msg));
			cancel = (*err_func)(err_msg);
			err_cnt += 1;
		}
	}
	if (warn && gxdef->description [0] == '\0')
	{
		sprintf (err_msg,"No description for geodetic transformation named %s.",gxdef->xfrmName);
		cancel = (*err_func)(err_msg);
	} 
	if (warn && gxdef->source [0] == '\0')
	{
		sprintf (err_msg,"No source for geodetic transformation named %s.",gxdef->xfrmName);
		cancel = (*err_func)(err_msg);
	}
	if (cancel && err_cnt == 0) err_cnt = 1;
	return (cancel ? -err_cnt : err_cnt);
}
