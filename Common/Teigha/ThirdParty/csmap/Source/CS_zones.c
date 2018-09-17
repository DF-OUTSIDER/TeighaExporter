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

/**********************************************************************
**	zone_cnt = CS_zones (csdef,zones);
**	zone_flg = CSzone (zone_ptr,west,cent,east);
**	zone_ptr = CS_znlocF (zones,zone_cnt,lng,lat);
**	zone_ptr = CS_znlocI (zones,zone_cnt,xx,yy);
**
**	struct cs_Csdef_ *csdef;	definition of the coordinate system
**	struct cs_Zone_ zones [8];	pointer to an array of 8 cs_Zone structures
**								in which the results are returned.
**	int zone_flg;				returns TRUE if a valid zone definition was
**								extracted and returned, else returns FALSE.
**	struct cs_Zone_ *zone_ptr;	pointer to a single zone structure in which
**								results are returned; or a pointer to the
**								appropriate zone if found, else NULL.
**	double west;				cs_Csdef_ parameter which may be the western
**								extent of a zone in degrees.
**	double cent;				cs_Csdef_ parameter which may be the central
**								meridian of a zone in degrees.
**	double east;				cs_Csdef_ parameter which may be the eastern
**								extent of a zone in degrees.
**	int zone_cnt;				number of valid zone definitions located, or
**								contained in a zone definition array.
**	double lng,lat;				lat/long for which a pointer to the appropriate
**								zone is to be returned.
**	double xx,yy;				cartesian coordinate pair for which a pointer to
**								the approriate zone is to be returned.
**
**	CS_zones returns the number of zone definitions located in
**	the provided cs_Csdef_ structure.
**
**	CS_znlocF and CS_znlocI return NULL if an appropriate zone
**	could not be located.
**
**	CSzone exaines its arguments and determines if the three
**	values define a zone.  If so, the zone structure is
**	populated and TRUE is returned.  Otherwise, FALSE is
**	returned.
**********************************************************************/

int EXP_LVL9 CS_zones (	Const struct cs_Csdef_ *csdef,
						struct cs_Zone_ zones [8])
{
	extern double cs_Mpi;			/* -PI */

	int ii;
	int zone_cnt;
	int east_idx;

	struct cs_Zone_ *zp;

	double east_lng;

	zone_cnt = 0;

	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm1,
						csdef->prj_prm2,
						csdef->prj_prm3);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm4,
						csdef->prj_prm5,
						csdef->prj_prm6);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm7,
						csdef->prj_prm8,
						csdef->prj_prm9);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm10,
						csdef->prj_prm11,
						csdef->prj_prm12);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm13,
						csdef->prj_prm14,
						csdef->prj_prm15);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm16,
						csdef->prj_prm17,
						csdef->prj_prm18);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm19,
						csdef->prj_prm20,
						csdef->prj_prm21);
	zone_cnt += CSzone (&zones [zone_cnt],
						csdef->prj_prm22,
						csdef->prj_prm23,
						csdef->prj_prm24);

	east_lng = cs_Mpi;
	east_idx = -1;
	for (ii = 0;ii < zone_cnt;ii++)
	{
		zp = &zones [ii];
		if (zp->east_lng > east_lng)
		{
			east_lng = zp->east_lng;
			east_idx = ii;
		}
	}
	if (east_idx >= 0)
	{
		zones [east_idx].east_flg = TRUE;
	}

	return (zone_cnt);
}

int EXP_LVL9 CSzone (	struct cs_Zone_ *zp,
						double west,
						double cent,
						double east)
{
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Degree;		/* PI / 180.0 */
	extern double cs_Two_pi;		/* 2 PI */

	int flag;

	zp->ns_flag  = 0;
	zp->east_flg = FALSE;
	zp->west_xx  = cs_Zero;
	zp->x_off    = cs_Zero;
	zp->east_xx  = cs_Zero;

	if (west == cs_Zero && east == cs_Zero)			/*lint !e777 */
	{
		zp->west_lng = cs_Zero;
		zp->cent_lng = cs_Zero;
		zp->east_lng = cs_Zero;
		zp->width    = cs_Zero;
		return (0);
	}

	/* Extract the flag word from the west
	   argument. */

	flag = ((int)fabs (west) / 1000) % 4;
	west = fmod (west,1000.0);
	switch (flag) {
	case 1:
		zp->ns_flag = 1;
		break;
	case 2:
		zp->ns_flag = -1;
		break;
	default:
	case 0:
	case 3:
		zp->ns_flag = 0;
		break;
	}
	zp->west_lng = CS_adj2pi (west * cs_Degree);
	zp->cent_lng = CS_adj2pi (cent * cs_Degree);
	zp->east_lng = CS_adj2pi (east * cs_Degree);
	zp->width = zp->east_lng - zp->west_lng;
	if (zp->width < cs_Zero) zp->width += cs_Two_pi;
	return (1);
}

Const struct cs_Zone_ * EXP_LVL9 CS_znlocF (	Const struct cs_Zone_ zones [8],
												int zone_cnt,
												double lng,
												double lat)
{
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Two_pi;		/* 2PI */

	int ii;
    const struct cs_Zone_ *zp;

	double del_lng;

	for (ii = 0,zp = zones;ii < zone_cnt;ii++,zp++)
	{
		/* The hemisphere check is pretty quick,
		   lets to that first before the more
		   intensive longitude test. */

		if ((lat >= cs_Zero && zp->ns_flag < 0) ||
			(lat <  cs_Zero && zp->ns_flag > 0)
		   )
		{
			continue;
		}

		/* The zone longitude boundaries are stored in
		   normalized longitudes in radians, -PI < lng <= PI.
		   Thus, to be able to straddle -180 properly, we use
		   the following.  Notice, by convention, we include
		   the western extreme in the zone, but not the
		   eastern extreme unless the zone is the easternmost
		   zone. */

		del_lng = (lng - zp->west_lng);
		if (del_lng < cs_Zero) del_lng += cs_Two_pi;

		if (del_lng < zp->width ||
			(del_lng <= zp->width && zp->east_flg))
		{
			return (zp);
		}
	}
	return (NULL);
}

Const struct cs_Zone_ * EXP_LVL9 CS_znlocI (	Const struct cs_Zone_ zones [8],
												int zone_cnt,
												double xx,
												double yy)
{
	extern double cs_Zero;		/* 0.0 */

	int ii;
    Const struct cs_Zone_ *zp;

	for (ii = 0,zp = zones;ii < zone_cnt;ii++,zp++)
	{
		if ((yy >= cs_Zero && zp->ns_flag < 0) ||
			(yy <  cs_Zero && zp->ns_flag > 0)
		   )
		{
			continue;
		}
		if (xx >= zp->west_xx &&
			(xx < zp->east_xx || (zp->east_flg && xx == zp->east_xx))	/*lint !e777 */		
		   )
		{
			return (zp);
		}
	}
	return (NULL);
}
