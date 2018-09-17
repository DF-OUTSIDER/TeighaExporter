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

struct cs_MIunit_
{
	char csMap [16];
	char mapinfo [16];
	short mi_nbr;
};
struct cs_MIunit_ cs_MIunit [] =
{
	{      "METER", "m",          7},
	{       "FOOT", "survey ft",  8},
	{       "INCH", "in",        -1},
	{      "IFOOT", "ft",         3},
	{      "IINCH", "in",         2},
	{ "CENTIMETER", "cm",         6},
	{  "KILOMETER", "km",         1},
	{       "YARD", "yd",        -1},
	{       "MILE", "mi",        -1},
	{      "IYARD", "yd",         4},
	{      "IMILE", "mi",         0},
	{       "KNOT", "nmi",        9},
	{      "NAUTM", "nmi",        9},
	{  "DECIMETER", "dm",        -2},
	{ "MILLIMETER", "mm",         5},
	{           "", "",          -3}
};

struct cs_MIdatum_
{
   	char csMap [10];
	short mapinfo;
};

struct cs_MIdatum_ cs_MIdatum [] =
{
	{  "ADINDAN",  1 },
	{  "AFGOOYE",  2 },
	{ "AINELABD",  3 },
	{   "ANNA65",  4 },
	{  "ARC1960",  6 },
	{  "ASCENSN",  7 },
	{ "ASTATN52", 11 },
	{ "ASTRLA66", 12 },
	{ "ASTRLA84", 13 },
	{ "BELLEVUE", 14 },
	{  "BERMUDA", 15 },
	{   "BOGOTA", 16 },
	{    "CAMPO", 17 },
	{   "CANTON", 18 },
	{     "CAPE", 19 },
	{  "CANAVRL", 20 },
	{ "CARTHAGE", 21 },
	{  "CHATHAM", 22 },
	{     "CHAU", 23 },
	{  "CORREGO", 24 },
	{  "DJAKRTA", 25 },
	{  "DOS1968", 26 },
	{   "EASTER", 27 },
	{  "EUROP50", 28 },
	{  "EUROP79", 29 },
	{ "GNDAJIKA", 30 },
	{   "GD1949", 31 },
	{   "GUAM63", 34 },
	{     "GUX1", 35 },
	{  "HJORSEY", 37 },
	{ "HONGKONG", 38 },
	{   "INDIAN", 41 },
	{ "INDIANTV", 40 },
	{ "IRELND65", 42 },
	{   "ISTS69", 43 },
	{   "JHNSTN", 44 },
	{ "KANDWALA", 45 },
	{ "KERGUELN", 46 },
	{ "KERTAU48", 47 },
	{     "L-C5", 48 },
	{  "LIBERIA", 49 },
	{    "LUZON", 50 },
	{ "LUZON-MI", 51 },
	{  "MADEIRA", 94 },
	{ "MAHE1971", 52 },
	{    "MARCO", 53 },
	{  "MASSAWA", 54 },
	{ "MERCHICH", 55 },
	{ "MICHIGAN", 73 },
	{   "MIDWAY", 56 },
	{    "MINNA", 57 },
	{    "NAD27", 62 },
	{ "NAD27-AK", 63 },
	{ "NAD27-BH", 64 },
	{ "NAD27-SS", 64 },
	{ "NAD27-CA", 66 },
	{ "NAD27-CZ", 67 },
	{ "NAD27-CB", 68 },
	{ "NAD27-CX", 69 },
	{ "NAD27-CU", 70 },
	{ "NAD27-GR", 71 },
	{ "NAD27-MX", 72 },
	{    "NAD83", 74 },
	{  "NHRWN-O", 58 },
	{  "NHRWN-S", 60 },
	{  "NHRWN-U", 59 },
	{ "NAPARIMA", 61 },
	{  "OBSRV66", 75 },
	{ "OLD_EGYP", 76 },
	{    "OLDHI", 77 },
	{     "OMAN", 78 },
	{     "OSGB", 79 },
	{ "PITCAIRN", 81 },
	{     "PRVI", 83 },
	{   "PSAD56", 82 },
	{    "PSC63", 36 },
	{    "QATAR", 84 },
	{   "QORNOQ", 85 },
	{  "REUNION", 86 },
	{ "ROME1940", 87 },
	{   "SA1969", 92 },
	{    "SANTO", 88 },
	{  "SAOBRAZ", 89 },
	{   "SAPPER", 90 },
	{ "SCHWARZK", 91 },
	{  "SINGAPR", 93 },
	{ "TIMBALAI", 96 },
	{    "TOKYO", 97 },
	{  "TRISTAN", 98 },
	{     "VITI", 99 },
	{     "WAKE",100 },
	{    "WGS72",103 },
	{    "WGS84",104 },
	{ "ZANDERIJ",106 },
	{         "", -1 }
};

