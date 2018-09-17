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

static int CScmpPrjParm (FILE* rptStrm,char* key_nm,struct cs_Prjtab_* pp,
									   int prmNbr,
									   double wasValue,
									   double isValue);

int CS_csDiff (FILE *rptStrm,struct cs_Csdef_ *was,struct cs_Csdef_ *is)
{
	extern struct cs_Prjtab_ cs_Prjtab [];		/* Projection Table */

	int flag = TRUE;
	int errCnt = 0;
	double okValue;

	struct cs_Prjtab_ *pp;

	char key_name [64];

	if (strcmp (was->dat_knm,is->dat_knm))
	{
		fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Datum Key Name (dat_knm):\n       was: %s\n    is now: %s\n",was->dat_knm,is->dat_knm);
	}
	if (strcmp (was->elp_knm,is->elp_knm))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Ellipsoid Key Name (elp_knm):\n       was: %s\n    is now: %s\n",was->elp_knm,is->elp_knm);
	}
	if (strcmp (was->prj_knm,is->prj_knm))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Projection Key Name (prj_knm):\n       was: %s\n    is now: %s\n",was->prj_knm,is->prj_knm);
	}
	if (strcmp (was->group,is->group))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Group (group):\n       was: %s\n    is now: %s\n",was->group,is->group);
	}
	if (strcmp (was->locatn,is->locatn))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Group (group):\n       was: %s\n    is now: %s\n",was->locatn,is->locatn);
	}
	if (strcmp (was->cntry_st,is->cntry_st))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Political Location (cntry_st):\n       was: %s\n    is now: %s\n",was->cntry_st,is->cntry_st);
	}
	if (strcmp (was->unit,is->unit))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Unit (unit):\n       was: %s\n    is now: %s\n",was->unit,is->unit);
	}

	/* Look this projection up in the projection table, we need to pass the
	   projection code to the parameter check function. */

	for (pp = cs_Prjtab;pp->key_nm [0] != '\0';pp += 1)
	{
		if (!CS_stricmp (was->prj_knm,pp->key_nm))
		{
			break;
		}
	}

	if (pp->check == NULL)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;

		fprintf (rptStrm,"  %s has an invalid projection code name.",was->prj_knm);
	}
	else
	{
		/* Check all of the parameters. */

		if (flag) CS_stcpy (key_name,was->key_nm);
		else      key_name [0] = '\0';

		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 1,was->prj_prm1 ,is->prj_prm1);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 2,was->prj_prm2 ,is->prj_prm2);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 3,was->prj_prm3 ,is->prj_prm3);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 4,was->prj_prm4 ,is->prj_prm4);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 5,was->prj_prm5 ,is->prj_prm5);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 6,was->prj_prm6 ,is->prj_prm6);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 7,was->prj_prm7 ,is->prj_prm7);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 8,was->prj_prm8 ,is->prj_prm8);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp, 9,was->prj_prm9 ,is->prj_prm9);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,10,was->prj_prm10,is->prj_prm10);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,11,was->prj_prm11,is->prj_prm11);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,12,was->prj_prm12,is->prj_prm12);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,13,was->prj_prm13,is->prj_prm13);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,14,was->prj_prm14,is->prj_prm14);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,15,was->prj_prm15,is->prj_prm15);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,16,was->prj_prm16,is->prj_prm16);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,17,was->prj_prm17,is->prj_prm17);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,18,was->prj_prm18,is->prj_prm18);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,19,was->prj_prm19,is->prj_prm19);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,20,was->prj_prm20,is->prj_prm20);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,21,was->prj_prm21,is->prj_prm21);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,22,was->prj_prm22,is->prj_prm22);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,23,was->prj_prm23,is->prj_prm23);
		errCnt += CScmpPrjParm (rptStrm,key_name,pp,24,was->prj_prm24,is->prj_prm24);
	}

	okValue = 1.0E-09;
	if (fabs (was->org_lng - is->org_lng) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Origin Longitude (org_lng):\n       was: %18.12f\n    is now: %18.12f\n",was->org_lng,is->org_lng);
	}
	if (fabs (was->org_lat - is->org_lat) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Origin Latitude (org_lat):\n       was: %18.12f\n    is now: %18.12f\n",was->org_lat,is->org_lat);
	}

	okValue = 1.0E-03;
	if (fabs (was->x_off - is->x_off) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  FALSE Easting (x_off):\n       was: %16.6f\n    is now: %16.6f\n",was->x_off,is->x_off);
	}
	if (fabs (was->y_off - is->y_off) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  FALSE Northing (y_off):\n       was: %16.6f\n    is now: %16.6f\n",was->y_off,is->y_off);
	}

	okValue = 1.0E-09;
	if (fabs (was->scl_red - is->scl_red) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Scale Reduction (scl_red):\n       was: %18.12f\n    is now: %18.12f\n",was->scl_red,is->scl_red);
	}

	if (!CS_stricmp (was->prj_knm,"LL"))
	{
		okValue = 1.0E-09;
		if (fabs (was->zero [0] - is->zero [0]) > okValue || fabs (was->zero [1] - is->zero [1]) > okValue)
		{
			if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
			flag = FALSE;
			errCnt += 1;
			fprintf (rptStrm,"  Miinimum Non Zero (zero):\n       was: %14.12f:%14.12f\n    is now: %14.12f:%14.12f\n",was->zero [0],was->zero[1],is->zero [0],is->zero [1]);
		}
	}
	else
	{
		okValue = 1.0E-03;
		if (fabs (was->zero [0] - is->zero [0]) > okValue || fabs (was->zero [1] - is->zero [1]) > okValue)
		{
			if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
			flag = FALSE;
			errCnt += 1;
			fprintf (rptStrm,"  Miinimum Non Zero (zero):\n       was: %8.6f:%8.6f\n    is now: %8.6f:%8.6f\n",was->zero [0],was->zero[1],is->zero [0],is->zero [1]);
		}
	}

	okValue = 1.0E-09;
	if (fabs (was->hgt_lng - is->hgt_lng) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Height Longitude (hgt_lng):\n       was: %18.12f\n    is now: %18.12f\n",was->hgt_lng,is->hgt_lng);
	}
	if (fabs (was->hgt_lat - is->hgt_lat) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Height Latitude (hgt_lat):\n       was: %18.12f\n    is now: %18.12f\n",was->hgt_lat,is->hgt_lat);
	}
	if (fabs (was->hgt_zz - is->hgt_zz) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Height Elevation (hgt_zz):\n       was: %12.6f\n    is now: %12.6f\n",was->hgt_zz,is->hgt_zz);
	}
	if (fabs (was->geoid_sep - is->geoid_sep) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Geoid Separation (geoid_sep):\n       was: %12.6f\n    is now: %12.6f\n",was->geoid_sep,is->geoid_sep);
	}

	okValue = 1.0E-08;
	if (fabs (was->ll_min [0] - is->ll_min [0]) > okValue || fabs (was->ll_min [1] - is->ll_min [1]) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Min Lat/Lng (ll_min):\n       was: %14.8f:%14.8f\n    is now: %14.8f:%14.8f\n",was->ll_min [0],was->ll_min[1],is->ll_min [0],is->ll_min [1]);
	}
	if (fabs (was->ll_max [0] - is->ll_max [0]) > okValue || fabs (was->ll_max [1] - is->ll_max [1]) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Max Lat/Lng (ll_max):\n       was: %14.8f:%14.8f\n    is now: %14.8f:%14.8f\n",was->ll_max [0],was->ll_max[1],is->ll_max [0],is->ll_max [1]);
	}

	okValue = 1.0E-03;
	if (fabs (was->xy_min [0] - is->xy_min [0]) > okValue || fabs (was->xy_min [1] - is->xy_min [1]) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Min X/Y (xy_min):\n       was: %14.4f:%14.4f\n    is now: %14.4f:%14.4f\n",was->xy_min [0],was->xy_min[1],is->xy_min [0],is->xy_min [1]);
	}
	if (fabs (was->xy_max [0] - is->xy_max [0]) > okValue || fabs (was->xy_max [1] - is->xy_max [1]) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Max X/Y (xy_max):\n       was: %14.4f:%14.4f\n    is now: %14.4f:%14.4f\n",was->xy_max [0],was->xy_max[1],is->xy_max [0],is->xy_max [1]);
	}

	if (strcmp (was->desc_nm,is->desc_nm))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Descriptive Name (desc_nm):\n       was: %s\n    is now: %s\n",was->desc_nm,is->desc_nm);
	}
	if (strcmp (was->source,is->source))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Source description (source):\n       was: %s\n    is now: %s\n",was->source,is->source);
	}

	if (was->quad != is->quad)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Quadrant (quad):\n       was: %d\n    is now: %d\n",was->quad,is->quad);
	}
	if (was->order != is->order)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Polynomial Order (order):\n       was: %d\n    is now: %d\n",was->order,is->order);
	}
	if (was->zones != is->zones)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Zone Count (zones):\n       was: %d\n    is now: %d\n",was->zones,is->zones);
	}
	if (was->protect != is->protect)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Protection Flag (protect):\n       was: %d\n    is now: %d\n",was->protect,is->protect);
	}
	if (was->epsg_qd != is->epsg_qd)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Automatic Geoid Flag (epsg_qd):\n       was: %d\n    is now: %d\n",was->epsg_qd,is->epsg_qd);
	}
	if (was->srid != is->srid)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Elevation Technique (srid):\n       was: %d\n    is now: %d\n",was->srid,is->srid);
	}
	if (was->epsgNbr != is->epsgNbr)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  EPSG (epsgNbr):\n       was: %d\n    is now: %d\n",was->epsgNbr,is->epsgNbr);
	}
	if (was->wktFlvr != is->wktFlvr)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  WKT Flavor (wktFlvr):\n       was: %d\n    is now: %d\n",was->wktFlvr,is->wktFlvr);
	}
	return errCnt;
}

