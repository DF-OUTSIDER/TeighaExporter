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

/*lint -esym(613,err_list) */
/*lint -esym(715,prj_code) */

/**********************************************************************
**	err_cnt = CStrmerQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	specifies the specific variation of the projection.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;				the size of the array pointed to be err_list,
**								may be zero.
**	int err_cnt;				returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greennwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CStrmerQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern char cs_Dir [];
	extern char *cs_DirP;

	extern double cs_One;			/*    1.0 */
	extern double cs_Mone;			/*    1.0 */
	extern double cs_K60;			/*   60.0 */
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_SclRedMin;		/*    0.75 */
	extern double cs_SclRedMax;		/*    1.1 */
	extern double cs_ParmTest;		/*    2.78E-05 */

	int err_cnt;
	double tmp1;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	if (prj_code == cs_PRJCOD_OSTN97)
	{
		/* First check, whether the txt representation of the OST97 file exists. If it doesn't, look for
		the binary file (which might have been compiled before).
		All the other parameters are hard coded into the projection. */
		CS_stcpy (cs_DirP,cs_OSTN97_NAME);
		if (CS_access (cs_Dir,4) != 0)
		{
			CS_stcpy (cs_DirP,cs_OSTN97_NAME_BIN);
			if (CS_access (cs_Dir,4) != 0)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_OSTN97;
			}
		}
	}
	else if (prj_code == cs_PRJCOD_OSTN02)
	{
		/* First check, whether the txt representation of the OST02 file exists. If it doesn't, look for
		the binary file (which might have been compiled before).
		All the other parameters are hard coded into the projection. */
		CS_stcpy (cs_DirP,cs_OSTN02_NAME);
		if (CS_access (cs_Dir,4) != 0) //txt file...
		{
			CS_stcpy (cs_DirP,cs_OSTN02_NAME_BIN); //...binary file
			if (CS_access (cs_Dir,4) != 0)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_OSTN02;
			}
		}
	}
	else if (prj_code != cs_PRJCOD_UTM)
	{
		/* Basic Transverse Mercator specific stuff. */
	
		if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
		}
		
		if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
		}
		if (prj_code != cs_PRJCOD_GAUSSK)
		{
			if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
			}
		}
		if (prj_code == cs_PRJCOD_TRMRKRG)
		{
			if (fabs (cs_def->org_lat) > cs_ParmTest)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_TMKRG0;
			}
		}
	}
	else
	{
		/* Here for the UTM projection. */

		if (cs_def->prj_prm1 < cs_One || cs_def->prj_prm1 > cs_K60)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_UTMZON;
		}
		if (cs_def->prj_prm2 < cs_Mone || cs_def->prj_prm2 > cs_One)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_HMISPHR;
		}
	}

	/* Some checks for the Affine post Processor. */
	if (prj_code == cs_PRJCOD_TRMERAF)
	{
		tmp1 = cs_def->prj_prm4 * cs_def->prj_prm7 - cs_def->prj_prm5 * cs_def->prj_prm6;
		if (fabs (tmp1) <= 1.0E-12)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_AFFZERO;
		}
	}

	/* That's it for Transverse Mercator. */

	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CStrmrtS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**									system parameters.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**
**	This is an internal function.  Calling this function with
**	erroneous data in the cs_Csprm_ structure can cause
**	matherr type problems.  Specifically, if either scl_red,
**	scale, or e_rad is zero you are guaranteed to get some divide
**	by zeros.
**********************************************************************/

