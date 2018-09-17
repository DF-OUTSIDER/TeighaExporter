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

// Now comes May 21, 2014 
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"
#	include <math.h>

bool EXP_LVL3 CSgetParamNm (char* paramName,size_t paramSize,EcsNameFlavor nmFlavor,int paramCode);
bool EXP_LVL3 CSAddParamValue (char* wktBufr,size_t bufrSize,EcsNameFlavor nmFlavor,int paramCode,double paramValue,unsigned paramFlags);
#define cs_PARAMFLG_UTM 1

extern "C" double cs_Zero;
extern "C" double cs_One;
extern "C" double cs_K90;
extern "C" double cs_Degree;
extern "C" char csErrnam [];
extern "C" struct cs_Prjtab_ cs_Prjtab [];
extern "C" struct cs_Prjprm_ csPrjprm [];

int EXP_LVL1 CS_cs2WktEx (char *bufr,size_t bufrSize,const char *csKeyName,int flavor,unsigned short flags)
{
	int rtnValue = -1;
	struct cs_Dtdef_ *dtDefPtr = 0;
	struct cs_Eldef_ *elDefPtr = 0;
	struct cs_Csdef_ *csDefPtr = 0;
	
	if (bufrSize > 0)
	{
		*bufr = '\0';
		csDefPtr = CS_csdef (csKeyName);
		if (csDefPtr != 0)
		{
			if (csDefPtr->dat_knm [0] == '\0')
			{
				CS_erpt (cs_WKT_NODTREF);
			}
			else
			{
				dtDefPtr = CS_dtdef (csDefPtr->dat_knm);
				if (dtDefPtr != 0)
				{
					elDefPtr = CS_eldef (dtDefPtr->ell_knm);
					if (elDefPtr != 0)
					{
						rtnValue = CScs2WktEx (bufr,bufrSize,(ErcWktFlavor)flavor,csDefPtr,dtDefPtr,elDefPtr,flags);
						CS_free (elDefPtr);
						elDefPtr = 0;
					}
					CS_free (dtDefPtr);
					dtDefPtr = 0;
				}
			}
			CS_free (csDefPtr);
			csDefPtr = 0;
		}
	}
	return rtnValue;
}

int EXP_LVL1 CS_cs2Wkt (char *bufr,size_t bufrSize,const char *csKeyName,int flavor)
{
	int rtnValue = -1;
	struct cs_Dtdef_ *dtDefPtr = 0;
	struct cs_Eldef_ *elDefPtr = 0;
	struct cs_Csdef_ *csDefPtr = 0;
	
	if (bufrSize > 0)
	{
		*bufr = '\0';
		csDefPtr = CS_csdef (csKeyName);
		if (csDefPtr != 0)
		{
			dtDefPtr = CS_dtdef (csDefPtr->dat_knm);
			if (dtDefPtr != 0)
			{
				elDefPtr = CS_eldef (dtDefPtr->ell_knm);
			}
			else
			{
				elDefPtr = CS_eldef (csDefPtr->elp_knm);
			}

			if (elDefPtr != 0)
			{
				rtnValue = CScs2Wkt (bufr,bufrSize,(ErcWktFlavor)flavor,csDefPtr,dtDefPtr,elDefPtr);
				CS_free (elDefPtr);
				elDefPtr = 0;
			}
			if (dtDefPtr != 0)
			{
				CS_free (dtDefPtr);
				dtDefPtr = 0;
			}
			CS_free (csDefPtr);
			csDefPtr = 0;
		}
	}
	return rtnValue;		
}

