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

extern "C" const double cs_Zero;
extern "C" const double cs_One;
extern "C" int cs_Error;
extern "C" char csErrnam [];

int EXP_LVL1 CS_el2WktEx (char *bufr,size_t bufrSize,const char *elKeyName,int flavor,unsigned short flags)
{
	int rtnValue = -1;
	struct cs_Eldef_ *elDefPtr = 0;

	if (bufrSize > 0)
	{
		*bufr = '\0';
		elDefPtr = CS_eldef (elKeyName);
		if (elDefPtr != 0)
		{
			rtnValue = CSel2WktEx (bufr,bufrSize,(ErcWktFlavor)flavor,elDefPtr,flags);
			CS_free (elDefPtr);
			elDefPtr = 0;
		}
	}
	return rtnValue;
}

int EXP_LVL1 CS_el2Wkt (char *bufr,size_t bufrSize,const char *elKeyName,int flavor)
{
	int rtnValue = -1;
	struct cs_Eldef_ *elDefPtr = 0;

	if (bufrSize > 0)
	{
		*bufr = '\0';
		elDefPtr = CS_eldef (elKeyName);
		if (elDefPtr != 0)
		{
			rtnValue = CSel2Wkt (bufr,bufrSize,(ErcWktFlavor)flavor,elDefPtr);
			CS_free (elDefPtr);
			elDefPtr = 0;
		}
	}
	return rtnValue;
}

int EXP_LVL3 CSel2Wkt (char *bufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Eldef_ *el_def)
{
	int st;
	
	st = CSel2WktEx (bufr,bufrSize,flavor,el_def,0);
	return st;
}
int EXP_LVL3 CSel2WktEx (char *bufr,size_t bufrSize,enum ErcWktFlavor flavor,const struct cs_Eldef_ *el_def,unsigned short flags)
{ 
	short mapWktNames;
	EcsMapSt csMapSt;
    EcsNameFlavor nmFlavor;
	Const char* kCp;
	double rcpFlattening = 0.0;

	char wktEllipsoidName [96];
	char cTemp [1024];

   	CS_stcpy (csErrnam,"CSel2WktEx");
	if (bufr == 0)
	{
	    CS_erpt (cs_INV_ARG1);
	    return -(cs_Error);
	}
	if (bufrSize < 1)
	{
	    CS_erpt (cs_INV_ARG2);
	    return -(cs_Error);
	}

    *bufr = '\0';
    nmFlavor = csWktFlvrToCsMapFlvr (flavor);
    mapWktNames = ((flags & cs_WKTFLG_MAPNAMES) != 0);

	if (el_def->flat != 0.0)
	{
		rcpFlattening = cs_One / el_def->flat;
	}
	else
	{
		/* The real value is infinity, but that this a rather awkward number
		   to deal with.  So, we use zero to indicate a sphere.  Quite frankly,
		   the specification (if there really is one) for WKT does not handle
		   spheres at all.  We should probably just bag anything references to
		   a spehere. */
        rcpFlattening = cs_Zero;
	}

    if (!mapWktNames)
    {
        kCp = el_def->key_nm;
        if (flavor == wktFlvrEsri || flavor == wktFlvrOracle)
        {
            kCp = el_def->name;
        }
        CS_stncp (wktEllipsoidName,kCp,sizeof (wktEllipsoidName));
    }
    else
    {
	    csMapSt = csMapNameToNameC (csMapEllipsoidKeyName,wktEllipsoidName,
	                                                      sizeof (wktEllipsoidName),
	                                                      nmFlavor,
	                                                      csMapFlvrAutodesk,
	                                                      el_def->key_nm);
	    if (csMapSt != csMapOk)
	    {
		    CS_stncp (wktEllipsoidName,el_def->key_nm,sizeof (wktEllipsoidName));
	    }
	}
	sprintf (cTemp,"SPHEROID[\"%s\",%.3f,%.8f]",wktEllipsoidName,el_def->e_rad,rcpFlattening);
	CS_stncp (bufr,cTemp,(int)bufrSize);
	return 0;
}