void EXP_LVL9 CStrmerS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern short cs_QuadMapSO [];		/* South oriented quad map */
	extern double cs_Radian;			/*  57.29577... */
	extern double cs_Degree;			/*   1.0 / 57.29577... */
	extern double cs_Zero;				/*   0.0 */
	extern double cs_Fourth;			/*   0.25 */
	extern double cs_Half;				/*   0.5 */
	extern double cs_One;				/*   1.0 */
	extern double cs_Two;				/*   2.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_EETest;			/* .001 seconds of arc
										   short of 90 degrees
										   in radians; used to
										   test for excessive
										   eastern longitude. */

	int zoneNbr;

	struct cs_Trmer_ *trmer;

	double sin_org_lat;
	double height;
	double p_rad;
	double flat;
	double zone_wd;					/* An approximation of the
									   width of the coordinate
									   system, in radians. */
	double nTerm2;
	double nTerm3;
	double nTerm4;
	double tmp1;

	double test_ll [2];
	double test_xy [2];

	trmer = &csprm->proj_prms.trmer;

	/* Setup of basic parameters is variation dependent. Note that
	   we ASS-U-ME that the definition has passed muster as determined
	   by CStrmerQ. */
	height = cs_Zero;
	trmer->affineA0 = cs_Zero;
	trmer->affineB0 = cs_Zero;
	trmer->affineA1 = cs_One;
	trmer->affineA2 = cs_Zero;
	trmer->affineB1 = cs_Zero;
	trmer->affineB2 = cs_One;
	trmer->affineDenom = cs_One;

	/* Set as as required for the specific variation. */
	trmer->kruger = (csprm->prj_code == cs_PRJCOD_TRMRKRG);
	switch (csprm->prj_code) {

	case cs_PRJCOD_TRMERAF:
		/* Standard case, but with the affine post processor. */
		trmer->affineA0 = csprm->csdef.prj_prm2;
		trmer->affineB0 = csprm->csdef.prj_prm3;
		trmer->affineA1 = csprm->csdef.prj_prm4;
		trmer->affineA2 = csprm->csdef.prj_prm5;
		trmer->affineB1 = csprm->csdef.prj_prm6;
		trmer->affineB2 = csprm->csdef.prj_prm7;
		/* Fall through to standard stuff by design. */
		/*lint -fallthrough */

	default:
	case cs_PRJCOD_TRMRKRG:
	case cs_PRJCOD_TRMER:
		/* This is the standard stuff, this also the default. */
		trmer->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
		trmer->org_lat = csprm->csdef.org_lat * cs_Degree;
		trmer->x_off = csprm->csdef.x_off;
		trmer->y_off = csprm->csdef.y_off;
		trmer->k0 = csprm->csdef.scl_red;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_GAUSSK:
		/* Gauss Kruger variation, scale reduction forced to 1.0. */
		trmer->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
		trmer->org_lat = csprm->csdef.org_lat * cs_Degree;
		trmer->x_off = csprm->csdef.x_off;
		trmer->y_off = csprm->csdef.y_off;
		trmer->k0 = cs_One;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_UTM:
		/* Here we have only two parameters, the Zone number
		   and the hemisphere. We figure out everything
		   else.  */
		zoneNbr = (int) csprm->csdef.prj_prm1;
		trmer->cent_lng = (double)(-183 + (6 * zoneNbr)) * cs_Degree;	/*lint !e790 */
		trmer->org_lat = cs_Zero;
		trmer->x_off = 500000.0 * csprm->csdef.scale;
		if (csprm->csdef.prj_prm2 >= 0.0) trmer->y_off = cs_Zero;
		else trmer->y_off = 10000000.0 * csprm->csdef.scale;
		trmer->k0 = 0.9996;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_WCCST:
		/* This is the same as the standard stuff, but we use
		   two extra parameters to change the size and shape
		   of the reference ellipsoid. */
		trmer->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
		trmer->org_lat = csprm->csdef.org_lat * cs_Degree;
		trmer->x_off = csprm->csdef.x_off;
		trmer->y_off = csprm->csdef.y_off;
		trmer->k0 = csprm->csdef.scl_red;

		height = csprm->csdef.prj_prm3 * csprm->csdef.unit_scl;
		height += csprm->csdef.prj_prm2;

		trmer->e_rad = csprm->datum.e_rad + height;
		p_rad = csprm->datum.p_rad + height;
		flat = cs_One - p_rad / trmer->e_rad;
		trmer->ecent = sqrt (flat + flat - flat * flat);
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_MNDOTT:
		/* Minnesota DOT variation. A bit more for this
		   variation appears below. */
		height = csprm->csdef.prj_prm2 * csprm->csdef.unit_scl;
		trmer->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
		trmer->org_lat = csprm->csdef.org_lat * cs_Degree;
		trmer->x_off = csprm->csdef.x_off;
		trmer->y_off = csprm->csdef.y_off;
		trmer->k0 = cs_One;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_SOTRM:
		/* This is the south oriented variation. All we need do
		   is jerk the quad value around. I.e. use quad == 3
		   is the normal thing for this projection. */
		trmer->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
		trmer->org_lat = csprm->csdef.org_lat * cs_Degree;
		trmer->x_off = csprm->csdef.x_off;
		trmer->y_off = csprm->csdef.y_off;
		trmer->k0 = csprm->csdef.scl_red;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMapSO [csprm->csdef.quad - cs_QuadMin];
		break;

	case cs_PRJCOD_OSTN97:
	case cs_PRJCOD_OSTN02:
		/* This is a real odd ball as it is both a projection and a datum
		   transformation.  The ellipsoid should always be GRS1980, and
		   the unit should always be meters.  We force all the rest of the
		   parameters to appropriate values.
		   
		   Should anyone out there want to enforce the ellipsoid and
		   unit, I suggest that they do that in the CStrmerQ function. */
		trmer->cent_lng = -2.0 * cs_Degree;
		trmer->org_lat = 49.0 * cs_Degree;
		trmer->x_off = 400000.0 * csprm->csdef.scale;
		trmer->y_off = -100000.0 * csprm->csdef.scale;
		trmer->k0 = 0.9996012717;
		trmer->e_rad = csprm->datum.e_rad;
		trmer->ecent = csprm->datum.ecent;
		trmer->e_sq = trmer->ecent * trmer->ecent;
		trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

		/* One might think that we would open the cs_Ostn97_ object
		   here.  However, while projections have setup functions
		   which are the equivalent of a constructor, projections
		   do not have the equivalent of a destructor.  Therefore,
		   dealing with the cs_Ostn97_ object here would produce
		   a file descriptor and memory leak.  This problem is
		   dealt with in the Forward and Inverse functions. */
		break;
	}

	/* Now, with one exception, everything else is the same for
	   all variations. */
	trmer->k = csprm->csdef.scale * trmer->k0;
	trmer->Rk = trmer->e_rad * trmer->k;
	sin_org_lat = sin (trmer->org_lat);

	if (trmer->ecent == 0.0)
	{
		/* Here for a sphere. */
		trmer->Rk_ovr_2 = trmer->Rk * cs_Half;
	}
	else
	{
		/* Here for an ellipsoid. */
		trmer->eprim_sq = trmer->e_sq / (cs_One - trmer->e_sq);

		/* The coefficients for the M calculations. */
		CSmmFsu (&trmer->mmcofF,trmer->e_rad,trmer->e_sq);
		CSmmIsu (&trmer->mmcofI,trmer->e_rad,trmer->e_sq);

		/* Now we can compute M0, the meridonal distance
		   from the equator to the origin latitude. */
		trmer->M0 = CSmmFcal (&trmer->mmcofF,trmer->org_lat,
						  sin_org_lat,
						  cos (trmer->org_lat));

		if (trmer->kruger != 0)
		{
			/* The following are terms used by the Kruger Formulation. */
			p_rad = trmer->e_rad * sqrt (cs_One - trmer->e_sq);
			trmer->nTerm = (trmer->e_rad - p_rad) / (trmer->e_rad + p_rad);
			nTerm2 = trmer->nTerm * trmer->nTerm;
			nTerm3 = nTerm2 * trmer->nTerm;
			nTerm4 = nTerm2 * nTerm2;
			tmp1 = cs_One + cs_Fourth * nTerm2 + (1.0 / 64.0) * nTerm4;
			trmer->aHat = cs_Half * (trmer->e_rad + p_rad) * tmp1;
			trmer->k0aHat = trmer->aHat * trmer->k0;

			trmer->beta1 = (     1.0 /      2.0) * trmer->nTerm -
						   (     2.0 /      3.0) * nTerm2 +
						   (     5.0 /     16.0) * nTerm3 +
						   (    41.0 /    180.0) * nTerm4;
						   
			trmer->beta2 = (    13.0 /     48.0) * nTerm2 -
						   (     3.0 /      5.0) * nTerm3 +
						   (   557.0 /   1440.0) * nTerm4;
						   
			trmer->beta3 = (    61.0 /    240.0) * nTerm3 -
						   (   103.0 /    140.0) * nTerm4;
						   
			trmer->beta4 = ( 49561.0 / 161280.0) * nTerm4;
		
			trmer->delta1 = (    1.0 /      2.0) * trmer->nTerm -
							(    2.0 /      3.0) * nTerm2 +
							(   37.0 /     96.0) * nTerm3 -
							(    1.0 /    360.0) * nTerm4;
							
			trmer->delta2 = (    1.0 /     48.0) * nTerm2 +
							(    1.0 /     15.0) * nTerm3 -
							(  437.0 /   1440.0) * nTerm4;
							
			trmer->delta3 = (   17.0 /    480.0) * nTerm3 -
							(   37.0 /    840.0) * nTerm4;
							
			trmer->delta4 = ( 4397.0 / 161280.0) * nTerm4; 

			/* Set up the conformal latitude series calculations. */
			CSchiFsu (&trmer->chicofF,trmer->e_sq);
			CSchiIsu (&trmer->chicofI,trmer->e_sq);
		}
		else
		{
			trmer->nTerm  = trmer->aHat   = trmer->k0aHat = cs_Zero;
			trmer->beta1  = trmer->beta2  = trmer->beta3  = trmer->beta4  = cs_Zero;
			trmer->delta1 = trmer->delta2 = trmer->delta3 = trmer->delta4 = cs_Zero;
		}
	}

	/* One extra tweak for the Minnesota Department of Transportation. */
	trmer->var_K = cs_One;
	if (csprm->prj_code == cs_PRJCOD_MNDOTT)
	{
		tmp1 = trmer->e_sq * sin_org_lat * sin_org_lat;
		tmp1 = trmer->e_rad / sqrt (cs_One - tmp1);
		trmer->var_K = (tmp1 + height) / tmp1;
		trmer->quad |= csTRMER_MNDOT;
	}

	/* Compute some terms used in the Affine inverse to keep
	   performance up to the max. */
	if (csprm->prj_code == cs_PRJCOD_TRMERAF)
	{
		trmer->affineK = sqrt (trmer->affineA1 * trmer->affineA1 + trmer->affineB1 * trmer->affineB1);
		if (trmer->affineB2 != 0.0 || trmer->affineA2 != 0.0)
		{
			trmer->affineGamma = -atan2 (trmer->affineA2,trmer->affineB2);
		}
		else
		{
			trmer->affineGamma = cs_Zero;
		}
		trmer->affineDenom = trmer->affineA1 * trmer->affineB2 - trmer->affineA2 * trmer->affineB1;
		trmer->quad |= csTRMER_AFFINE;
	}
	else if (csprm->prj_code == cs_PRJCOD_OSTN97)
	{
		trmer->quad |= csTRMER_OSTN97;

		/* We don't try to open the file here, as there is no way to indicate
		   an error condition in a setup function. */
	}
	else if (csprm->prj_code == cs_PRJCOD_OSTN02)
	{
		trmer->quad |= csTRMER_OSTN02;

		/* We don't try to open the file here, as there is no way to indicate
		   an error condition in a setup function. */
	}

	/* Need to compute the maximum value of X which can be
	   supported mathematically.  This is, essentially, the
	   value generated by a del_lng of a skosh less than
	   90 degrees at the equator.  Since most all the setup
	   is complete, we can use the CStrmerF function to
	   compute this for us. */

	test_ll [LNG] = CS_adj2pi (trmer->cent_lng + cs_EETest) * cs_Radian;
	test_ll [LAT] = cs_Zero;
	CStrmerF (trmer,test_ll,test_ll);				/*lint !e534   ignoring return value */
	trmer->xx_max = fabs (test_ll [XX] - trmer->x_off);

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */
	csprm->cent_mer = trmer->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 && csprm->csdef.ll_min [LAT] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0 && csprm->csdef.ll_max [LAT] == 0.0)
	{
		/* Here to calculate a useful range for this coordinate system. */
		if (csprm->prj_code == cs_PRJCOD_UTM)
		{
			/* Here for a hard core UTM zone, which is quite frequent.
			   This is pretty easy.  The value picked is pretty
			   arbitrary once you go above 3 degrees. */
			csprm->min_ll [LNG] = -3.5;
			csprm->max_ll [LNG] =  3.5;
			if (csprm->csdef.prj_prm2 >= 0.0)
			{
				csprm->min_ll [LAT] = -1.0;
				csprm->max_ll [LAT] = 84.0;
			}
			else
			{
				csprm->min_ll [LAT] = -80.0;
				csprm->max_ll [LAT] =   1.0;
			}
		}
		else
		{
			/* Not a UTM zone.  We presume that the scale reduction
			   factor indicates the width of the zone.  Note, there
			   are some coordinate systems where the scale reduction
			   factor is greater than 1; strange as it might seem. */
			tmp1 = trmer->k0;
			if (tmp1 > cs_One) tmp1 = cs_One;
			if (tmp1 >= cs_One)
			{
				csprm->min_ll [LNG] = -1.75;
				csprm->max_ll [LNG] =  1.75;
			}
			else
			{
				zone_wd = (cs_Two * acos (tmp1) * cs_Radian) + 0.5;
				csprm->min_ll [LNG] = -zone_wd;
				csprm->max_ll [LNG] =  zone_wd;
			}

			/* Now for the latitude, which is kind of interesting. */
            if (csprm->prj_code == cs_PRJCOD_SOTRM) /* This is a south oriented variation. The expectation is that this projection SOTRM is used on the south hemisphere. */
            {
                csprm->max_ll [LAT] = trmer->org_lat * cs_Radian;
			    csprm->max_ll [LAT] += trmer->y_off / 111000.000;
			    if (csprm->max_ll [LAT] > cs_K90) csprm->max_ll [LAT] = cs_K90;
			    csprm->min_ll [LAT] = csprm->max_ll [LAT] - cs_K90;
			    if (csprm->min_ll [LAT] < cs_Km90) csprm->min_ll [LAT] = cs_Km90;
            }
            else
            {
			    csprm->min_ll [LAT] = trmer->org_lat * cs_Radian;
			    csprm->min_ll [LAT] -= trmer->y_off / 111000.000;
			    if (csprm->min_ll [LAT] < cs_Km90) csprm->min_ll [LAT] = cs_Km90;
			    csprm->max_ll [LAT] = csprm->min_ll [LAT] + cs_K90;
			    if (csprm->max_ll [LAT] > cs_K90) csprm->max_ll [LAT] = cs_K90;
            }
		}
	}
	else
	{
		/* The definition includes a useful range specification.
		   We use these values without checking.  We expect the
		   user to give us absolute values, and we convert
		   to values relative to the central meridian. */
		csprm->min_ll [LNG] = CS_adj180 (csprm->csdef.ll_min [LNG] - csprm->cent_mer);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj180 (csprm->csdef.ll_max [LNG] - csprm->cent_mer);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the X's and Y's.  If the coordinate system
	   definition carries some values, we use them.  If not, we
	   calculate some appropriate values. */
	if (csprm->csdef.xy_min [XX] == 0.0 && csprm->csdef.xy_min [YY] == 0.0 &&
		csprm->csdef.xy_max [XX] == 0.0 && csprm->csdef.xy_max [YY] == 0.0)
	{
		/* No specification in the coordinate system definition.
		   The setup is virtually complete, so we can use CStrmerF
		   to calculate these values.  This works if we're very
		   careful. */
		test_ll [LNG] = CS_adj180 (csprm->cent_mer + csprm->max_ll [LNG]);

		/* Make sure we do this calculation with the latitude which is the
		   smallest in magnitude. */
		tmp1 = trmer->org_lat * cs_Radian;
		if (fabs (csprm->min_ll [LAT]) < fabs (tmp1)) tmp1 = csprm->min_ll [LAT];
		if (fabs (csprm->max_ll [LAT]) < fabs (tmp1)) tmp1 = csprm->max_ll [LAT];
		test_ll [LAT] = tmp1;

		/* Convert the resulting point, and therby obtain the min/max in X. */
		CStrmerF (trmer,test_xy,test_ll);				/*lint !e534   ignoring return value */
		tmp1 = test_xy [XX] - trmer->x_off;
		csprm->max_xy [XX] = tmp1;
		csprm->min_xy [XX] = -tmp1;

		/* Somthing similar in Y.  Again, be very careful about the
		   the points which you pick. */
		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = csprm->min_ll [LAT];
		CStrmerF (trmer,test_xy,test_ll);				/*lint !e534   ignoring return value */
		csprm->min_xy [YY] = test_xy [YY] - trmer->y_off;

		test_ll [LAT] = csprm->max_ll [LAT];
		CStrmerF (trmer,test_xy,test_ll);				/*lint !e534   ignoring return value */
		csprm->max_xy [YY] = test_xy [YY] - trmer->y_off;

		/* Apply quad processing, e.g. a left handed coordinate system. */
		CS_quadMM (csprm->min_xy,csprm->max_xy,trmer->x_off,
											   trmer->y_off,
											   trmer->quad);
	}
	else
	{
		/* Use what ever the user has given us.  No adjustment
		   necessary.  Note: we don't check anything. */
		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}

	/* That's all the calculations we can do at this time.
	   Set up the internal function calls.  Note, since the
	   Transverse Mercator is a conformal projection, the
	   H and K scale factors are the same.  Therefore, we
	   set all three scale function pointers to the same
	   function. */
	csprm->ll2cs    = (cs_LL2CS_CAST)CStrmerF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CStrmerI;
	csprm->cs_scale = (cs_SCALE_CAST)CStrmerK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CStrmerK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CStrmerK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CStrmerC;
	csprm->llchk    = (cs_LLCHK_CAST)CStrmerL;
	csprm->xychk    = (cs_XYCHK_CAST)CStrmerX;

	return;
}