int CScmpPrjParm (FILE* rptStrm,char* key_nm,struct cs_Prjtab_* pp,int prmNbr,double wasValue,double isValue)
{
	extern struct cs_PrjprmMap_ cs_PrjprmMap [];
	extern struct cs_Prjprm_ csPrjprm [];

	unsigned char parmType;
	int errCnt;
	struct cs_PrjprmMap_ *mapPtr;
	struct cs_Prjprm_ *prmPtr;

	double okValue;

	errCnt = 0;

	/* Get the type of parameter. */

	for (mapPtr = cs_PrjprmMap;mapPtr->prj_code != cs_PRJCOD_END;mapPtr += 1)
	{
		if (mapPtr->prj_code == pp->code) break;
	}
	if (mapPtr->prj_code == cs_PRJCOD_END)
	{
		if (*key_nm != '\0')
		{
			fprintf (rptStrm,"\n%s has changed:\n",key_nm);
			*key_nm = '\0';
		}
		fprintf (rptStrm,"  Projection code did not map.\n");
		errCnt += 1;
		return errCnt;
	}

	/* Locate the parameter type. */

	parmType = mapPtr->prm_types [prmNbr - 1];

	/* See if this parameter is used. */

	if (parmType == 0)
	{
		/* May want to put a test for zero in here. */
		return 0;
	}
	prmPtr = &csPrjprm [parmType];
	
	/* Test the value. */

	switch (prmPtr->log_type) {
	case cs_PRMLTYP_LNG:
	case cs_PRMLTYP_LAT:
		okValue = 1.0E-09;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %18.12f\n    is now: %18.12f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_AZM:
	case cs_PRMLTYP_ANGD:
		okValue = 1.0E-07;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %18.12f\n    is now: %18.12f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_CMPLXC:
		okValue = 1.0E-07;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %18.12f\n    is now: %18.12f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_ZNBR:
		okValue = 0.1;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %4.1f\n    is now: %4.1f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_HSNS:
		okValue = 0.1;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %4.1f\n    is now: %4.1f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_GHGT:
	case cs_PRMLTYP_ELEV:
		okValue = 1.0E-03;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %10.4f\n    is now: %10.4f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_AFCOEF:
		okValue = 1.0E-10;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %.8f\n    is now: %.8f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_XYCRD:
		okValue = 0.001;
		if (fabs (wasValue - isValue) >= okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %.3f\n    is now: %.3f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;

	case cs_PRMLTYP_SCALE:
		okValue = 1.0E-10;
		if (fabs (wasValue - isValue) > okValue)
		{
			if (*key_nm != '\0')
			{
				fprintf (rptStrm,"\n%s has changed:\n",key_nm);
				*key_nm = '\0';
			}
			fprintf (rptStrm,"  %s (prj_prm%d):\n       was: %.8f\n    is now: %.8f\n",
										prmPtr->label,prmNbr,wasValue,isValue);
			errCnt += 1;
		}
		break;
		
	default:
		if (*key_nm != '\0')
		{
			fprintf (rptStrm,"\n%s has a problem:\n",key_nm);
			*key_nm = '\0';
		}
		fprintf (rptStrm,"  Parameter number %d did not map.\n",prmNbr);
		errCnt += 1;
		break;
	}
	return errCnt;
}
int CS_dtDiff (FILE *rptStrm,struct cs_Dtdef_ *was,struct cs_Dtdef_ *is)
{
	int flag = TRUE;
	int errCnt = 0;
	double okValue;

	if (CS_stricmp (was->ell_knm,is->ell_knm))
	{
		fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Ellipsoid Key Name (ell_knm):\n       was: %s\n    is now: %s\n",was->ell_knm,is->ell_knm);
	}
	if (CS_stricmp (was->group,is->group))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Group (group):\n       was: %s\n    is now: %s\n",was->group,is->group);
	}
	if (CS_stricmp (was->locatn,is->locatn))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Location (locatn):\n       was: %s\n    is now: %s\n",was->locatn,is->locatn);
	}
	if (CS_stricmp (was->cntry_st,is->cntry_st))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Political Location (cntry_st):\n       was: %s\n    is now: %s\n",was->cntry_st,is->cntry_st);
	}

	okValue = 1.0E-03;
	if (fabs (was->delta_X - is->delta_X) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Delta X (delta_X):\n       was: %9.3f\n    is now: %9.3f\n",was->delta_X,is->delta_X);
	}
	if (fabs (was->delta_Y - is->delta_Y) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Delta Y (delta_Y):\n       was: %9.3f\n    is now: %9.3f\n",was->delta_Y,is->delta_Y);
	}
	if (fabs (was->delta_Z - is->delta_Z) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Delta Z (delta_Z):\n       was: %9.3f\n    is now: %9.3f\n",was->delta_Z,is->delta_Z);
	}

	okValue = 1.0E-03;
	if (fabs (was->rot_X - is->rot_X) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  X Rotation (rot_X):\n       was: %9.3f\n    is now: %9.3f\n",was->rot_X,is->rot_X);
	}
	if (fabs (was->rot_Y - is->rot_Y) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Y Rotation (rot_Y):\n       was: %9.3f\n    is now: %9.3f\n",was->rot_Y,is->rot_Y);
	}
	if (fabs (was->rot_Z - is->rot_Z) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Z Rotation (rot_Z):\n       was: %9.3f\n    is now: %9.3f\n",was->rot_Z,is->rot_Z);
	}

	okValue = 1.0E-06;
	if (fabs (was->bwscale - is->bwscale) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Scale (bwscale):\n       was: %10.6f\n    is now: %10.6f\n",was->bwscale,is->bwscale);
	}

	if (CS_stricmp (was->name,is->name))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Descriptive Name (name):\n       was: %s\n    is now: %s\n",was->name,is->name);
	}
	if (CS_stricmp (was->source,is->source))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Information Source (source):\n       was: %s\n    is now: %s\n",was->source,is->source);
	}

	if (was->protect != is->protect)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Protection (protect):\n       was: %d\n    is now: %d\n",was->protect,is->protect);
	}
	if (was->to84_via != is->to84_via)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Calculation Technique (to84_via):\n       was: %d\n    is now: %d\n",was->to84_via,is->to84_via);
	}
	if (was->epsgNbr != is->epsgNbr)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Epsg Number (epsgNbr):\n       was: %d\n    is now: %d\n",was->epsgNbr,is->epsgNbr);
	}
	return errCnt;
}