int EXP_LVL3 CScs2Wkt (char *csWktBufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Csdef_ *cs_def,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def)
{
	int st;
	unsigned short flags;

	flags = cs_WKTFLG_MAPNAMES;
	st = CScs2WktEx (csWktBufr,bufrSize,flavor,cs_def,dt_def,el_def,flags);
	return st;
}
int EXP_LVL3 CScs2WktEx (char *csWktBufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Csdef_ *cs_def,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def,unsigned short flags)
{
	int status;
	int rtnValue;
	unsigned paramFlags;

	EcsMapSt csMapSt;
	EcsNameFlavor nmFlavor;

	double e_sq;
	double e_rad;
	double tmpDbl;
	double primeMer;
	double stdParallel;
	double unitFactor;
	double epsgParm;

	char *ccPtr;
	const char *kCp;
	struct cs_Dtdef_ *dtDefPtr;
	struct cs_Eldef_ *elDefPtr;
	struct cs_Prjtab_ *prjPtr;

	char refDtmName [64];
	char baseName [64];
	char cTemp [128];

	char elpWkt[512];
	char datmWkt [512];
	char pmerWkt [512];
	char geogWkt [512];
	char parmWkt [512];
	char unitWkt [128];
	char projWkt [1024];
	char geoAxis [1024];
	char prjAxis [1024];
	char geoTranWkt [1024];
	char projection [64];
	char csysWktName [96];
	char gcsName [256];
	char tmpBufr [1024];

	status = 0;
	rtnValue = 0;
	refDtmName [0] = '\0';
	elDefPtr = 0;
	dtDefPtr = 0;

	if (cs_def == 0)
	{
		CS_stncp (csErrnam,"CS_cs2Wkt:1",MAXPATH);
		CS_erpt (cs_ISER);
		goto error;
	}

	/* Make sure we have a datum pointer. */
	if (dt_def == NULL)
	{
		/* The calling application did not provide a specific datum definition.
		   We use the datum name in the Coordinate System definition to obtain
		   a definition fomr the dictionary. */
		if (cs_def->dat_knm [0] != '\0')
		{
			/* We need to free this definition, we stash it in a new pointer variable. */
			dtDefPtr = CS_dtdef (cs_def->dat_knm);
			if (dtDefPtr == NULL)
			{
				/* If the coordinate system definition came from the dictionary,
				   this shouldn't happen.  If the application got the cs_Csdef_
				   data from somewhere else, it is possible. */
				CS_erpt (cs_NO_REFERNCE);
				goto error;
			}
			dt_def = dtDefPtr;
		}
	}

	if (el_def != 0)
	{
		if(dt_def != 0)
		{
			if (CS_stricmp (dt_def->ell_knm,el_def->key_nm))
			{
				CS_erpt (cs_WKT_INCNSIST);
				goto error;
			}
		}
	}

	/* Locate the projection in the projection table.  We need this for access
	   to the flag word for this projection. */
	for (prjPtr = cs_Prjtab;prjPtr->code != cs_PRJCOD_END;prjPtr += 1)
	{
		if (!strcmp (cs_def->prj_knm,prjPtr->key_nm)) break;
	}
	if (prjPtr->code == cs_PRJCOD_END)
	{
		CS_erpt (cs_UNKWN_PROJ);
		goto error;
	}

	/* Determine the flavor we will use to generate the WKT.  We will use the
	   user specified flavor if it is likely to produce success.  Otherwise,
	   we switch to Autodesk flavor which should always produce success.  If
	   the user fails to set a specific flavor, we assume ESRI for legacy
	   consistency. */
	if (flavor == wktFlvrNone)
	{
		flavor = wktFlvrEsri;
	}

	/* Success of WKT generation is largely a function of the flavor and the
	   projection.  Some projections don't convert to WKT at all. */
	if (prjPtr->code == cs_PRJCOD_TRMERAF ||
	    prjPtr->code == cs_PRJCOD_LMBRTAF ||
	    prjPtr->code == cs_PRJCOD_AZEDE   ||
	    prjPtr->code == cs_PRJCOD_MSTRO   ||
	    prjPtr->code == cs_PRJCOD_BPCNC   ||
	    prjPtr->code == cs_PRJCOD_HOM1UV  ||
	    prjPtr->code == cs_PRJCOD_HOM2UV)
	{
		CS_stncp (csErrnam,prjPtr->key_nm,MAXPATH);
		CS_erpt (cs_WKT_PRJSUPRT);
		goto error;
	}

	/* Some CS-MAP projections don't work with any flavor of WKT other than
	   Autodesk. */
	if (prjPtr->code == cs_PRJCOD_SSTRO     ||
		prjPtr->code == cs_PRJCOD_KRVK95    ||
		prjPtr->code == cs_PRJCOD_MODPC)
	{
		/* If we are not allowed to switch flavors, we report an error for
		   these projections. */
		if ((flags & cs_WKTFLG_ALLWFLVRSW) == 0)
		{
			CS_stncp (csErrnam,prjPtr->key_nm,MAXPATH);
			CS_erpt (cs_WKT_PRJSUPRT);
			goto error;
		}

		/* Otherwise, we simply switch the flavor to Autodesk, which can
		   handle just about any projection. */
		flavor = wktFlvrAutodesk;

		/* Since we have changed the flavor, we return a +1 status value if
		   requested to signal a flavor switch. */
		if ((flags & cs_WKTFLG_SGNFLVRSW) != 0)
		{
			rtnValue = 1;
		}
	}
	
	/* Certain flavors do not support certain projections which are otherwise
	   commonly supported. */
	if (flavor == wktFlvrEsri)
	{
		/* ESRI uses an approximation for the SWISS projection.  As best
		   I can determine.   CS-MAP uses the proper projection. So, we could
		   force an Autodesk flavor in this case.  However, since the WKT
		   reader makes the conversion back to the correct projection, we
		   allow this these through and kludge up the code below to make the
		   switch to the approximations. */
		if (prjPtr->code == cs_PRJCOD_SOTRM    ||
		    prjPtr->code == cs_PRJCOD_RSKEWC   ||
		    prjPtr->code == cs_PRJCOD_RSKEWO   ||
    		prjPtr->code == cs_PRJCOD_WCCSL    ||
			prjPtr->code == cs_PRJCOD_WCCST    ||
			prjPtr->code == cs_PRJCOD_MNDOTL   ||
			prjPtr->code == cs_PRJCOD_MNDOTT   ||
		    prjPtr->code == cs_PRJCOD_OBQCYL   ||
		    prjPtr->code == cs_PRJCOD_SYS34    ||
			prjPtr->code == cs_PRJCOD_SYS34_99 ||
			prjPtr->code == cs_PRJCOD_OSTN97   ||
			prjPtr->code == cs_PRJCOD_OSTN02   ||
		    prjPtr->code == cs_PRJCOD_TRMRS    ||
		    prjPtr->code == cs_PRJCOD_TRMRKRG
		   )
		{
			/* If we are not allowed to switch flavors, we report an error for
			   these projections. */
			if ((flags & cs_WKTFLG_ALLWFLVRSW) == 0)
			{
				CS_stncp (csErrnam,prjPtr->key_nm,MAXPATH);
				CS_erpt (cs_WKT_PRJSUPRT);
				goto error;
			}

			/* Switch the flavor to Autodesk. */
			flavor = wktFlvrAutodesk;
			rtnValue = 1;

			/* Since we have changed the flavor, we return a +1 status value if
			   requested to signal a flavor switch. */
			if ((flags & cs_WKTFLG_SGNFLVRSW) != 0)
			{
				rtnValue = 1;
			}
		}
	}
	if (flavor == wktFlvrOracle)
	{
		if (prjPtr->code == cs_PRJCOD_SOTRM  ||
			prjPtr->code == cs_PRJCOD_RSKEWC ||
			prjPtr->code == cs_PRJCOD_RSKEWO ||
			prjPtr->code == cs_PRJCOD_WCCSL  ||
			prjPtr->code == cs_PRJCOD_WCCST  ||
			prjPtr->code == cs_PRJCOD_MNDOTL ||
			prjPtr->code == cs_PRJCOD_MNDOTT ||
			prjPtr->code == cs_PRJCOD_OSTN97 ||
			prjPtr->code == cs_PRJCOD_OSTN02 ||
			prjPtr->code == cs_PRJCOD_OBQCYL)
		{
			/* If we are not allowed to switch flavors, we report an error for
			   these projections. */
			if ((flags & cs_WKTFLG_ALLWFLVRSW) == 0)
			{
				CS_stncp (csErrnam,prjPtr->key_nm,MAXPATH);
				CS_erpt (cs_WKT_PRJSUPRT);
				goto error;
			}

			/* Switch the flavor to Autodesk. */
			flavor = wktFlvrAutodesk;
			rtnValue = 1;

			/* Since we have changed the flavor, we return a +1 status value if
			   requested to signal a flavor switch. */
			if ((flags & cs_WKTFLG_SGNFLVRSW) != 0)
			{
				rtnValue = 1;
			}
		}
	}
	nmFlavor = csWktFlvrToCsMapFlvr (flavor);
	/* We have selected a flavor that should produce success.  We have done
	   done this prior to all other processing so that the flavor of the
	   resulting WKT string is consistent throughout. */

	/* Determine the possibly flavored name of the projection.  If the projection
	   is the Unity projection, i.e. a GEOGCS, this will not be used.  We populate
	   the projection character array anyway just to be defensive.  Rather have a
	   strange projection name rather than a crash. */
	CS_stncp (projection,"LL",sizeof (projection));
	if (prjPtr->code != cs_PRJCOD_UNITY)
	{
		csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),nmFlavor,csMapFlvrCsMap,prjPtr->key_nm);
		if (csMapSt != csMapOk && prjPtr->code == cs_PRJCOD_LMTAN)
		{
			/* The LMTAN and LMSP1 projections are essentially the same thing.
			   Since it appears that the requested flavor does not support a
			   name for this projection, we switch to whatever name the flavor
			   uses for the LM1SP.  The parameters for these two projections
			   are the same, */
			csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),nmFlavor,csMapFlvrCsMap,"LM1SP");
		}
		if (csMapSt != csMapOk)
		{
			/* If none of that worked, try to get the Autodesk name that appears
			   in the mapping table. */
			csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),csMapFlvrAutodesk,csMapFlvrCsMap,prjPtr->key_nm);
		}
		if (csMapSt != csMapOk)
		{
			/* If that didn't work, we bag it. */
			CS_stncp (csErrnam,prjPtr->key_nm,MAXPATH);
			CS_erpt (cs_WKT_PRJSUPRT);
			goto error;
		}
	}
	/* Projection name is handled. */

	/* Locate the datum definition, if there is one. */
	if (dt_def == NULL)
	{
		/* The calling application did not provide a specific datum definition.
		   We use the datum name in the Coordinate System definition to obtain
		   a definition from the dictionary. */
		if (cs_def->dat_knm [0] == '\0')
		{
			/* Oops!!! No datum definition in the coordinate system definition.
			   We need to output a cartographically referenced definition.  While
			   there is little in the way of a standard, a DATUM element with a
			   blank name and a SPHEROID element appears to be the accepted way of
			   doing this.

			   If the user has supplied an ellipsoid definition, we use it to
			   produce the SPHEROID element of the cartographically referenced
			   DATUM element.  If none was provided, which is legitimate, we
			   use the ellipsoid key name provided in the coordinate system
			   definition.  If that does not exist (should never happen),
			   we simply use WGS84. */
			if (el_def != 0)
			{
				status = CSel2WktEx (elpWkt,sizeof (elpWkt),flavor,el_def,flags);
			}
			else
			{
				kCp = cs_def->elp_knm;
				if (*kCp == '\0')
				{
					/* This should never happen; but we handle it anyway. */
					kCp = "WGS84";
				}
				elDefPtr = CS_eldef (kCp);
				if (elDefPtr == NULL)
				{
					/* Couldn't get an ellipsoid to reference nohow!!! */
					CS_erpt (cs_NO_REFERNCE);
					goto error;
				}
				status = CSel2WktEx (elpWkt,sizeof (elpWkt),flavor,elDefPtr,flags);
				CS_free (elDefPtr);
				elDefPtr = 0;
			}
			if(status == 0)
			{
				sprintf (datmWkt,"DATUM[\"\",%s]",elpWkt);
			}
			else
			{
				CS_erpt (cs_NO_REFERNCE);
				goto error;
			}

			/* Since there is no datum reference, reset refDtmName to null. */
			refDtmName [0] = '\0';
		}
		else
		{
			/* No datum pointer provided, but we do have a datum name in the
			   coordinate system defintiion.  We use that name to get a
			   definition.  Capture the name of referenced datum which will be
			   used to locate a base GEOGCS definition for a PROJCS definition. */
			CS_stncp (refDtmName,cs_def->dat_knm,sizeof (refDtmName));
			dtDefPtr = CS_dtdef (cs_def->dat_knm);
			if (dtDefPtr == NULL)
			{
				/* If the coordinate system definition came from the dictionary,
				   this shouldn't happen.  If the application got the cs_Csdef_
				   data from somewhere else, it is possible. */
				CS_erpt (cs_NO_REFERNCE);
				goto error;
			}

			/* I don't have to locate the ellipsoid, CSdt2WktEx will do that. */
			status = CSdt2WktEx (datmWkt,sizeof (datmWkt),geoTranWkt,sizeof (geoTranWkt),flavor,dtDefPtr,0,flags);
			CS_free (dtDefPtr);
			dtDefPtr = 0;
		}
	}
	else
	{
		/* If the user actually supplied a datum definition, and a specific
		   flavor has been specified, the assumption is that it came from a WKT
		   string, and therefore the real WKT name of the definition will be in
		   the 'name' member of the cs_Dtdef_ structure.  We use this name as
		   it will tend to reproduce the original WKT.  Not sure this is
		   necessary any longer; but only has an effect when the user supplies
		   the datum definition pointer. */
		if ((flags & cs_WKTFLG_MAPNAMES) == 0
			&& (flavor == wktFlvrEsri || flavor == wktFlvrOracle))
		{
			CS_stncp (refDtmName,dt_def->name,sizeof (refDtmName));
		}
		else
		{
			CS_stncp (refDtmName,dt_def->key_nm,sizeof (refDtmName));
		}
		status = CSdt2WktEx (datmWkt,sizeof (datmWkt),geoTranWkt,sizeof (geoTranWkt),flavor,dt_def,el_def,flags);
	}
	if (status != 0)
	{
		/* CSdt2WktEx will have reported the nature of the error. */
		goto error;
	}
	/* OK!!! We have the WKT form of the DATUM element. */

	/* Deal with the quad/axis situation.  If the quad member of the coordinate
	   system definition is 0 or 1, the axes are the normal set and we don't
	   bother with all this stuff.  This is the case 99.999% of the time. */
	geoAxis [0] = '\0';
	prjAxis [0] = '\0';
	if (cs_def->quad != 0 && cs_def->quad != 1)
	{
		if ((prjPtr->flags & cs_PRJFLG_GEOGR) != 0)
		{
			/* This is a grographic definition, we need a geographic axis specifiction. */
			switch (cs_def->quad) {
			default:
			case 0:
			case 1:
				CS_stncp (geoAxis,",AXIS[\"Lon\",EAST],AXIS[\"Lat\",NORTH]",sizeof (prjAxis));
				break;
			case 2:
				CS_stncp (geoAxis,",AXIS[\"Lon\",WEST],AXIS[\"Lat\",NORTH]",sizeof (prjAxis));
				break;
			case 3:
				CS_stncp (geoAxis,",AXIS[\"Lon\",WEST],AXIS[\"Lat\",SOUTH]",sizeof (prjAxis));
				break;
			case 4:
				CS_stncp (geoAxis,",AXIS[\"Lon\",EAST],AXIS[\"Lat\",SOUTH]",sizeof (prjAxis));
				break;
			case -1:
				CS_stncp (geoAxis,",AXIS[\"Lat\",NORTH],AXIS[\"Lon\",EAST]",sizeof (prjAxis));
				break;
			case -2:
				CS_stncp (geoAxis,",AXIS[\"Lat\",NORTH],AXIS[\"Lon\",WEST]",sizeof (prjAxis));
				break;
			case -3:
				CS_stncp (geoAxis,",AXIS[\"Lat\",SOUTH],AXIS[\"Lon\",WEST]",sizeof (prjAxis));
				break;
			case -4:
				CS_stncp (geoAxis,",AXIS[\"Lat\",SOUTH],AXIS[\"Lon\",EAST]",sizeof (prjAxis));
				break;
			}
		}
		else
		{
			/* A projective system definition. */
			switch (cs_def->quad) {
			default:
			case 0:
			case 1:
				CS_stncp (prjAxis,",AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]",sizeof (prjAxis));
				break;
			case 2:
				CS_stncp (prjAxis,",AXIS[\"X\",WEST],AXIS[\"Y\",NORTH]",sizeof (prjAxis));
				break;
			case 3:
				CS_stncp (prjAxis,",AXIS[\"X\",WEST],AXIS[\"Y\",SOUTH]",sizeof (prjAxis));
				break;
			case 4:
				CS_stncp (prjAxis,",AXIS[\"X\",EAST],AXIS[\"Y\",SOUTH]",sizeof (prjAxis));
				break;
			case -1:
				CS_stncp (prjAxis,",AXIS[\"Y\",NORTH],AXIS[\"X\",EAST]",sizeof (prjAxis));
				break;
			case -2:
				CS_stncp (prjAxis,",AXIS[\"Y\",NORTH],AXIS[\"X\",WEST]",sizeof (prjAxis));
				break;
			case -3:
				CS_stncp (prjAxis,",AXIS[\"Y\",SOUTH],AXIS[\"X\",WEST]",sizeof (prjAxis));
				break;
			case -4:
				CS_stncp (prjAxis,",AXIS[\"Y\",SOUTH],AXIS[\"X\",EAST]",sizeof (prjAxis));
				break;
			}
		}
	}

	/* Locate the base.  The base is what this coordinate system is referenced
	   to.  For example, the definition of UTM27-13 defines how to get from
	   LL27 to UTM27-13.  Therefore, LL27 is the base for that coordinate
	   system.  We hereby establish that 'basename' is a CS-MAP dictionary
	   name and shall remain unflavored.  This is to prevent the flavoring from
	   being applied more than once. */
	if ((prjPtr->flags & cs_PRJFLG_GEOGR) != 0)
	{
		CS_stncp (baseName,cs_def->key_nm,sizeof (baseName));
	}
	else
	{
		/* Otherwise, basename is the name of a geographic coordinate system
		   referenced to the same datum as the projected coordinate system.
		   We have a function which will find this for us.  If we don't
		   find one, we we need to do some special stuff.

		   Note that this name is not flavored. */
		CSllCsFromDt (baseName,sizeof baseName,refDtmName);		//lint !e534   ignoring return value

		if (baseName[0] == '\0')
		{
			/* Here if a base name could not be located.  This is actually
			   the normal situation in the case of a cartographically
			   referenced coordinate system.  In this case, baseName will
			   actually contain the null string.  Thus, the GEOGCS
			   will contain a null name.  This appears to be a
			   legitimate thing to do in this case. */
			CS_stncp (baseName,cs_def->dat_knm,sizeof (baseName));
		}
	}

	/* We will always need the GEOGCS object.  If this is a geographic
	   coordinate system definition, than that's all we'll need.  If this is
	   a Projective coordinate system, the unit is the default for internal
	   coordinates within CS-MAP, namely the degree.  Only if this is a 
	   Geographic Coordinate system, is the actual unit of the definition
	   of importance to us here.
	   
	   First we prepare a default if any of the stuff following it fails. */
	csMapSt = csMapNameToNameC (csMapAngularUnitKeyName,cTemp,sizeof (cTemp),
														nmFlavor,
														csMapFlvrAutodesk,
														"DEGREE");
	if (csMapSt != csMapOk)
	{
		CS_stncp (cTemp,"Degree",sizeof (cTemp));
	}
	unitFactor = cs_One;

	/* With the default determined, we proceed to genberate the more
	   specific, if possible. */
	if ((prjPtr->flags & cs_PRJFLG_GEOGR) == 0)
	{
		// This little bit of redundant nonsense is retained so as to prevent
		// a regression tester from producing thousands of regression failures.
		// TODO: A refactoring of all WKT code to remove nonsense like this is
		// on the proposed schedule; we'll remove it at that time.
		sprintf (unitWkt,"UNIT[\"%s\",0.017453292519943295]",cTemp);
	}
	else
	{
		/* This is a geographic coordinate system, extract the unit information
		   from the definition. */
		unitFactor = CS_unitlu (cs_UTYP_ANG,cs_def->unit);
		csMapSt = csMapNameToNameC (csMapAngularUnitKeyName,cTemp,sizeof (cTemp),
															nmFlavor,
															csMapFlvrAutodesk,
															cs_def->unit);
		if (csMapSt != csMapOk)
		{
			CS_stncp (cTemp,cs_def->unit,sizeof (cTemp));
		}
		sprintf (unitWkt,"UNIT[\"%s\",%.14f]",cTemp,unitFactor * cs_Degree);
	}

	/* Manufacture the prime meridian.  Produce the default value first in
	   case anything goes wrong.  In CS-MAP, all projective parameters are in
	   degrees referenced to greenwich.  Thus, the only time we need a
	   PRIMEM element other than the greenwich default is for a GEOGCS
	   definition. */
	CS_stncp (pmerWkt,"PRIMEM[\"Greenwich\",0]",sizeof (pmerWkt));
	if (((prjPtr->flags & cs_PRJFLG_GEOGR) != 0 || prjPtr->code == cs_PRJCOD_KROVAK) && cs_def->org_lng != 0.0)
	{
		int orgLng = (int)cs_def->org_lng;
		kCp = 0;				// redundant, but it keeps lint happy.
		primeMer = 0.0;
		switch (orgLng) {
		case 0:
			primeMer = 0.0;
			kCp = "Greenwich";
			break;
		case 2:
			primeMer = 2.337229166666667;
			kCp = "Paris";
			break;
		case 4:
			primeMer = 4.367975;
			kCp = "Brussels";
			break;
		case 7:
			primeMer = 7.439583333333333;
			kCp = "Bern";
			break;
		case 9:
			primeMer = 9.13190611111111;
			kCp = "Lisbon";
			break;
		case 10:
			primeMer = 10.72291666666667;
			kCp = "Oslo";
			break;
		case 12:
			primeMer = 12.45233333333333;
			kCp = "Rome";
			break;
		case 18:
			primeMer = 18.05082777777778;
			kCp = "Stockholm";
			break;
		case 106:
			primeMer = 106.8077194444444;
			kCp = "Jakarta";
			break;
		case -3:
			primeMer = -3.68793888888889;
			kCp = "Madrid";
			break;		
		case -17:
			if (fabs (cs_def->org_lng - (-17.6665931666667)) < 1.0E-05)
			{
				primeMer = -17.666593166666667;
				kCp = "FerroPrecise";
			}
			else
			{
				primeMer = -17.666666666666667;
				kCp = "Ferro";
			}
			break;
		case -74:
			primeMer = -74.08175;
			kCp = "Bogota";
			break;
		default:
			kCp = 0;
			break;
		}
		if (kCp != 0)
		{
			if (flavor == wktFlvrOracle || flavor == wktFlvrOracle9 || flavor == wktFlvrGeoTools)
			{
				primeMer /= unitFactor;
			}
			sprintf (pmerWkt,"PRIMEM[\"%s\",%18.13f]",kCp,primeMer);
		}
	}

	/* If we are mapping names, the GCS name and the Datum name need to be
	   mapped, and then configured for the active flavor.  First, we do the
	   default in case anything goes wrong in the name mapping process.
	   Note, this is where the null baseName is introduced to the GEOGCS
	   in the event of a cartographically referenced coordinate system. */
	sprintf (geogWkt,"GEOGCS[\"%s\",%s,%s,%s%s]",baseName,datmWkt,pmerWkt,unitWkt,geoAxis);

	if ((flags & cs_WKTFLG_MAPNAMES) != 0)
	{
		/* Here to generate a flavored GEOGCS.  This is the ONE place for
		   mapping geographic system names and datum names.  Note how baseName
		   is used here; it must be an unflavored CS-MAP dictionary name.
		   
		   We have already generated a suitable default to use in case any of this
		   goes awry. */
		csMapSt = csMapNameToNameC (csMapGeographicCSysKeyName,gcsName,sizeof (gcsName),
																	   nmFlavor,
																	   csMapFlvrAutodesk,
																	   baseName);
		if (csMapSt != csMapOk)
		{
			/* There is no flavored name for this geogrpahic definition in the
			   name mapper.  There are some conventions we can use to generate
			   appropriate flavored names from the datum name.  These apply
			   only if we can obtain a flavored datum name. */
			if (refDtmName [0] != '\0')
			{
				csMapSt = csMapNameToNameC (csMapDatumKeyName,tmpBufr,sizeof (tmpBufr),
																	  nmFlavor,
																	  csMapFlvrAutodesk,
																	  refDtmName);
				if (csMapSt == csMapOk)
				{
					if (flavor == wktFlvrEsri)
					{
						ccPtr = tmpBufr;
						if (*ccPtr == 'D' && *(ccPtr + 1) == '_')
						{
							ccPtr += 2;
						}
						CS_stncp (gcsName,"GCS_",sizeof (gcsName));
						CS_stncp (&gcsName [4],ccPtr,sizeof (gcsName) - 4);
					}
					else if (flavor == wktFlvrOracle9)
					{
						sprintf (gcsName,"Longitude / Latitude (%s)",tmpBufr);
					}
					else if (flavor == wktFlvrOracle)
					{
						ccPtr = strstr (tmpBufr," (EPSG ID ");
						if (ccPtr != 0)
						{
							*ccPtr = '\0';
						}
						CS_stncp (gcsName,tmpBufr,sizeof (gcsName));
					}
					else
					{
						CS_stncp (gcsName,tmpBufr,sizeof (gcsName));
					}
				}
			}
		}
		if (csMapSt == csMapOk)
		{
			/* If after all that, it appears that the name flavoring worked,
			   we reproduce the GEOGCS definition string. */
			sprintf (geogWkt,"GEOGCS[\"%s\",%s,%s,%s%s]",gcsName,datmWkt,pmerWkt,unitWkt,geoAxis);
		}
	}

	/* Preparation complete. */
	if ((prjPtr->flags & cs_PRJFLG_GEOGR) != 0)
	{
		/* If this is a geographic coordinate system, we're done. */
		CS_stncp (csWktBufr,geogWkt,(int)bufrSize);
	}
	else
	{
		/* We get here only if the coordinate system is of the projected type. */

		/* Do the linear unit for the projWkt. */
		unitFactor = CS_unitlu (cs_UTYP_LEN,cs_def->unit);
		csMapSt = csMapNameToNameC (csMapLinearUnitKeyName,cTemp,sizeof (cTemp),
														   nmFlavor,
														   csMapFlvrAutodesk,
														   cs_def->unit);
		if (csMapSt != csMapOk)
		{
			if (csMapSt == csMapNoName)
			{
				/* Use the CS-MAP name if there is no flavored name in the
				   name mapper. */
				CS_stncp (cTemp,cs_def->unit,sizeof (cTemp));
			}
			else
			{
				/* This suggests a real problem in the name mapper.  That is,
				   the name mapper did not contain an entry for the CS-MAP
				   unit. */
				CS_stncp (csErrnam,cs_def->unit,MAXPATH);
				CS_erpt (cs_WKT_UNITMAP);
				return -1;
			}
		}
		sprintf (unitWkt,"UNIT[\"%s\",%.14f]",cTemp,unitFactor);

		/* Build the parameter portion of the WKT PROJCS element.
		   CSAddParamValue is a bit of a kludge, but it enables all the
		   flavored projection parameter name code to reside in one place.  It also enables
		   a consistent means of producing the appropriate precision for all projection
		   parameters. */ 

		parmWkt [0] = '\0';
		paramFlags = 0;
		if (prjPtr->code == cs_PRJCOD_UTM)
		{
				paramFlags |= cs_PARAMFLG_UTM;
		}

		switch (prjPtr->code) {
		case  cs_PRJCOD_UNITY:
			/* This is case should never happen.  We leave this is for legacy
			   purposes.  It was added by the MapGuide folk in 2006. */
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_TRMER:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_ALBER:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_MRCAT:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_MRCATPV:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_AZMED:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			if (cs_def->prj_prm1 != 0.0)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_YAXISAZ,cs_def->prj_prm1,paramFlags);
			}
			break;
		case  cs_PRJCOD_PLYCN:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;		
		case  cs_PRJCOD_MODPC:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_ESTDMER,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NPARALL,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SPARALL,cs_def->prj_prm4,paramFlags);
			break;
		case  cs_PRJCOD_AZMEA:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			if (cs_def->prj_prm1 != 0.0)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_YAXISAZ,cs_def->prj_prm1,paramFlags);
			}
			break;
		case  cs_PRJCOD_EDCNC:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_MILLR:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_MSTRO:
			// TODO
			break;
		case  cs_PRJCOD_NZLND:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_SINUS:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_ORTHO:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_GNOMC:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_EDCYL:
		case  cs_PRJCOD_EDCYLE:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_VDGRN:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_CSINI:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_ROBIN:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_BONNE:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_EKRT4:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_EKRT6:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_MOLWD:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_HMLSN:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			break;
		case  cs_PRJCOD_NACYL:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDPLL,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_TACYL:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			break;
		case  cs_PRJCOD_BPCNC:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_P1LNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_P1LAT,cs_def->prj_prm2,paramFlags);
			if (cs_def->prj_prm5 <= cs_Zero)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_P2LNG,cs_def->prj_prm3,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_P2LAT,cs_def->prj_prm4,paramFlags);
			}
			else
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_ADP1P2,cs_def->prj_prm5,paramFlags);
			}
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_ADSP1,cs_def->prj_prm6,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_ADSP2,cs_def->prj_prm7,paramFlags);
			break;
		case  cs_PRJCOD_PSTRO:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			break;
		case  cs_PRJCOD_PSTROSL:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDCIR,cs_def->prj_prm1,paramFlags);
			
			/* Kludge time again.  If the flavor is ESRI, we change the projection name based
			   on the origin latitude.  Sorry about that, but this WKT stuff is maddening. */
			if (flavor == wktFlvrEsri)
			{
				if (cs_def->org_lat < -89.0)
				{
					CS_stncp (projection,"Stereographic_South_Pole",sizeof (projection));
				}
				else if (cs_def->org_lat < -89.0)
				{
					CS_stncp (projection,"Stereographic_North_Pole",sizeof (projection));
				}
			}
			break;
		case  cs_PRJCOD_OSTRO:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			break;
		case  cs_PRJCOD_SSTRO:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			break;
		case  cs_PRJCOD_LM1SP:
		case  cs_PRJCOD_LMTAN:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_LM2SP:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_LMBLG: 
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			break;
		case  cs_PRJCOD_WCCSL:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GHGT,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AELEV,cs_def->prj_prm4,paramFlags);
			break;
		case  cs_PRJCOD_WCCST: 
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GHGT,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AELEV,cs_def->prj_prm3,paramFlags);
			break;
		case  cs_PRJCOD_MNDOTL: 
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AELEV,cs_def->prj_prm3,paramFlags);
			break;
		case  cs_PRJCOD_MNDOTT: 
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AELEV,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_SOTRM:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			break;
		case  cs_PRJCOD_UTM:
			if (flavor == wktFlvrAutodesk)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_UTMZN,cs_def->prj_prm1,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_HSNS,cs_def->prj_prm2,paramFlags);
			}
			else
			{
				/* Switch the projection to Transverse Mercator */
				csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),nmFlavor,csMapFlvrCsMap,"TM");
				if (csMapSt != csMapOk)
				{
					/* If that didn't work, try to get the Autodesk name that
					   appears in the mapping table. */
					csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),csMapFlvrAutodesk,csMapFlvrCsMap,"TM");
					if (csMapSt != csMapOk)
					{
						/* If that didn't work, use a default that is rational. */	
						CS_stncp (projection,"Transverse_Mercator",sizeof (projection));
					}
				}

				/* Generate the standard UTM zone parameters from the zone and
				   hemisphere which are in the definition. */
				tmpDbl = 500000.00 / unitFactor;
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,tmpDbl,paramFlags);
				tmpDbl = (cs_def->prj_prm2 >= 0.0) ? 0.0 : 10000000.0;
				tmpDbl /= unitFactor;
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,tmpDbl,paramFlags);
				tmpDbl = (double)(-183 + (6 * (int)cs_def->prj_prm1)); 				/*lint !e790 */
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,tmpDbl,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,0.9996,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_Zero,paramFlags);
			}
			break;
		case  cs_PRJCOD_TRMRS:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_KROVAK:
			if (flavor == wktFlvrAutodesk)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_POLELNG,cs_def->prj_prm1,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_POLELAT,cs_def->prj_prm2,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_OSTDPLL,cs_def->prj_prm3,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			}
			else
			{
				/* Convert the parameter from CS-MAP form to EPSG form. */
				e_sq = el_def->ecent * el_def->ecent;
				e_rad = el_def->e_rad * cs_def->scl_red;
				epsgParm = CSkrovkEpsgParam (e_rad,e_sq,cs_def->org_lng,cs_def->org_lat,cs_def->prj_prm2);

				/* Produce the parameter string. */				
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLNG,cs_def->prj_prm1,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLAT,cs_def->org_lat,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCAZM,epsgParm,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_OSTDPLL,cs_def->prj_prm3,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			}
			break;
		case  cs_PRJCOD_MRCATK:
			/* There are flavors of WKT that do not support this variation of the Mercator projection.
			   In this case, we need to convert the scale reduction factor to a standard parallel
			   value and output a standard Merctor.  Painful, but true. */
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			if (flavor == wktFlvrEsri || flavor == wktFlvrOracle || flavor == wktFlvrOracle9 || flavor == wktFlvrGeoTiff)
			{
				/* Compute the value of the standard parallel which produces the same
				   effect of the scale factor provided for this definition.  We output
				   that as the standard parallel.  Of course, we also have to change
				   the name of the projection back to the standard Mercator. */
				e_sq = el_def->ecent * el_def->ecent;
				stdParallel = CSmrcatPhiFromK (e_sq,cs_def->scl_red);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDPLL,stdParallel,paramFlags);

				/* Switch the projection to Transverse Mercator */
				csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),nmFlavor,csMapFlvrCsMap,"MRCAT");
				if (csMapSt != csMapOk)
				{
					/* If that didn't work, try to get the Autodesk name that
					   appears in the mapping table. */
					csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),csMapFlvrAutodesk,csMapFlvrCsMap,"MRCAT");
					if (csMapSt != csMapOk)
					{
						/* If that didn't work, use a default that is rational. */	
						CS_stncp (projection,"Mercator",sizeof (projection));
					}
				}
			}
			else
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			}
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			break;
		case  cs_PRJCOD_KRVK95:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_POLELNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_POLELAT,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_OSTDPLL,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			break;
		case  cs_PRJCOD_TRMERAF:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA0,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB0,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA1,cs_def->prj_prm4,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA2,cs_def->prj_prm5,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB1,cs_def->prj_prm6,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB2,cs_def->prj_prm7,paramFlags);
			break;
		case  cs_PRJCOD_OBQCYL:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NRMLPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			break;
		case  cs_PRJCOD_SYS34:
			/* Could add some code here for Oracle flavor, as they seem to support this
			   system.  They use a different projection name to distinguish between
			   the regions.  Thus, we'd probably want to invent some gimmick in the
			   mapping table to have some way of getting the Oracle projection name
			   from the name mapper.  Of course, in this case, there would be no
			   parameters. */
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_DENRGN,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_OSTN97:
			/* Noparameters required; it's all hard coded. */
			break;
		case  cs_PRJCOD_AZEDE:
 			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			/* For this variation, which is special to CS-MAP, we always output the
			   Y Axis Azimuth parameter. */
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_YAXISAZ,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AELEV,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_OSTN02:
			/* Noparameters required; it's all hard coded. */
			break;
		case  cs_PRJCOD_SYS34_99:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_DENRGN,cs_def->prj_prm1,paramFlags);
			break;
		case  cs_PRJCOD_TRMRKRG:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_WINKL:
 			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_STDPLL,cs_def->prj_prm1,paramFlags);
            break;
		case  cs_PRJCOD_LMBRTAF:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_NSTDPLL,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SSTDPLL,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA0,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB0,cs_def->prj_prm4,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA1,cs_def->prj_prm5,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFA2,cs_def->prj_prm6,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB1,cs_def->prj_prm7,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_AFB2,cs_def->prj_prm8,paramFlags);
			break;
		case  cs_PRJCOD_HOM1UV:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCAZM,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLAT,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_HOM1XY:
		case  cs_PRJCOD_RSKEW:
		case  cs_PRJCOD_RSKEWC:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCAZM,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLAT,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_RSKEWO:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_SKWAZM,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLAT,cs_def->prj_prm2,paramFlags);
			break;
		case  cs_PRJCOD_HOM2UV:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP1LNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP1LAT,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP2LNG,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP2LAT,cs_def->prj_prm4,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_HOM2XY:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_def->scl_red,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP1LNG,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP1LAT,cs_def->prj_prm2,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP2LNG,cs_def->prj_prm3,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCP2LAT,cs_def->prj_prm4,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_GAUSSK:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->prj_prm1,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		case  cs_PRJCOD_SWISS:
			if (flavor == wktFlvrAutodesk || flavor == wktFlvrOgc)
			{
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			}
			else
			{
				/* Best I can determine, all other flavors use an Oblique Mercator
				   approximation, with the azimuth value set to +90.0.  This includes
				   EPSG 6.14 which does actually include a Swiss Oblique Mercator
				   projection.  Oh well!!!  Job security for you and me.
				   
				   First we obtain the flavored name of the RSKEWC projection. */	
				csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),nmFlavor,csMapFlvrCsMap,"RSKEWC");
				if (csMapSt != csMapOk)
				{
					/* If that didn't work, try to get the Autodesk name that
					   appears in the mapping table. */
					csMapSt = csMapNameToNameC (csMapProjectionKeyName,projection,sizeof (projection),csMapFlvrAutodesk,csMapFlvrCsMap,"RSKEWC");
					if (csMapSt != csMapOk)
					{
						/* If that didn't work, use a default that is rational. */	
						CS_stncp (projection,"Rectified Skew Orthomorphic, Origin & Azimuth at Center",sizeof (projection));
					}
				}
				
				/* Output the appropriate parameters, using +90.0 as the Azimuth
				   parameter. */
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_SCLRED,cs_One,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCAZM,cs_K90,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLNG,cs_def->org_lng,paramFlags);
				CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_GCPLAT,cs_def->org_lat,paramFlags);
			}
			break;
		case  cs_PRJCOD_PCARREE:
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FEAST,cs_def->x_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_FNORTH,cs_def->y_off,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_PRMCOD_CNTMER,cs_def->org_lng,paramFlags);
			CSAddParamValue (parmWkt,sizeof (parmWkt),nmFlavor,cs_WKTCOD_ORGLAT,cs_def->org_lat,paramFlags);
			break;
		}												/*lint !e744 */

		/* Need to determine the name we will assign to this definition.  We
		   generate a default value which is used if everything else fails. */
		if ((flags & cs_WKTFLG_MAPNAMES) == 0
			&& (flavor == wktFlvrEsri || flavor == wktFlvrOracle))
		{
			CS_stncp (csysWktName,cs_def->desc_nm,sizeof (csysWktName));
		}
		else
		{
			CS_stncp (csysWktName,cs_def->key_nm,sizeof (csysWktName));
		}
		if ((flags & cs_WKTFLG_MAPNAMES) != 0)
		{
			csMapSt = csMapNameToNameC (csMapProjectedCSysKeyName,tmpBufr,
																  sizeof (tmpBufr),
																  nmFlavor,
																  csMapFlvrAutodesk,
																  cs_def->key_nm);
			if (csMapSt == csMapOk)
			{
				CS_stncp (csysWktName,tmpBufr,sizeof (baseName));
			}
		}

		/* Construct the final string. */
		sprintf (projWkt,"PROJCS[\"%s\",%s,PROJECTION[\"%s\"]%s,%s%s]",csysWktName,geogWkt,projection,parmWkt,unitWkt,prjAxis);
		CS_stncp (csWktBufr,projWkt,(int)bufrSize);
	}
	if (dtDefPtr != 0)
	{
		CS_free (dtDefPtr);
	}
	if (elDefPtr != 0)			//lint !e774   boolean always evaluats to false (I don't believe it_
	{
		CS_free (elDefPtr);
	}
	return rtnValue;