/**********************************************************************
**	rtn_val = CStrmerF (trmer,xy,ll);
**
**	struct cs_Trmer_ *trmer;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]) values
**								to be converted, in degrees.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and the xy arrays may share the same memory
**	with no adverse affects.
**********************************************************************/

int EXP_LVL9 CStrmerF (Const struct cs_Trmer_ *trmer,double xy [2],Const double ll [2])
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern struct cs_Ostn97_ *cs_Ostn97Ptr;
	extern struct cs_Ostn02_ *cs_Ostn02Ptr;

	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Pi_o_2;			/* Pi over two */
	extern double cs_Mpi_o_2;			/* minus Pi over two */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_AnglTest;			/* .001 seconds of arc in
										   radians. */
	extern double cs_AnglTest1;			/* 1.0 minus .001 seconds of
										   arc radians. */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the south pole,
										   in radians. */
	extern double cs_EETest;			/* .001 seconds of arc
										   short of 90 degrees,
										   in radians. */
	extern double cs_WETest;			/* .001 seconds of arc
										   short of -90 degrees,
										   in radians. */
	extern double cs_Huge;				/* An approximation of
										   infinity, 1.0E+37,
										   but not so large as
										   we can't use it in
										   calculations. */
	extern double cs_Mhuge;				/* Negative infinity. */

	int st;
	int rtn_val;

	double lng;
	double lat;
	double del_lng;

	double sin_lat;
	double cos_lat;
	double tan_lat;
	double tan_lat_sq;

	double nu;
	double eta_sq;

	double A1;
	double A2;
	double A3;
	double A4;
	double A5;
	double A6;
	double A7;
	double B;
	double L;
	double L_sq;
	double M;

	double mm;
	double tmp1;
	double affineXY [2];

	rtn_val = cs_CNVRT_NRML;
	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	/* This projection blows up in two cases.  A del_lng of
	   +- 90 at the equator or a latitude of +- 90.

	   The del_lng of +- 90 is outside of the normal usage of
	   this projection, so we simply limit it to a value close
	   to these extents.  No one can say that the numbers are
	   inaccurate.

	   A lat of +- 90 yields a specific Y value, but the X
	   value = x_off, regardless of the longitude.

	   The tests, and alternate values provided, in the following
	   lines of code work around all of these problems.  That is,
	   they provide rational results regardless of what lat/long
	   is provided. */

	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
		if (lat >= cs_NPTest)
		{
			/* North Pole */

			sin_lat = cs_One;
			cos_lat = cs_Zero;
			tan_lat = cs_Huge;
			lat = cs_Pi_o_2;
		}
		else if (lat <= cs_SPTest)
		{
			/* South Pole */

			sin_lat = cs_Mone;
			cos_lat = cs_Zero;
			tan_lat = cs_Mhuge;
			lat = cs_Mpi_o_2;
		}
		else
		{
			sin_lat = sin (lat);
			cos_lat = cos (lat);
			tan_lat = sin_lat / cos_lat;
		}
	}
	else
	{
		/* Otherwise.  Since we have handled the poles,
		   cos (lat) should never be zero here. */

		sin_lat = sin (lat);
		cos_lat = cos (lat);
		tan_lat = sin_lat / cos_lat;
	}

	/* cs_Huge isn't so huge as to make the following fail. */

	tan_lat_sq = tan_lat * tan_lat;

	/* That's the latitude, deal with the longitude now. */

	del_lng = lng - trmer->cent_lng;

	/* The following statement deals with the issue of the -180 crack.
	   For example, if the central meridian is -179, and the provided
	   longitude is +179, we want a del_lng of -2. */

	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	/* We can't allow a del_lng value to get too close to
	   PI over 2 (i.e. 90 degrees) if the cosine of the latitude
	   is one (i.e. the equator).  Otherwise, all longitude
	   values can be handled.  Note, that we allow delta longitude
	   values greater than 90, it's the specific value of 90
	   that causes trouble.  When delta longitude exceeds 90,
	   the projection just wraps back on itself, thus providing
	   the "rational" results which we must return. */

	if (del_lng > cs_EETest || del_lng < cs_WETest)
	{
		rtn_val = cs_CNVRT_RNG;
		if (fabs (fabs (del_lng) - cs_Pi_o_2) < cs_AnglTest &&
			fabs (cos_lat) > cs_AnglTest1)
		{
			/* Opps!!! We're in never never land. */

			if (del_lng < 0.0)
			{
				del_lng = cs_Mpi_o_2 + cs_AnglTest;
			}
			else
			{
				del_lng = cs_Pi_o_2 - cs_AnglTest;
			}
		}
	}

	/* OK, we can do our thing pretty safely now. */

	if (trmer->ecent == 0.0)
	{
		/* Here for a sphere.  If the above was written
		   correctly, B will never be precisely +- 1. */

		B = cos_lat * sin (del_lng);
		tmp1 = (cs_One + B) / (cs_One - B);
		xy [XX] = trmer->Rk_ovr_2 * log (tmp1);

		tmp1 = tan_lat / cos (del_lng);
		xy [YY] = trmer->Rk * (atan (tmp1) - trmer->org_lat);
	}
	else
	{
		/* Here for the ellipsoid.  If we are not to use the
		   Kruger Formulation, we use Meade's Nested Solution,
		   which is a high performance variation of Refearns's
		   Formulation. A bit more precise, and significantly faster than
		   Synder's adaptation of Redfearn's equation which was
		   used previously. */
		if (trmer->kruger == 0)
		{
			eta_sq = trmer->eprim_sq * cos_lat * cos_lat;
			M = CSmmFcal (&trmer->mmcofF,lat,sin_lat,cos_lat);

			tmp1 = trmer->e_sq * sin_lat * sin_lat;
			nu = trmer->e_rad / sqrt (cs_One - tmp1);
			A1 = trmer->k * nu;

			A2 = cs_Half * trmer->k * nu * tan_lat;

			A3 = (1.0 / 6.0) * (cs_One - tan_lat_sq + eta_sq);

			A4 = (1.0 / 12.0) * (cs_Five - tan_lat_sq + eta_sq * (cs_Nine + cs_Four * eta_sq));

			tmp1  = cs_Five - tan_lat_sq * (18.0 - tan_lat_sq);
			tmp1 += eta_sq * (14.0 - 58.0 * tan_lat_sq);
			A5 = (1.0 / 120.0) * tmp1;

			tmp1 = 61.0 - tan_lat_sq * (58.0 - tan_lat_sq);
			tmp1 += eta_sq * (270.0 - 330.0 * tan_lat_sq);
			A6 = (1.0 / 360.0) * tmp1;

			A7 = (1.0 / 5040.0) * (61.0 - tan_lat_sq * (479.0 -
														179.0 * tan_lat_sq +
														tan_lat_sq * tan_lat_sq));

			L = del_lng * cos_lat;
			L_sq = L * L;

			xy [XX] = ((((A7 * L_sq) + A5) * L_sq + A3) * L_sq + cs_One) * A1 * L;
			mm = M - trmer->M0;
			xy [YY] = ((((A6 * L_sq) + A4) * L_sq + cs_One) * A2 * L_sq) + trmer->k * mm;
		}
		else
		{
			double sinDeltaLng, cosDeltaLng;
			double chi, cosChi, tanChi;
			double xiPrime, etaPrime;
			double twoXiPrime, twoEtaPrime;

			double reciprocal;
			double exp2EtaPrime;
			double sin2XiPrime,  cos2XiPrime;
			double sinh2EtaPrime,cosh2EtaPrime;
			double sin4XiPrime,  cos4XiPrime;
			double sinh4EtaPrime,cosh4EtaPrime;
			double sin6XiPrime,  cos6XiPrime;
			double sinh6EtaPrime,cosh6EtaPrime;
			double sin8XiPrime,  cos8XiPrime;
			double sinh8EtaPrime,cosh8EtaPrime;

			/* Since the Transverse Mercator is used for 90% of all the mapping in
			   the world, the performance of this code is important.  Important
			   enough that we inline most of it. */
			sinDeltaLng = sin (del_lng);
			cosDeltaLng = cos (del_lng);
			chi = CSchiFcal (&trmer->chicofF,lat);
			cosChi = cos (chi);
			tanChi = tan (chi);

			xiPrime = atan2 (tanChi,cosDeltaLng);
			/* etaPrime = rcArcTanH (cosChi * sinDeltaLng);
			   This goes off the deep end when deltaLng is unreasonable.  In
			   the long run, etaPrime is an approximation of deltaLng, while
			   after refinement will be multiplied by an adjusted radius to
			   produce the appropriate X value.  I relat all this to
			   explain how I arrive at how we handle absurd numbers in order
			   to prevent floating point exceptions. */
			if (fabs (sinDeltaLng) < cs_Half)
			{
				/* We have a realistic deltaLng which the normal calculation
				   should handle OK. */
				tmp1 = cosChi * sinDeltaLng;
				tmp1 = (cs_One + tmp1) / (cs_One - tmp1);
				etaPrime = cs_Half * log (tmp1);
			}
			else
			{
				/* deltaLng is off the charts, greater than 30 degrees. Any
				   result within a couple hundred thousand meters can be
				   considered "rational". */
				etaPrime = sinDeltaLng;
			}

			twoXiPrime  = xiPrime  + xiPrime;
			twoEtaPrime = etaPrime + etaPrime;
			
			sin2XiPrime = sin (twoXiPrime);
			cos2XiPrime = cos (twoXiPrime);
			exp2EtaPrime = exp (twoEtaPrime);
			reciprocal = cs_One / exp2EtaPrime;
			sinh2EtaPrime = cs_Half * (exp2EtaPrime - reciprocal);
			cosh2EtaPrime = cs_Half * (exp2EtaPrime + reciprocal);

			sin4XiPrime = sin2XiPrime * cos2XiPrime + cos2XiPrime * sin2XiPrime;
			cos4XiPrime = cos2XiPrime * cos2XiPrime - sin2XiPrime * sin2XiPrime;
			sinh4EtaPrime = sinh2EtaPrime * cosh2EtaPrime + cosh2EtaPrime * sinh2EtaPrime;
			cosh4EtaPrime = cosh2EtaPrime * cosh2EtaPrime + sinh2EtaPrime * sinh2EtaPrime;
			
			sin6XiPrime = sin4XiPrime * cos2XiPrime + cos4XiPrime * sin2XiPrime;
			cos6XiPrime = cos4XiPrime * cos2XiPrime - sin4XiPrime * sin2XiPrime;
			sinh6EtaPrime = sinh4EtaPrime * cosh2EtaPrime + cosh4EtaPrime * sinh2EtaPrime;
			cosh6EtaPrime = cosh4EtaPrime * cosh2EtaPrime + sinh4EtaPrime * sinh2EtaPrime;

			sin8XiPrime = sin4XiPrime * cos4XiPrime + cos4XiPrime * sin4XiPrime;
			cos8XiPrime = cos4XiPrime * cos4XiPrime - sin4XiPrime * sin4XiPrime;
			sinh8EtaPrime = sinh4EtaPrime * cosh4EtaPrime + cosh4EtaPrime * sinh4EtaPrime;
			cosh8EtaPrime = cosh4EtaPrime * cosh4EtaPrime + sinh4EtaPrime * sinh4EtaPrime;

			xy [XX] = etaPrime;
			xy [YY] = xiPrime;

			xy [XX] += trmer->beta1 * cos2XiPrime * sinh2EtaPrime;
			xy [YY] += trmer->beta1 * sin2XiPrime * cosh2EtaPrime;

			xy [XX] += trmer->beta2 * cos4XiPrime * sinh4EtaPrime;
			xy [YY] += trmer->beta2 * sin4XiPrime * cosh4EtaPrime;

			xy [XX] += trmer->beta3 * cos6XiPrime * sinh6EtaPrime;
			xy [YY] += trmer->beta3 * sin6XiPrime * cosh6EtaPrime;

			xy [XX] += trmer->beta4 * cos8XiPrime * sinh8EtaPrime;
			xy [YY] += trmer->beta4 * sin8XiPrime * cosh8EtaPrime;

			xy [XX] *= trmer->k0aHat;
			xy [YY] *= trmer->k0aHat;		
		}
	}

	if (trmer->quad == 0)
	{
		xy [XX] += trmer->x_off;
		xy [YY] += trmer->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],trmer->x_off,trmer->y_off,trmer->quad);
		if ((trmer->quad & csTRMER_MNDOT) != 0)
		{
			xy [XX] *= trmer->var_K;
			xy [YY] *= trmer->var_K;
		}
	}

	/* I'm assuming that the affine transformation applies to the
	   cartesians after the false origin is applied.  I think this
	   is a good assumption. */
	if ((trmer->quad & csTRMER_AFFINE) != 0)
	{
		affineXY [XX] = xy [XX];
		affineXY [YY] = xy [YY];
		xy [XX] = trmer->affineA0 + trmer->affineA1 * affineXY [XX] +
									trmer->affineA2 * affineXY [YY];
		xy [YY] = trmer->affineB0 + trmer->affineB1 * affineXY [XX] +
									trmer->affineB2 * affineXY [YY];
	}
	else if ((trmer->quad & csTRMER_OSTN97) != 0)
	{
		if (cs_Ostn97Ptr == NULL)
		{
			CS_stcpy (cs_DirP,cs_OSTN97_NAME);
			cs_Ostn97Ptr = CSnewOstn97 (cs_Dir);
		}
		if (cs_Ostn97Ptr != NULL)
		{
			st = CSforwardOstn97 (cs_Ostn97Ptr,xy,xy);
			if (st != 0) rtn_val = cs_CNVRT_RNG;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
		}
	}
	else if ((trmer->quad & csTRMER_OSTN02) != 0)
	{
		if (cs_Ostn02Ptr == NULL)
		{
			CS_stcpy (cs_DirP,cs_OSTN02_NAME);
			cs_Ostn02Ptr = CSnewOstn02 (cs_Dir);
		}
		if (cs_Ostn02Ptr != NULL)
		{
			st = CSforwardOstn02 (cs_Ostn02Ptr,xy,xy);
			if (st != 0) rtn_val = cs_CNVRT_RNG;
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
		}
	}
	return (rtn_val);
}
/**********************************************************************
**	rtn_val = CStrmerI (trmer,ll,xy);
**
**	struct cs_Trmer_ *trmer;	structure detailing the Transverse Mercator
**								coordinate system in use.
**	double ll [2];				converted results returned here, longitude ([0])
**								and latitude ([1]), in degrees.
**	double xy [2];				coordinates to be converted are obtained from
**								here, x ([0]) and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The xy and ll arrays may share the same memory.
**********************************************************************/