int CS_elDiff (FILE *rptStrm,struct cs_Eldef_ *was,struct cs_Eldef_ *is)
{
	int flag = TRUE;
	int errCnt = 0;
	double okValue;

	if (CS_stricmp (was->group,is->group))
	{
		fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Group (group):\n       was: %s\n    is now: %s\n",was->group,is->group);
	}

	okValue = 1.0E-04;
	if (fabs (was->e_rad - is->e_rad) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Equatorial Radius (e_rad):\n       was: %12.3f\n    is now: %12.3f\n",was->e_rad,is->e_rad);
	}
	if (fabs (was->p_rad - is->p_rad) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Polar radius (p_rad):\n       was: %12.3f\n    is now: %12.3f\n",was->p_rad,is->p_rad);
	}

	okValue = 1.0E-12;
	if (fabs (was->flat - is->flat) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Flattening (flat):\n       was: %14.11f\n    is now: %14.11f\n",was->flat,is->flat);
	}
	if (fabs (was->ecent - is->ecent) > okValue)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Eccentricity (ecent):\n       was: %14.11f\n    is now: %14.11f\n",was->ecent,is->ecent);
	}

	if (CS_stricmp (was->name,is->name))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Descriptive Name (name):\n       was: %s\n    is now: %s\n",was->name,is->name);
	}

	if (CS_stricmp (was->source,is->source))
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Information Source (source):\n       was: %s\n    is now: %s\n",was->source,is->source);
	}

	if (was->protect != is->protect)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Protection (protect):\n       was: %d\n    is now: %d\n",was->protect,is->protect);
	}

	if (was->epsgNbr != is->epsgNbr)
	{
		if (flag) fprintf (rptStrm,"\n%s has changed:\n",is->key_nm);
		flag = FALSE;
		errCnt += 1;
		fprintf (rptStrm,"  Epsg Number (epsgNbr):\n       was: %d\n    is now: %d\n",was->epsgNbr,is->epsgNbr);
	}

	return errCnt;
}