error:
	if (dtDefPtr != 0)
	{
		CS_free (dtDefPtr);
	}
	if (elDefPtr != 0)			//lint !e774   boolean always evaluats to false (I don't believe it_
	{
		CS_free (elDefPtr);
	}
	return -1;
}
bool EXP_LVL3 CSgetParamNm (char* paramName,size_t paramSize,EcsNameFlavor nmFlavor,int paramCode)
{
	bool ok;
	EcsMapSt nmSt;
	const char *kCp;

	ok = (paramCode >= 0 && paramCode <= cs_PRMCOD_MAXIDX) ||
		 (paramCode >= cs_WKTCOD_FEAST && paramCode <= cs_WKTCOD_SCLRED) ||
		 (paramCode >= cs_WKTCOD_DELTAX && paramCode <= cs_WKTCOD_BWSCAL);
	if (ok)
	{
		nmSt  = csMapIdToNameC (csMapParameterKeyName,paramName,paramSize,nmFlavor,csMapFlvrCsMap,paramCode);
		if (nmSt != csMapOk)
		{
			/* If that didn't work, we just use the CS-MAP parameter name. */
			nmSt  = csMapIdToNameC (csMapParameterKeyName,paramName,paramSize,csMapFlvrCsMap,csMapFlvrCsMap,paramCode);

			/* If that didn't work, extract a parameter name from the CS-MAP parameter table. */
			if (nmSt != csMapOk)
			{
				if (paramCode >= 0 && paramCode <= cs_PRMCOD_MAXIDX)
				{
					CS_stncp (paramName,csPrjprm[paramCode].label,sizeof (paramSize));
				}
				else
				{
					switch (paramCode) {
					case cs_WKTCOD_FEAST:
						kCp = "False Easting";
						break;
					case cs_WKTCOD_FNORTH:
						kCp = "False Northing";
						break;
					case cs_WKTCOD_ORGLNG:
						kCp = "Origin Longitude";
						break;
					case cs_WKTCOD_ORGLAT:
						kCp = "Origin Latitude";
						break;
					case cs_WKTCOD_SCLRED:
						kCp = "Scale Reduction";
						break;
					case cs_WKTCOD_DELTAX:
						kCp = "Delta X";
						break;
					case cs_WKTCOD_DELTAY:
						kCp = "Delta Y";
						break;
					case cs_WKTCOD_DELTAZ:
						kCp = "Delta Z";
						break;
					case cs_WKTCOD_ROTATX:
						kCp = "X Rotation";
						break;
					case cs_WKTCOD_ROTATY:
						kCp = "Y Rotation";
						break;
					case cs_WKTCOD_ROTATZ:
						kCp = "Z Rotation";
						break;
					case cs_WKTCOD_BWSCAL:
						kCp = "Datum Scale";
						break;
					default:
						kCp = "Parameter Value";
						break;
					}
				}
			}
		}
	}
	return ok;
}
bool EXP_LVL3 CSAddParamValue (char* wktBufr,size_t bufrSize,EcsNameFlavor nmFlavor,int paramCode,double paramValue,unsigned paramFlags)
{
	bool ok;
	int avail;
	int precision;
	char lclBufr [256];
	char paramName [128];

	precision = 6;
	ok = CSgetParamNm (paramName,sizeof (paramName),nmFlavor,paramCode);
	if (ok)
	{
		if (paramCode >= 0 && paramCode <= cs_PRMCOD_MAXIDX)
		{
			switch (csPrjprm[paramCode].log_type) {
			case cs_PRMLTYP_NONE:   precision =  6;   break;
			case cs_PRMLTYP_LNG:    precision = 14;   break;
			case cs_PRMLTYP_LAT:    precision = 14;   break;
			case cs_PRMLTYP_AZM:    precision = 14;   break;
			case cs_PRMLTYP_ANGD:   precision = 12;   break;
			case cs_PRMLTYP_CMPLXC: precision = 12;   break;
			case cs_PRMLTYP_ZNBR:   precision =  1;   break;
			case cs_PRMLTYP_HSNS:   precision =  1;   break;
			case cs_PRMLTYP_GHGT:   precision =  4;   break;
			case cs_PRMLTYP_ELEV:   precision =  4;   break;
			case cs_PRMLTYP_AFCOEF: precision = 12;   break;
			case cs_PRMLTYP_XYCRD:  precision =  3;   break;
			case cs_PRMLTYP_SCALE:  precision =  3;   break;
			default:
				precision = 6;
				break;
			}
		}
		else
		{
			switch (paramCode) {
			case cs_WKTCOD_FEAST:    precision =  3;  break;
			case cs_WKTCOD_FNORTH:   precision =  3;  break;
			case cs_WKTCOD_ORGLNG:   precision = 14;  break;
			case cs_WKTCOD_ORGLAT:   precision = 14;  break;
			case cs_WKTCOD_SCLRED:   precision = 12;  break;
			case cs_WKTCOD_DELTAX:   precision =  3;  break;
			case cs_WKTCOD_DELTAY:   precision =  3;  break;
			case cs_WKTCOD_DELTAZ:   precision =  3;  break;
			case cs_WKTCOD_ROTATX:   precision =  6;  break;
			case cs_WKTCOD_ROTATY:   precision =  6;  break;
			case cs_WKTCOD_ROTATZ:   precision =  6;  break;
			case cs_WKTCOD_BWSCAL:   precision =  6;  break;
			default:                 precision =  6;  break;
			}
		}

		/* Cosmetic */
		if ((paramFlags & cs_PARAMFLG_UTM) != 0)
		{
			if (paramCode == cs_WKTCOD_SCLRED)
			{
				if (fabs (paramValue - 0.9996) < 1.0E-12)
				{
					precision = 4;
				}
			}
			if (paramCode == cs_WKTCOD_ORGLAT)
			{
				if (fabs (paramValue - 0.0) < 1.0E-12)
				{
					precision = 3;
				}
			}
		}
		/* Defensive */
		if (precision <= 0 || precision >= 14)
		{
			precision = 14;
		}
		sprintf (lclBufr,",PARAMETER[\"%s\",%.*f]",paramName,precision,paramValue);
		avail = static_cast<int>(bufrSize - strlen (wktBufr)) - 1;
		ok = avail >= static_cast<int>(strlen (lclBufr));
	}
	if (ok)
	{
		strcat (wktBufr,lclBufr);
	}
	return ok;
}