int EXP_LVL9 CStrmerI (Const struct cs_Trmer_ *trmer,double ll [2],Const double xy [2])
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern struct cs_Ostn97_ *cs_Ostn97Ptr;
	extern struct cs_Ostn02_ *cs_Ostn02Ptr;

	extern double cs_Pi;				/* Pi = 3.14159... */
	extern double cs_Pi_o_2;			/* PI / 2.0 */
	extern double cs_Mpi_o_2;			/* -PI / 2.0 */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Six;				/* 6.0 */
	extern double cs_Eight;				/* 8.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	int st;
	int rtn_val;

	double xx;
	double yy;

	double lat;
	double del_lng;

	double phi1;
	double sin_phi1;
	double cos_phi1;
	double tan_phi1;
	double tan_phi1_sq;

	double eta1_sq;
	double nu1;
	double B2;
	double B3;
	double B4;
	double B5;
	double B6;
	double B7;
	double D;
	double L;
	double M;
	double Q;
	double Q_sq;

	double tmp1;
	double tmp2;

	double myXY [3];

	rtn_val = cs_CNVRT_NRML;

	if (trmer->quad == 0)
	{
		/* Do the normal case very fast. */
		xx = xy [XX] - trmer->x_off;
		yy = xy [YY] - trmer->y_off;
	}
	else
	{
		/* Here for all strange variations.  This is a bit slower. */
		if ((trmer->quad & csTRMER_AFFINE) != 0)
		{
			myXY [XX]  = trmer->affineB2 * (xy [XX] - trmer->affineA0) - trmer->affineA2 * (xy [YY] - trmer->affineB0);
			myXY [XX] /= trmer->affineDenom;

			myXY [YY]  = trmer->affineA1 * (xy [YY] - trmer->affineB0) - trmer->affineB1 * (xy [XX] - trmer->affineA0);
			myXY [YY] /= trmer->affineDenom;
		}
		else if ((trmer->quad & csTRMER_OSTN97) != 0)
		{
			if (cs_Ostn97Ptr == NULL)
			{
				CS_stcpy (cs_DirP,cs_OSTN97_NAME);
				cs_Ostn97Ptr = CSnewOstn97 (cs_Dir);
			}
			if (cs_Ostn97Ptr != NULL)
			{
				st = CSinverseOstn97 (cs_Ostn97Ptr,myXY,xy);
				if (st != 0) rtn_val = cs_CNVRT_RNG;
			}
			else
			{
				/* Make it very obvious that something is wrong. */
				myXY [XX] = cs_Zero;
				myXY [YY] = cs_Zero;
				rtn_val = cs_CNVRT_RNG;
			}
		}
		else if ((trmer->quad & csTRMER_OSTN02) != 0)
		{
			if (cs_Ostn02Ptr == NULL)
			{
				CS_stcpy (cs_DirP,cs_OSTN02_NAME);
				cs_Ostn02Ptr = CSnewOstn02 (cs_Dir);
			}
			if (cs_Ostn02Ptr != NULL)
			{
				st = CSinverseOstn02 (cs_Ostn02Ptr,myXY,xy);
				if (st != 0) rtn_val = cs_CNVRT_RNG;
			}
			else
			{
				/* Make it very obvious that something is wrong. */
				myXY [XX] = cs_Zero;
				myXY [YY] = cs_Zero;
				rtn_val = cs_CNVRT_RNG;
			}
		}
		else
		{
			myXY [XX] = xy [XX];
			myXY [YY] = xy [YY];
		}
		if ((trmer->quad & csTRMER_MNDOT) != 0)
		{
			myXY [XX] /= trmer->var_K;
			myXY [YY] /= trmer->var_K;
		}
		CS_quadI (&xx,&yy,myXY,trmer->x_off,trmer->y_off,trmer->quad);
	}

	/* Deal with the limiting xx case.  Xx_max is the value which
	   the forward function returns for a latitude of zero and
	   a delta longitude of +-90.0 degrees.  This is the limit
	   of the map.  In any case, it is about 10 times the
	   scaled radius of the earth (Rk).

	   Since latitude is determined primarily by the sin (yy/R),
	   it wraps around automatically.  Therefore, there is no
	   mathematical limit on Y. */

	if (fabs (xx) > trmer->xx_max)
	{
		rtn_val = cs_CNVRT_RNG;
		xx = (xx >= 0.0) ? trmer->xx_max : -trmer->xx_max;
	}

	if (trmer->ecent == 0.0)
	{
		/* Here for a sphere.  From Synder. */

		D = yy / trmer->Rk + trmer->org_lat;
		tmp1 = xx / trmer->Rk;

		tmp2 = sin (D) / cosh (tmp1);	/* cosh is never < 1 */
		lat = asin (tmp2);

		/* Careful, cos (D) is zero at the poles. */

		tmp2 = cos (D);
		if (fabs (tmp2) > cs_AnglTest)
		{
			del_lng = atan2 (sinh (tmp1),tmp2);
		}
		else
		{
			/* Longitude is undefined at the poles, so
			   we return a cs_CNVRT_INDF. */

			del_lng = cs_Zero;
			if (rtn_val == cs_CNVRT_NRML)
			{
				rtn_val = cs_CNVRT_INDF;
			}
		}
	}
	else
	{
		/* Here for an ellipsoid.  Unless the Kruger Formulation
		   is stipulated, we use Meade's Nested Solution, a
		   high performance variation of Refearn's Formulation. */
		   
		if (trmer->kruger == 0) 
		{   
			/*   Phi1 is the footprint
			latitide, which may (unlikely but possible) be
			a pole.  We use Synder, page 63 for the footprint
			latitude. */

			M = trmer->M0 + (yy / trmer->k);
			phi1 = CSmmIcal (&trmer->mmcofI,M);
			cos_phi1 = cos (phi1);
			if (fabs (cos_phi1) < cs_AnglTest)
			{
				/* Here for a pole.  The longitude is not
				defined at wither pole, so we return
				cs_CNVRT_INDF to indicate this. */

				del_lng = cs_Zero;
				lat = (phi1 < 0.0) ? -cs_Pi_o_2 : cs_Pi_o_2;
				rtn_val = cs_CNVRT_INDF;
			}
			else
			{
				sin_phi1 = sin (phi1);
				tan_phi1 = sin_phi1 / cos_phi1;
				tan_phi1_sq = tan_phi1 * tan_phi1;

				/* Using Meade's nested solution.  From
				Maling, page 447. */

				tmp1 = cs_One - trmer->e_sq * sin_phi1 * sin_phi1;
				nu1 = trmer->e_rad / sqrt (tmp1);

				eta1_sq = trmer->eprim_sq * cos_phi1 * cos_phi1;

				Q = xx / (trmer->k * nu1);
				Q_sq = Q * Q;

				B2 = (-1.0 / 2.0) * tan_phi1 * (cs_One + eta1_sq);

				B3 = (-1.0 / 6.0) * (cs_One + cs_Two * tan_phi1_sq + eta1_sq);

				tmp1 = cs_One - cs_Nine * tan_phi1_sq - cs_Four * eta1_sq;
				tmp1 = cs_Five + cs_Three * tan_phi1_sq + eta1_sq * tmp1;
				B4   = (-1.0 / 12.0) * tmp1;

				tmp1  = eta1_sq * (cs_Six + cs_Eight * tan_phi1_sq);
				tmp1 += tan_phi1_sq * (28.0 + 24.0 * tan_phi1_sq);
				B5 = (1.0 / 120.0) * (cs_Five + tmp1);

				/* Some programs use 60 in place of 90 in the
				following.  I believe the 90 is correct.
				The difference this makes is very small.
				Perhaps why the bug in those programs has
				never been found.  I've never had bugs like
				that, of course :-). */

				tmp1 = 46.0 - 252.0 * tan_phi1_sq - 90.0 * (tan_phi1_sq * tan_phi1_sq);
				tmp1 = tan_phi1_sq * (90.0 + 45.0 * tan_phi1_sq) + eta1_sq * tmp1;
				B6   = (61.0 + tmp1) * (1.0 / 360.0);

				tmp1 = 662.0 + 1320.0 * tan_phi1_sq + 720.0 * (tan_phi1_sq * tan_phi1_sq);
				B7   = (-1.0 / 5040.0) * (61.0 + tan_phi1_sq * tmp1);

				L = Q * (cs_One + Q_sq * (B3 + Q_sq * (B5 + B7 * Q_sq)));
				del_lng = L / cos_phi1;

				lat = phi1 + B2 * Q_sq * (cs_One + Q_sq * (B4 + B6 * Q_sq));
			}
		}
		else
		{
			double chi;
			double exp2Eta, expEtaPrime;
			double reciprocal;
			double eta,xi;
			double twoEta,twoXi;
			double sin2Xi, cos2Xi;
			double sinh2Eta,cosh2Eta;
			double sin4Xi, cos4Xi;
			double sinh4Eta,cosh4Eta;
			double sin6Xi, cos6Xi;
			double sinh6Eta,cosh6Eta;
			double sin8Xi, cos8Xi;
			double sinh8Eta,cosh8Eta;
			double etaPrime,xiPrime;
			double sinXiPrime, cosXiPrime;
			double sinhEtaPrime,coshEtaPrime;
	
			eta = xx / trmer->k0aHat;
			xi  = yy / trmer->k0aHat;
			if (fabs (xi) > cs_Pi_o_2)
			{
				/* The Y coordinate appears to off the charts, north of the
				   north pole or south of the south pole.  To prevent a
				   possible floating point exception, we trucate it now to
				   what may be deemed a "rational" value. */
				xi = (yy > 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
			}

			twoEta = eta + eta;
			twoXi  = xi  + xi;

			sin2Xi = sin (twoXi);
			cos2Xi = cos (twoXi);
			exp2Eta = exp (twoEta);

			reciprocal = cs_One / exp2Eta;

			sinh2Eta = cs_Half * (exp2Eta - reciprocal);
			cosh2Eta = cs_Half * (exp2Eta + reciprocal);

			sin4Xi = sin2Xi * cos2Xi + cos2Xi * sin2Xi;
			cos4Xi = cos2Xi * cos2Xi - sin2Xi * sin2Xi;
			sinh4Eta = sinh2Eta * cosh2Eta + cosh2Eta * sinh2Eta;;
			cosh4Eta = cosh2Eta * cosh2Eta + sinh2Eta * sinh2Eta;
			
			sin6Xi = sin4Xi * cos2Xi + cos4Xi * sin2Xi;
			cos6Xi = cos4Xi * cos2Xi - sin4Xi * sin2Xi; 
			sinh6Eta = sinh4Eta * cosh2Eta + cosh4Eta * sinh2Eta;
			cosh6Eta = cosh4Eta * cosh2Eta + sinh4Eta * sinh2Eta;

			sin8Xi = sin4Xi * cos4Xi + cos4Xi * sin4Xi;
			cos8Xi = cos4Xi * cos4Xi - sin4Xi * sin4Xi;
			sinh8Eta = sinh4Eta * cosh4Eta + cosh4Eta * sinh4Eta;
			cosh8Eta = cosh4Eta * cosh4Eta + sinh4Eta * sinh4Eta;

			etaPrime = eta;
			xiPrime  = xi;

			etaPrime -= trmer->delta1 * cos2Xi * sinh2Eta;
			xiPrime  -= trmer->delta1 * sin2Xi * cosh2Eta;

			etaPrime -= trmer->delta2 * cos4Xi * sinh4Eta;
			xiPrime  -= trmer->delta2 * sin4Xi * cosh4Eta;

			etaPrime -= trmer->delta3 * cos6Xi * sinh6Eta;
			xiPrime  -= trmer->delta3 * sin6Xi * cosh6Eta;

			etaPrime -= trmer->delta4 * cos8Xi * sinh8Eta;
			xiPrime  -= trmer->delta4 * sin8Xi * cosh8Eta;

			expEtaPrime = exp (etaPrime);
			reciprocal = cs_One / expEtaPrime;
			sinhEtaPrime = cs_Half * (expEtaPrime - reciprocal);
			coshEtaPrime = cs_Half * (expEtaPrime + reciprocal);
			sinXiPrime = sin (xiPrime);
			cosXiPrime = cos (xiPrime);

			del_lng = atan2 (sinhEtaPrime,cosXiPrime);
			tmp1 = sinXiPrime / coshEtaPrime;
			if (fabs (tmp1) <  cs_One)
			{
				chi = asin (tmp1);
				lat = CSchiIcal (&trmer->chicofI,chi);
			}
			else
			{
				lat = (tmp1 < 0.0) ? cs_Mpi_o_2 : cs_Pi_o_2;
			}
		}
	}

	if (fabs (del_lng) > cs_Pi)
	{
		if (rtn_val == cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_RNG;
		}
		del_lng = CS_adj2pi (del_lng);
	}

	ll [LNG] = (del_lng + trmer->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	alpha = CStrmerC (trmer,ll);
**
**	struct m_Trmer_ *trmer;		structure containing all parameters
**									necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double alpha;				the convergence angle, in degrees;
**								i.e. the deviation btewteen true
**								north and grid north.
**
**	In this case, the reference is:
**********************************************************************/

double EXP_LVL9 CStrmerC (Const struct cs_Trmer_ *trmer,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Third;				/* 0.3333333333333333 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_Huge;				/* An approximation of
										   infinity which is small
										   enough to calculate with
										   without producing
										   FP faults. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the south pole
										   in radians. */
	extern double cs_EETest;			/* 0.001 seconds of arc
										   short of +90.00, in
										   radians. */
	extern double cs_WETest;			/* 0.001 seconds of arc
										   short of -90.00, in
										   radians. */

	double alpha;
	double lng;
	double lat;
	double del_lng;

	double cos_lat;
	double tan_lat;
	double cos_lat_sq;
	double tan_lat_sq;

	double eta_sq;
	double C3;
	double C5;
	double L;
	double L_sq;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	del_lng = CS_adj2pi (lng - trmer->cent_lng);
	if (del_lng < cs_WETest) del_lng = cs_WETest;
	if (del_lng > cs_EETest) del_lng = cs_EETest;

	if (lat > cs_NPTest)
	{
		tan_lat = cs_Huge;
		cos_lat = cs_Zero;
	}
	else if (lat < cs_SPTest)
	{
		tan_lat = -cs_Huge;
		cos_lat = cs_Zero;
	}
	else
	{
		cos_lat = cos (lat);
		tan_lat = tan (lat);
	}
	cos_lat_sq = cos_lat * cos_lat;
	tan_lat_sq = tan_lat * tan_lat;

	L = del_lng * cos_lat;
	L_sq = L * L;

	if (trmer->ecent == 0.0)
	{
		/* For the sphere.  We use the ellipsoid equations,
		   with eccentricity set to zero. */

		eta_sq = cs_Zero;
	}
	else
	{
		/* For the ellipsoid. */

		eta_sq = trmer->eprim_sq * cos_lat_sq;
	}

	/* Fom Maling, page 445. */

	/* C1 = tan_lat */

	C3 = cs_Third * (cs_One + eta_sq * (cs_Three + cs_Two * eta_sq));

	C5 = (1.0 / 15.0) * (cs_Two - tan_lat_sq);

	alpha = tan_lat * L * (cs_One + L_sq * (C3 + (C5 * L_sq)));

	/* If we have an affine post process, we need to factor that in.
	   I just know I'm going to get the sign wrong on this.  */
	if ((trmer->quad & csTRMER_AFFINE) != 0) alpha -= trmer->affineGamma;

	/* Return the computed value of alpha. */
	return (alpha * cs_Radian);
}

/**********************************************************************
**	k = CStrmerK (trmer,ll);
**
**	struct m_Trmer_ *trmer;		structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**				latitude ([1]) in degrees.
**	double k;					the true map scale factor at the
**								specified point.  Should be
**								compared with the trmer->k.
**********************************************************************/

double EXP_LVL9 CStrmerK (Const struct cs_Trmer_ *trmer,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi_o_2;			/* Pi over two */
	extern double cs_Mpi_o_2;			/* Minus Pi over two */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the North Pole,
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the South Pole,
										   in radians. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */
	extern double cs_Huge;				/* 1.0E+37, an approximation
										   if infinity which can
										   actually be used without
										   causing floating point
										   errors. */

	double kk;
	double lng;
	double lat;
	double del_lng;

	double cos_lat;
	double cos_lat_sq;
	double tan_lat_sq;

	double B;
	double eta_sq;
	double F2;
	double F4;
	double L_sq;

	double tmp1;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	if (lat > cs_NPTest)
	{
		lat = cs_Pi_o_2;
		cos_lat = cs_Zero;
	}
	else if (lat < cs_SPTest)
	{
		lat = cs_Mpi_o_2;
		cos_lat = cs_Zero;
	}
	else
	{
		cos_lat = cos (lat);
	}
	del_lng = CS_adj2pi (lng - trmer->cent_lng);

	if (trmer->ecent == 0.0)
	{
		/* Here for a sphere; from Synder, page 58.
		   We have a singularity if cos_lat = 1.0 and
		   sin (del_lng) = 1.  In this case, the
		   appropriate scale factor is infinity. */

		B = cos_lat * sin (del_lng);
		tmp1 = cs_One - (B * B);
		if (tmp1 > 1.0E-05)
		{
			kk = trmer->k0 / sqrt (tmp1);
		}
		else
		{
			kk = cs_SclInf;
		}
	}
	else
	{
		/* Here for an ellipsoid, using Meade's nested
		   solution.  From Maling, page 446. */

		if (cos_lat > 0.0)
		{
			cos_lat_sq = cos_lat * cos_lat;
			tmp1 = tan (lat);
			tan_lat_sq = tmp1 * tmp1;
		}
		else
		{
			cos_lat_sq = cs_Zero;
			tan_lat_sq = cs_Huge;
		}

		eta_sq = trmer->eprim_sq * cos_lat_sq;

		F2 = cs_Half * (cs_One + eta_sq);

		F4 = (1.0 / 12.0) * (cs_Five - cs_Four * tan_lat_sq + eta_sq * (cs_Nine - 24.0 * tan_lat_sq));

		L_sq = (del_lng * del_lng) * cos_lat_sq;

		kk = trmer->k0 * (cs_One + F2 * L_sq * (cs_One + F4 * L_sq));
	}

	/* If we have an affine post process, we need to factor that in. */
	if ((trmer->quad & csTRMER_AFFINE) != 0) kk *= trmer->affineK;

	return (kk);
}

/**********************************************************************
**	status = CStrmerL (trmer,cnt,pnts);
**
**	struct cs_Trmer_ *trmer;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][2];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line, or region
**								is completely within the domain of the
**								coordinate system.  Otherwise, cs_CNVRT_DOMN
**								is returned.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**
**	This function expects that the input lat/longs are normalized
**	if appropriate.
**********************************************************************/

static int CStrmerLP (Const struct cs_Trmer_ *trmer,Const double ll [3])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi_o_2;		/* PI / 2.0 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc in
									   radians */
	double del_lng;

	double my_lng, my_lat;

	/* There is one case which we must check for; 90 degrees from
	   the central meridian on the equator.  That's it. */

	my_lng =       ll [LNG] * cs_Degree;
	my_lat = fabs (ll [LAT] * cs_Degree);

	if (my_lat > cs_Pi_o_2) return (cs_CNVRT_DOMN);

	del_lng = fabs (CS_adj2pi (my_lng - trmer->cent_lng));
	if (my_lat < cs_AnglTest &&
		fabs (del_lng - cs_Pi_o_2) < cs_AnglTest)
	{
		return (cs_CNVRT_DOMN);
	}

	return (cs_CNVRT_OK);
}

int EXP_LVL9 CStrmerL (Const struct cs_Trmer_ *trmer,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;
	extern double cs_EETest;			/* .001 seconds of arc less
										   than PI/2 in radians. */
	extern double cs_WETest;			/* -cs_EETest */

	int ii;
	int status;

	double tmp;
	double del_lng;

	/* Check all the points. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
	{
		status = CStrmerLP (trmer,pnts [ii]);
	}
	if (cnt <= 1 || status != cs_CNVRT_OK) return (status);

	/* If cnt is 2, we have a line which we must check. */

	if (cnt == 2)
	{
		/* WHAT WE SHOULD BE DOING:
		   We should be calculating the angular distance
		   from the great circle defined by the the two
		   points to each of the two singularity points.
		   If either distance is less than cs_AnglTest,
		   we have a problem.

		   WHAT WE DO NOW:
		   If the longitude of either point exceeds the
		   central meridian +- pi/2, we have a problem. */

		for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - trmer->cent_lng);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
			}
		}
	}
	else if (cnt == 3)
	{
		/* Can't handle a three point list, the region must
		   be closed. */

		CS_erpt (cs_RGN_PNTCNT);
		return (cs_CNVRT_ERR);
	}
	else
	{
		/* WHAT WE SHOULD BE DOING:
		   The great circle between the two singularity points
		   is a 180 degree segment of the equator.  We should
		   1) use the technique described for the line case
			  to see if any segment of the region actually
			  goes through either of the singularity points.
		   2) count the intersections of all segments in the
			  boundary with the equator segment between the
			  two singularity points.  If the count is even
			  (or zero) we are OK.  Otherwise, we have a problem.

		   WHAT WE DO NOW:
		   We simply see if any of the points has a longitude
		   which puts it outside of the range of central
		   meridian +- pi/2. */

		for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - trmer->cent_lng);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
			}
		}
	}
	return (status);
}