/**********************************************************************
**	coordsys = CS_mifcs (cs_def);
**
**	struct cs_Csdef_ *cs_def;	pointer to the definition of the coordinate
**								system for which a COORDSYS line is required.
**	char *coordsys;				returns a pointer to a static character array
**								which contains the formatted string.
**
**	Since the return pointer is to a static character array,
**	the results will only remain valid until the next call to
**	this function.
**
**********************************************************************/

Const char* EXP_LVL3 CS_mifcs (Const struct cs_Csdef_ *cs_def)
{
	extern struct cs_Prjtab_ cs_Prjtab [];
	extern char csErrnam [];

   	static char cs_claus [256];

	int datum;
	int zoneNbr;

	char *unit;

	char *cp;
	struct cs_Prjtab_ *pp;
	struct cs_MIdatum_ *dt_ptr;
	struct cs_MIunit_  *un_ptr;

	double cntMer;
	double falseEasting;
	double falseNorthing;
	double unitFactor;

	/* Locate the proper units string. If the coordinate system is
	   geographic in nature, the unit string is blank. */

	for (pp = cs_Prjtab;*pp->key_nm != '\0';pp++)
	{
		if (!CS_stricmp (pp->key_nm,cs_def->prj_knm)) break;
	}
	if (*pp->key_nm == '\0' || pp->setup == NULL)
	{
		CS_stncp (csErrnam,cs_def->prj_knm,MAXPATH);
		CS_erpt (cs_UNKWN_PROJ);
		goto error;
	}
	if (pp->code != cs_PRJCOD_UNITY)
	{
		for (un_ptr = cs_MIunit;un_ptr->csMap [0] != '\0';un_ptr += 1)
		{
			if (!CS_stricmp (cs_def->unit,un_ptr->csMap)) break;
		}
		if (un_ptr->mi_nbr <= 0)
		{
			CS_stncp (csErrnam,cs_def->prj_knm,MAXPATH);
			CS_erpt (cs_MIF_UNIT);
			goto error;
		}
		unit = un_ptr->mapinfo;
	}
	else
	{
		unit = " ";
	}

	/* Locate the datum number. */

	if (cs_def->dat_knm [0] == '\0')
	{
		CS_erpt (cs_MIF_ELREF);
		goto error;
	}
	else
	{
		datum = -1;
		for (dt_ptr = cs_MIdatum;dt_ptr->mapinfo >= 0;dt_ptr += 1)
		{
			if (!CS_stricmp (cs_def->dat_knm,dt_ptr->csMap))
			{
				datum = dt_ptr->mapinfo;
				break;
			}
		}
		if (dt_ptr->mapinfo < 0)
		{
			CS_stncp (csErrnam,cs_def->dat_knm,MAXPATH);
			CS_erpt (cs_MIF_DATUM);
			goto error;
		}
	}

	cp = cs_claus;
	cp += sprintf (cp,"Coordsys Earth Projection ");

	/* Now we ve special code for each projection for which there is
	  known MapInfo support. */

	CS_stncp (csErrnam,cs_def->key_nm,MAXPATH);
	switch (pp->code) {

	case cs_PRJCOD_TRMER:

		sprintf (cp,"8, %d, \"%s\", %.8f, %.8f, %.9f,%.2f,%.2f",
							datum,
							unit,
							cs_def->prj_prm1,
							cs_def->org_lat,
							cs_def->scl_red,
							cs_def->x_off,
							cs_def->y_off);
		break;

	case cs_PRJCOD_UTM:

		unitFactor = CS_unitlu (cs_UTYP_LEN,cs_def->unit);
		if (unitFactor == 0.0) unitFactor = 1.0;
		zoneNbr = (int)cs_def->prj_prm1;
		cntMer = (double)(-183 + (6 * zoneNbr));		/*lint !e790 */
		falseEasting = 500000.0 / unitFactor;
		if (cs_def->prj_prm2 >= 0) falseNorthing = 0.0;
		else falseNorthing = 10000000.0;
		falseNorthing /= unitFactor;
		sprintf (cp,"8, %d, \"%s\", %.8f, %.8f, %.9f,%.2f,%.2f",
							datum,
							unit,
							cntMer,
							0.0,
							0.9996,
							falseEasting,
							falseNorthing);
		break;

	case cs_PRJCOD_LM2SP:

		sprintf (cp,"3, %d, \"%s\", %.8f, %.8f, %.8f, %.8f, %.2f, %.2f",
							datum,
							unit,
							cs_def->org_lng,
							cs_def->org_lat,
							cs_def->prj_prm1,
							cs_def->prj_prm2,
							cs_def->x_off,
							cs_def->y_off);
		break;

	case cs_PRJCOD_ALBER:

		sprintf (cp,"9, %d, \"%s\", %.8f, %.8f, %.8f, %.8f, %.2f, %.2f",
								datum,
								unit,
								cs_def->org_lng,
								cs_def->org_lat,
								cs_def->prj_prm1,
								cs_def->prj_prm2,
								cs_def->x_off,
								cs_def->y_off);
		break;

	case cs_PRJCOD_MRCAT:

		if (cs_def->prj_prm2 != 0.0 ||
			cs_def->x_off != 0.0 ||
			cs_def->y_off != 0.0)
		{
			CS_erpt (cs_MIF_FEATR);
			goto error;
		}
		sprintf (cp,"10, %d, \"%s\", %.8f",
						datum,
						unit,
						cs_def->prj_prm1);
		break;

	case cs_PRJCOD_AZMED:
		if (cs_def->prj_prm1 != 0.0  ||
			cs_def->x_off != 0.0 ||
			cs_def->y_off != 0.0)
		{
			CS_erpt (cs_MIF_FEATR);
			goto error;
		}
		sprintf (cp,"5, %d, \"%s\", %.8f, %.8f, 90.0",
							datum,
							unit,
							cs_def->org_lng,
							cs_def->org_lat);
		break;

	case cs_PRJCOD_AZMEA:
		if (cs_def->prj_prm1 != 0.0 ||
			cs_def->x_off != 0.0 ||
			cs_def->y_off != 0.0)
		{
			CS_erpt (cs_MIF_FEATR);
			goto error;
		}
		sprintf (cp,"5, %d, \"%s\", %.8f, %.8f, 90.0",
							datum,
							unit,
							cs_def->org_lng,
							cs_def->org_lat);
		break;

	case cs_PRJCOD_HOM1XY:
		sprintf (cp,"7, %d, \"%s\", %.8f, %.8f, %.8f, %.9f, %.3f, %.3f",
							datum,
							unit,
							cs_def->prj_prm1,
							cs_def->prj_prm2,
							cs_def->prj_prm3,
							cs_def->scl_red,
							cs_def->x_off,
							cs_def->y_off);
		break;

	case cs_PRJCOD_EDCNC:

		sprintf (cp,"6, %d, \"%s\", %.8f, %.8f, %.8f, %.8f, %.2f, %.2f",
							datum,
							unit,
							cs_def->org_lng,
							cs_def->org_lat,
							cs_def->prj_prm1,
							cs_def->prj_prm2,
							cs_def->x_off,
							cs_def->y_off);
		break;

	case cs_PRJCOD_MILLR:

		if (cs_def->x_off != 0.0 ||
			cs_def->y_off != 0.0)
		{
			CS_erpt (cs_MIF_FEATR);
			goto error;
		}
		sprintf (cp,"11, %d, \"%s\", %.8f",
							datum,
							unit,
							cs_def->prj_prm1);
		break;

	case cs_PRJCOD_UNITY:

		if (cs_def->org_lng != 0.0 ||
			CS_stricmp (cs_def->unit,"DEGREE"))
		{
			CS_erpt (cs_MIF_FEATR);
			goto error;
		}
		sprintf (cp,"1, %d",datum);
		break;

	default:
		CS_erpt (cs_MIF_PROJ);
		goto error;
	}
	return (cs_claus);
error:
	return (NULL);
}

