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

#include "cs_Legacy.h"

extern "C" int cs_Error;
extern "C" char csErrnam [];
extern "C" const double cs_Zero;

int EXP_LVL1 CS_dt2WktEx (char *bufr,size_t bufrSize,const char *dtKeyName,int flavor,unsigned short flags)
{
	int rtnValue = -1;
	struct cs_Dtdef_ *dtDefPtr = 0;
	struct cs_Eldef_ *elDefPtr = 0;
	
	if (bufrSize > 0)
	{
		*bufr = '\0';
		dtDefPtr = CS_dtdef (dtKeyName);
		if (dtDefPtr != 0)
		{
			elDefPtr = CS_eldef (dtDefPtr->ell_knm);
			if (elDefPtr != 0)
			{
				rtnValue = CSdt2WktEx (bufr,bufrSize,0,0,(ErcWktFlavor)flavor,dtDefPtr,elDefPtr,flags);
				CS_free (elDefPtr);
				elDefPtr = 0;
			}
			CS_free (dtDefPtr);
			dtDefPtr = 0;
		}
	}
	return rtnValue;			
}

int EXP_LVL1 CS_dt2Wkt (char *bufr,size_t bufrSize,const char *dtKeyName,int flavor)
{
	int rtnValue = -1;
	struct cs_Dtdef_ *dtDefPtr = 0;
	struct cs_Eldef_ *elDefPtr = 0;
	
	if (bufrSize > 0)
	{
		*bufr = '\0';
		dtDefPtr = CS_dtdef (dtKeyName);
		if (dtDefPtr != 0)
		{
			elDefPtr = CS_eldef (dtDefPtr->ell_knm);
			if (elDefPtr != 0)
			{
				rtnValue = CSdt2Wkt (bufr,bufrSize,0,0,(ErcWktFlavor)flavor,dtDefPtr,elDefPtr);
				CS_free (elDefPtr);
				elDefPtr = 0;
			}
			CS_free (dtDefPtr);
			dtDefPtr = 0;
		}
	}
	return rtnValue;			
}
int EXP_LVL3 CSdt2Wkt (char *datum,size_t datumSize,char *geoTran,size_t geoTranSize,enum ErcWktFlavor flavor,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def)
{
	int st;
	
	st = CSdt2WktEx (datum,datumSize,geoTran,geoTranSize,flavor,dt_def,el_def,0);
	return st;
}
int EXP_LVL3 CSdt2WktEx (char *datum,size_t datumSize,char *geoTran,size_t geoTranSize,enum ErcWktFlavor flavor,const struct cs_Dtdef_ *dt_def,const struct cs_Eldef_ *el_def,unsigned short flags)
{
	bool ok;
    bool mapWktNames;
	EcsMapSt csMapSt;
    EcsNameFlavor nmFlavor;
	int status;

	short parmCount;

	Const char* kCp;
	const char *method;

	struct cs_Eldef_ *elDef;

	char wktDatumName [96];
	char gcsDatumName [96];
	char elWkt [256];
	char dtWkt [512];
	char geoCSdatum [512];
	char geoCSwgs84 [] = "GEOGCS[\"WGS84.LL\",DATUM[\"WGS84\",SPHEROID[\"WGS84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]";
	char paramsWkt [512];
	char geoTranName [96];
	char geoTranWkt [1024];
	char mappedDtName [96];
	char prmNameXTr [96];
	char prmNameYTr [96];
	char prmNameZTr [96];
	char prmNameXRot [96];
	char prmNameYRot [96];
	char prmNameZRot [96];
	char prmNameScale [96];

    // Prepare for some sort of parameter error.
   	CS_stcpy (csErrnam,"CSel2WktEx");

    // Check our arguments to a limited degree.
    if (dt_def == 0)
    {
	    CS_erpt (cs_INV_ARG6);
	    return -(cs_Error);
    }

    nmFlavor = csWktFlvrToCsMapFlvr (flavor);
    mapWktNames = ((flags & cs_WKTFLG_MAPNAMES) != 0);
	if (datum != 0 && datumSize != 0) *datum = '\0';
	if (geoTran != 0 && geoTranSize != 0) *geoTran = '\0';
	if ((datum == 0 || datumSize <= 1) &&
	    (geoTran == 0 || geoTranSize <=1))
	{
	    CS_erpt (cs_INV_ARG1);
	    return -(cs_Error);
	}

	if (el_def == NULL)
	{
		elDef = CS_eldef (dt_def->ell_knm);
		if (elDef == NULL)
		{
    	    CS_erpt (cs_WKT_INCNSIST);
    	    return -(cs_Error);
		}
		status = CSel2WktEx (elWkt,sizeof (elWkt),flavor,elDef,flags);
		CS_free (elDef);
		if (status != 0)
		{
		    // CSel2WktEx should have reported the reason why.
			return -1;
		}
	}
	else
	{
		status = CSel2WktEx (elWkt,sizeof (elWkt),flavor,el_def,flags);
		if (status != 0)
		{
		    // CSel2WktEx should have reported the reason why.
			return -1;
		}
	}

	/* Set up the parameter names consistent with the flavor. */
	ok  = CSgetParamNm (prmNameXTr,sizeof (prmNameXTr),nmFlavor,cs_WKTCOD_DELTAX);
	ok |= CSgetParamNm (prmNameYTr,sizeof (prmNameYTr),nmFlavor,cs_WKTCOD_DELTAY);
	ok |= CSgetParamNm (prmNameZTr,sizeof (prmNameZTr),nmFlavor,cs_WKTCOD_DELTAZ);
	ok |= CSgetParamNm (prmNameXRot,sizeof (prmNameXRot),nmFlavor,cs_WKTCOD_ROTATX);
	ok |= CSgetParamNm (prmNameYRot,sizeof (prmNameYRot),nmFlavor,cs_WKTCOD_ROTATY);
	ok |= CSgetParamNm (prmNameZRot,sizeof (prmNameZRot),nmFlavor,cs_WKTCOD_ROTATZ);
	ok |= CSgetParamNm (prmNameScale,sizeof (prmNameScale),nmFlavor,cs_WKTCOD_BWSCAL);
	if (!ok)
	{
		CS_stncp (csErrnam,"CS_cs2Wkt:2",MAXPATH);
		CS_erpt (cs_ISER);
		return -1;
	}

    if ((flags & cs_WKTFLG_MAPNAMES) == 0
        && (flavor == wktFlvrEsri || flavor == wktFlvrOracle))
    {
        CS_stncp (wktDatumName,dt_def->name,sizeof (wktDatumName));
        CS_stncp (gcsDatumName,dt_def->name,sizeof (gcsDatumName));
    }
    else
    {
	    CS_stncp (wktDatumName,dt_def->key_nm,sizeof (wktDatumName));
	    CS_stncp (gcsDatumName,dt_def->key_nm,sizeof (gcsDatumName));
    }
	if (strlen (gcsDatumName) < sizeof (gcsDatumName) - 4)
	{
		strcat (gcsDatumName,".LL");
	}
	
	if ((flags & cs_WKTFLG_MAPNAMES) != 0)
    {
	    csMapSt = csMapNameToNameC (csMapDatumKeyName,mappedDtName,
                                                      sizeof (mappedDtName),
                                                      nmFlavor,
                                                      csMapFlvrAutodesk,
                                                      dt_def->key_nm);
		if (csMapSt == csMapOk)
		{
			if (flavor == wktFlvrEsri)
			{
				CS_stncp (wktDatumName,mappedDtName,sizeof (wktDatumName));
				CS_stcpy (gcsDatumName,"GCS_");
				CS_stncp (&gcsDatumName [4],&mappedDtName [2],sizeof (gcsDatumName) - 4);
				CS_stncp (geoCSwgs84,"GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]",sizeof (geoCSwgs84));
			}
			else if (flavor == wktFlvrOracle)
			{
				CS_stncp (wktDatumName,mappedDtName,sizeof (wktDatumName));
				CS_stncp (gcsDatumName,mappedDtName,sizeof (gcsDatumName));
				CS_stncp (geoCSwgs84,"GEOGCS[\"WGS 84\",DATUM [\"WGS 84\",SPHEROID [\"WGS 84\",6378137.000000,298.257224]],PRIMEM [\"Greenwich\",0.000000],UNIT[\"Decimal Degree\", 0.01745329251994330]]",sizeof (geoCSwgs84));
			}
			else
			{
				CS_stncp (wktDatumName,mappedDtName,sizeof (wktDatumName));
				CS_stncp (gcsDatumName,mappedDtName,sizeof (gcsDatumName));
			}
		}
	}
	sprintf (dtWkt,"DATUM[\"%s\",%s]",wktDatumName,elWkt);
	sprintf (geoCSdatum,"GEOGCS[\"%s\",%s,PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]",gcsDatumName,dtWkt);

	/* I suspect that there are different "flavors" of the following.  I'll have to
	   do some more research before I know what they might be. */
	switch (dt_def->to84_via) {
	case cs_DTCTYP_MOLO:
		parmCount = 3;
		method = "Geocentric_Translation";
		break;
	case cs_DTCTYP_3PARM:
	case cs_DTCTYP_GEOCTR:
		parmCount = 3;
		method = "Geocentric_Translation";
		break;
	case cs_DTCTYP_BURS:
		parmCount = 7;
		method = "Coordinate_Frame";
		break;
	case cs_DTCTYP_7PARM:
		parmCount = 7;
		method = "Coordinate_Frame";
		break;
	case cs_DTCTYP_NAD83:
	case cs_DTCTYP_WGS84:
	case cs_DTCTYP_GDA94:
	case cs_DTCTYP_NZGD2K:
	case cs_DTCTYP_ETRF89:
	case cs_DTCTYP_RGF93:
		// This is the case where the datum is considered to be equivalent to
		// WGS84, no transformation is required.
		parmCount = -3;
		method = "Geocentric_Translation";
		break;
		
	case cs_DTCTYP_NAD27:
	case cs_DTCTYP_MREG:
	case cs_DTCTYP_WGS72:
	case cs_DTCTYP_HPGN:
	case cs_DTCTYP_AGD66:
	case cs_DTCTYP_6PARM:
	case cs_DTCTYP_4PARM:
	case cs_DTCTYP_AGD84:
	case cs_DTCTYP_NZGD49:
	case cs_DTCTYP_CSRS:  
	case cs_DTCTYP_ATS77:
	case cs_DTCTYP_TOKYO:
	case cs_DTCTYP_ED50:
	case cs_DTCTYP_DHDN:
	case cs_DTCTYP_CHENYX:
	default:
		/* These may be supported in EPSG, but they are not supported in WKT. */
		parmCount = 0;
		method = "Unsupported";
		break;
	}

	if (parmCount == 3)
	{
		sprintf (paramsWkt,"METHOD[\"%s\"],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f]",
															method,
															prmNameXTr,dt_def->delta_X,
															prmNameYTr,dt_def->delta_Y,
															prmNameZTr,dt_def->delta_Z);
	}
	else if (parmCount == 7)
	{
		sprintf (paramsWkt,"METHOD[\"%s\"],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f],"
										  "PARAMETER[\"%s\",%.6f],PARAMETER[\"%s\",%.6f],PARAMETER[\"%s\",%.6f],"
										  "PARAMETER[\"%s\",%.8f]",
										  method,
										  prmNameXTr,dt_def->delta_X,
										  prmNameYTr,dt_def->delta_Y,
										  prmNameZTr,dt_def->delta_Z,
										  prmNameXRot,dt_def->rot_X,
										  prmNameYRot,dt_def->rot_Y,
										  prmNameZRot,dt_def->rot_Z,
										  prmNameScale,dt_def->bwscale);
	}
	else if (parmCount == -3)
	{
		sprintf (paramsWkt,"METHOD[\"%s\"],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f],PARAMETER[\"%s\",%.4f]",
															method,
															prmNameXTr,cs_Zero,
															prmNameYTr,cs_Zero,
															prmNameZTr,cs_Zero);
	}
	else
	{
		sprintf (paramsWkt,"METHOD[\"Unsupport_Method\"]");
	}

    if ((flags & cs_WKTFLG_MAPNAMES) == 0
        && (flavor == wktFlvrEsri || flavor == wktFlvrOracle))
    {
        CS_stncp (geoTranName,dt_def->name,sizeof (geoTranName) - 15); 
    }
    else
    {
    	CS_stncp (geoTranName,dt_def->key_nm,sizeof (geoTranName) - 15); 
    }
    if (mapWktNames)
    {
	    csMapSt = csSysNameToNameC (csMapDatumKeyName,mappedDtName,
                                                      sizeof (mappedDtName),
                                                      nmFlavor,
                                                      dt_def->key_nm);
        if (csMapSt == csMapOk)
        {
		    if (flavor == wktFlvrEsri)
		    {
		        kCp = mappedDtName;
		        if (*kCp == 'D' && *(kCp + 1) == '_')
		        {
		            kCp += 2;
		        }
			    CS_stncp (geoTranName,kCp,sizeof (geoTranName) - 15);
		    }
        }
	}
	strcat (geoTranName,"_To_WGS_1984");
	sprintf (geoTranWkt,"GEOTRAN[\"%s\",%s,%s,%s]",geoTranName,geoCSdatum,geoCSwgs84,paramsWkt);

	/* We now redo the datum, this time we'll put the TOQGS84 thing in.
	   However, it makes no sense to do this unless there are some valid
	   paremeters. */
	if (parmCount == 3 || parmCount == 7)
	{
		sprintf (dtWkt,"DATUM[\"%s\",%s,TOWGS84[%.4f,%.4f,%.4f,%.6f,%.6f,%.6f,%.8f]]",wktDatumName,elWkt,
												dt_def->delta_X,dt_def->delta_Y,dt_def->delta_Z,
												dt_def->rot_X,dt_def->rot_Y,dt_def->rot_Z,dt_def->bwscale);
	}
/******************************************************************************
	Clients have indicated that this feature is rather undesireable; so we
	comment it out, leaving the code intact, in case anyone has a different
	opinion.
		
	else if (parmCount == -3)
	{
		sprintf (dtWkt,"DATUM[\"%s\",%s,TOWGS84[%.4f,%.4f,%.4f,%.6f,%.6f,%.6f,%.8f]]",wktDatumName,elWkt,
												cs_Zero,cs_Zero,cs_Zero,
												cs_Zero,cs_Zero,cs_Zero,cs_Zero);
	}
******************************************************************************/
	else
	{
		/* The datum type is one that is unsupported by WKT.  We simply write the
		   datum definition with the name of the datum. */
		sprintf (dtWkt,"DATUM[\"%s\",%s]",wktDatumName,elWkt);
	}	
	if (datumSize > 1)
	{
		CS_stncp (datum,dtWkt,(int)datumSize);
	}
	if (geoTranSize > 1)
	{
		CS_stncp (geoTran,geoTranWkt,(int)geoTranSize);
	}
	return 0;
}