/**********************************************************************
**	status = CStrmer (trmer,cnt,pnts);
**
**	struct cs_Trmer_ *trmer;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CStrmerL to check lat/long
**	values.
**
**	What gets checked depends upon the cnt argument.  A value
**	of zero (or less than that) checks nothing successfully.
**	A value of 1 indicates that a single point is to be checked.
**	a value of 2 indicates that a line is to be checked.  The
**	entire line must reside within the domain of the coordinate
**	system to pass.
**
**	A value of 4 or more for cnt indicates that a region is to
**	be checked.  The pnts array must contain the indicated
**	number of points which defines a region.  The last point
**	in the array must duplicate the first point.  The resulting
**	polygon must be simply connected (no bow ties).  The region
**	passes if the entire region is within the domain of the
**	coordinate system.
**
**	The return value refers to either the point, or the line
**	segment, provided. Return values are as follows:
**
**	cs_CNVRT_OK	the point, entire line segment, or entire region,
**			is within the catresian domain of the coordinate
**			system.
**	cs_CNVRT_DOMN	the point, all or a portion of the line segment,
**			or all or a portion of the region, is not within
**			the cartesian domain of the coordinate system.
**********************************************************************/

int EXP_LVL9 CStrmerX (Const struct cs_Trmer_ *trmer,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double test_val;
	double dummy;

	rtn_val = cs_CNVRT_OK;

	/* All Y values are within the domain of the function.  However,
	   in the cartesian frame of reference, an absolute value of
	   X greater than xx_max is outside of the domain. */

	for (ii = 0;ii < cnt && rtn_val == cs_CNVRT_OK;ii++)
	{
		if (trmer->quad == 0)
		{
			test_val = pnts [ii][XX] - trmer->x_off;
		}
		else
		{
			CS_quadI (&test_val,&dummy,pnts [ii],trmer->x_off,
												 trmer->y_off,
												 trmer->quad);
		}

		if (fabs (test_val) > trmer->xx_max)
		{
			rtn_val = cs_CNVRT_DOMN;
		}
	}
	return (rtn_val);
}
